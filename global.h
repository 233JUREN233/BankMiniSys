#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>

// 账户状态
typedef enum
{
    ACC_NORMAL, // 正常状态
    ACC_FROZEN, // 冻结状态
    ACC_LOST    // 挂失状态
} AccountStatus;

// 交易类型
typedef enum
{
    TRANS_DEPOSIT,  // 存款
    TRANS_WITHDRAW, // 取款
    TRANS_TRANSFER  // 转账（含转入/转出）
} TransType;

#define HASH_SIZE 50                  // 账户哈希表大小
#define MAX_LOGIN_FAIL 5              // 登录失败最大次数
#define TRANS_FEE_RATE 0.005          // 转账手续费率
#define ACCOUNT_FILE "accounts.dat"   // 账户数据存储文件
#define TRANS_FILE "transactions.dat" // 交易记录存储文件
#define ACCOUNT_PREFIX "6222"         // 账号前缀

// 账户结构体
typedef struct Account
{
    char acc_id[20];      // 账号
    char name[30];        // 开户人姓名
    char pwd_hash[33];    // 密码哈希值
    double balance;       // 账户余额
    AccountStatus status; // 账户状态（安全/系统模块用）
    int login_fail_count; // 登录失败次数
    struct Account *next; // 链表节点
} Account;

// 交易记录结构体
typedef struct Transaction
{
    char acc_id[20];          // 关联账号
    TransType type;           // 交易类型
    double amount;            // 交易金额
    char target_acc[20];      // 转账对方账号（无则为空）
    char time[20];            // 交易时间
    struct Transaction *next; // 链表节点
} Transaction;

extern Account *acc_hash[HASH_SIZE]; // 账户哈希表
extern Transaction *trans_head;      // 交易记录链表头
extern char current_login_acc[20];   // 当前登录账号（空字符串=未登录）

#endif
