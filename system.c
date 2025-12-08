#include<stdio.h>
#include<string.h>
#include "global.h"
#include "login.h"
#define N 10

Account accounts[N];//最多有N个账号
int account_count = 0;//当前帐号数量
// 1.系统初始化：启动时加载备份数据
void system_init() {
    // 先清空现有数据
    account_count = 0;
    memset(accounts, 0, sizeof(accounts));

    // 尝试读取备份文件
    FILE *fp = fopen("bank_backup.txt", "r");
    if (!fp) {
        printf("系统初始化：无备份数据，新建系统！\n");
        return;
    }

    // 按行读取备份数据
    char line[50];
    while (fgets(line, sizeof(line), fp) && account_count < 10) {
        // 解析账号（格式：账号：622202010001，状态：正常）
        sscanf(line, "账号：%[^,]，状态：%*s", accounts[account_count].acc_id);
        // 解析状态
        if (strstr(line, "冻结")) {
            accounts[account_count].status = ACC_FROZEN;
        } else {
            accounts[account_count].status = ACC_NORMAL;
        }
        account_count++;
    }
    fclose(fp);
    printf("系统初始化：已加载 %d 条账号数据！\n", account_count);
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
