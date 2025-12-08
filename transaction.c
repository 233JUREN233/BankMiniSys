#include <stdio.h>
#include "global.h"
#include "login.h"
#include "transaction.h"
#include "bill.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

// 校验金额是否合法
static int isValidAmount(double amount)
{
    return (amount > 0.0001) ? 1 : 0;
}

// 获取当前时间
static void getCurrentTime(char *time_buf, int buf_len)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(time_buf, buf_len, "%Y-%m-%d %H:%M", tm_info);
}

// 生成交易记录
void createTransactionRecord(const char *acc_id, TransType type, double amount, const char *target_acc)
{
    Transaction *new_trans = (Transaction *)malloc(sizeof(Transaction));
    if (new_trans == NULL)
    {
        printf("交易记录生成失败：内存分配失败！\n");
        return;
    }
    strncpy(new_trans->acc_id, acc_id, sizeof(new_trans->acc_id) - 1);
    new_trans->acc_id[sizeof(new_trans->acc_id) - 1] = '\0';
    new_trans->type = type;
    new_trans->amount = amount;
    strncpy(new_trans->target_acc, target_acc ? target_acc : "", sizeof(new_trans->target_acc) - 1);
    new_trans->target_acc[sizeof(new_trans->target_acc) - 1] = '\0';
    getCurrentTime(new_trans->time, sizeof(new_trans->time));
    new_trans->next = trans_head;
    trans_head = new_trans;
    const char *type_str = (type == TRANS_DEPOSIT) ? "存款" : (type == TRANS_WITHDRAW) ? "取款"
                                                                                       : "转账";
    printf("交易记录：%s | %s | 金额%.2f | 对方账号：%s\n",
           new_trans->time, type_str, amount, new_trans->target_acc);

    save_transactions_to_file();
}

// 存款
int deposit(double amount, const char *trade_pwd)
{
    // 校验登录状态
    if (strlen(current_login_acc) == 0)
    {
        printf("存款失败：请先登录！\n");
        return 1;
    }
    // 查找账户
    Account *curr_acc = find_account(current_login_acc);
    if (curr_acc == NULL)
    {
        printf("存款失败：当前登录账号不存在！\n");
        return 2;
    }
    // 校验账户状态
    if (curr_acc->status != ACC_NORMAL)
    {
        printf("存款失败：账户状态异常(%s)!\n",
               curr_acc->status == ACC_FROZEN ? "冻结" : "挂失");
        return 3;
    }
    // 校验金额
    if (!isValidAmount(amount))
    {
        printf("存款失败:金额必须大于0!\n");
        return 4;
    }
    // 校验交易密码
    if (trade_pwd != NULL && !cmp_password(trade_pwd, curr_acc->pwd_hash))
    {
        printf("存款失败：交易密码错误！\n");
        return 5;
    }
    // 执行存款操作
    curr_acc->balance += amount;
    // 生成交易记录
    createTransactionRecord(current_login_acc, TRANS_DEPOSIT, amount, "");
    // 输出结果
    printf("存款成功！当前余额：%.2f\n", curr_acc->balance);
    return 0;
}

// 取款
int withdraw(double amount, const char *trade_pwd)
{
    // 校验登录状态
    if (strlen(current_login_acc) == 0)
    {
        printf("取款失败：请先登录！\n");
        return 1;
    }
    // 查找账户
    Account *curr_acc = find_account(current_login_acc);
    if (curr_acc == NULL)
    {
        printf("取款失败：当前登录账号不存在！\n");
        return 2;
    }
    // 校验账户状态
    if (curr_acc->status != ACC_NORMAL)
    {
        printf("取款失败：账户状态异常(%s)!\n",
               curr_acc->status == ACC_FROZEN ? "冻结" : "挂失");
        return 3;
    }
    // 校验金额
    if (!isValidAmount(amount))
    {
        printf("取款失败:金额必须大于0!\n");
        return 4;
    }
    // 校验余额
    if (curr_acc->balance < amount)
    {
        printf("取款失败：余额不足！当前余额：%.2f,需取款：%.2f\n",
               curr_acc->balance, amount);
        return 5;
    }
    // 校验交易密码
    if (trade_pwd != NULL && !cmp_password(trade_pwd, curr_acc->pwd_hash))
    {
        printf("取款失败：交易密码错误！\n");
        return 6;
    }
    // 执行取款操作
    curr_acc->balance -= amount;
    // 生成交易记录
    createTransactionRecord(current_login_acc, TRANS_WITHDRAW, amount, "");
    // 输出结果
    printf("取款成功！当前余额：%.2f\n", curr_acc->balance);
    return 0;
}

// 转账
int transfer(const char *target_acc_id, double amount, const char *trade_pwd)
{
    // 校验登录状态
    if (strlen(current_login_acc) == 0)
    {
        printf("转账失败：请先登录！\n");
        return 1;
    }
    // 查找转出账户
    Account *from_acc = find_account(current_login_acc);
    if (from_acc == NULL)
    {
        printf("转账失败：转出账号不存在！\n");
        return 2;
    }
    // 查找转入账户
    Account *to_acc = find_account(target_acc_id);
    if (to_acc == NULL)
    {
        printf("转账失败：转入账号不存在！\n");
        return 3;
    }
    // 校验转出账户状态
    if (from_acc->status != ACC_NORMAL)
    {
        printf("转账失败：转出账户状态异常(%s)!\n",
               from_acc->status == ACC_FROZEN ? "冻结" : "挂失");
        return 4;
    }
    // 校验转入账户状态
    if (to_acc->status != ACC_NORMAL)
    {
        printf("转账失败：转入账户状态异常(%s)!\n",
               to_acc->status == ACC_FROZEN ? "冻结" : "挂失");
        return 5;
    }
    // 校验金额
    if (!isValidAmount(amount))
    {
        printf("转账失败:金额必须大于0!\n");
        return 6;
    }
    // 计算手续费和总扣款金额
    double fee = amount * TRANS_FEE_RATE;
    double total_deduct = amount + fee;
    // 校验转出方余额
    if (from_acc->balance < total_deduct)
    {
        printf("转账失败：余额不足！需扣除本金%.2f + 手续费%.2f = 合计%.2f,当前余额%.2f\n",
               amount, fee, total_deduct, from_acc->balance);
        return 7;
    }
    // 校验交易密码
    if (trade_pwd != NULL && !cmp_password(trade_pwd, from_acc->pwd_hash))
    {
        printf("转账失败：交易密码错误！\n");
        return 8;
    }
    // 执行转账
    from_acc->balance -= total_deduct;
    to_acc->balance += amount;
    // 生成交易记录
    createTransactionRecord(current_login_acc, TRANS_TRANSFER, amount, target_acc_id);
    createTransactionRecord(target_acc_id, TRANS_TRANSFER, amount, current_login_acc);
    // 输出结果
    printf("转账成功！\n");
    printf("转出账号：%s | 扣除本金%.2f + 手续费%.2f | 当前余额：%.2f\n",
           current_login_acc, amount, fee, from_acc->balance);
    printf("转入账号：%s | 到账金额%.2f | 当前余额：%.2f\n",
           target_acc_id, amount, to_acc->balance);
    return 0;
}
