#include "login.h"
#include "account.h"

// md5加密函数
void password_md5(const char *input, char *output)
{
    MD5_CTX ctx;
    unsigned char digest[16];
    MD5Init(&ctx); // 初始化MD5计算上下文
    MD5Update(&ctx, (unsigned char *)input, strlen(input));
    MD5Final(&ctx, digest); // 结束MD5计算
    for (int i = 0; i < 16; i++)
        sprintf(output + i * 2, "%02x", digest[i]);
    output[32] = '\0';
}

// 密码比较函数(相同返回1，不同返回0)
int cmp_password(const char *input, const char *password)
{
    char input_md5[33];
    password_md5(input, input_md5);
    if (strcmp(input_md5, password) == 0)
        return 1;
    else
        return 0;
}

// 登录验证
int login(const char *acc_id, const char *pwd)
{
    Account *acc = find_account(acc_id);
    if (acc == NULL)
        return 0; // 账号并不存在
    if (acc->status == ACC_FROZEN)
    {
        save_accounts();
        return 3;
    } // 登录被锁定了

    if (cmp_password(pwd, acc->pwd_hash))
    {
        acc->login_fail_count = 0; // 登录成功，失败次数清零
        save_accounts();
        return 1;
    }
    else
    {
        acc->login_fail_count++;
        if (acc->login_fail_count >= MAX_LOGIN_FAIL)
            acc->status = ACC_FROZEN;
        save_accounts();
        return 2; // 密码错误
    }
}

// 挂失
int lost(const char *acc_id)
{
    Account *acc = find_account(acc_id);
    if (acc == NULL)
        return 0; // 账号不存在
    else
    {
        if (acc->status == ACC_LOST)
            return 2; // 已挂失
        if (acc->status == ACC_FROZEN)
            return 3; // 已冻结
        acc->status = ACC_LOST;
        save_accounts();
        return 1; // 挂失成功
    }
}

// 冻结
int freeze(const char *acc_id)
{
    Account *acc = find_account(acc_id);
    if (acc == NULL)
        return 0; // 账号不存在
    else
    {
        if (acc->status == ACC_LOST)
            return 2; // 已挂失
        if (acc->status == ACC_FROZEN)
            return 3; // 已冻结
        acc->status = ACC_FROZEN;
        save_accounts;
        return 1; // 冻结成功
    }
}

// 解冻(挂失和冻结共用)
int unfreeze(const char *acc_id)
{
    Account *acc = find_account(acc_id);
    if (acc == NULL)
        return 0; // 账号不存在
    if (acc->status == ACC_NORMAL)
        return 2; // 账户已经是正常状态
    acc->status = ACC_NORMAL;
    acc->login_fail_count = 0;
    save_accounts;
    return 1; // 解冻成功
}
