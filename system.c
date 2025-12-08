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
    for (int i = 0; i < HASH_SIZE; i++)
    {
        acc_hash[i] = NULL;
    }
    trans_head = NULL;
    current_login_acc[0] = '\0';

    // 加载账户与交易记录
    load_accounts(ACCOUNT_FILE);
    load_transactions_from_file();
}

// 保存数据
void system_save()
{
    save_accounts();
    save_transactions_to_file();
}

// 数据备份
void backup_data()
{
    save_accounts();
    save_transactions_to_file();
    printf("备份完成：账户与交易记录已写入文件\n");
}
