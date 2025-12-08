#include <stdio.h>
#include <string.h>
#include "global.h"
#include "login.h"
#include "system.h"
#include "account.h"
#include "bill.h"

// 系统初始化：清空内存状态并加载账户、交易记录
void system_init()
{
    // 清空全局内存状态
    for (int i = 0; i < HASH_SIZE; i++)
    {
        acc_hash[i] = NULL;
    }
    trans_head = NULL;
    current_login_acc[0] = '\0';

    // 加载账户与交易记录
    load_accounts();
    load_transactions_from_file();
}

// 数据备份：保存账户与交易记录到文件
void backup_data()
{
}
