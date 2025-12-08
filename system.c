#include<stdio.h>
#include<string.h>
#include "global.h"
#include "login.h"
#include "system.h"
#include "account.h"
#include "bill.h"


// 1.系统初始化：启动时加载备份数据
void system_init() {
    // 先清空现有数据
    // 读取账号数据
    load_accounts();
    // 读取账号信息
    load_transactions_from_file();
}
    
    
    

// 2.数据备份：保存到文件
void backup_data() {
    FILE *fp = fopen("bank_backup.txt", "w");
    if (!fp) {
        printf("备份失败！\n");
        return;
    }
    for (int i = 0; i < account_count; i++) {
        fprintf(fp, "账号：%s,状态:%s\n", 
                accounts[i].acc_id, 
                accounts[i].status == ACC_FROZEN ? "冻结" : "正常");
    }
    fclose(fp);
    printf("数据已备份！\n");
}