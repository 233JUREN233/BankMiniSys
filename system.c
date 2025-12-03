#include<stdio.h>
#include<string.h>
// 定义账号结构体（存储账号，状态）
typedef struct{
    char account[20];//账号最多20位
    int is_frozen;//0是正常状态，1是冻结状态
}BankAccount;
// 定义全局变量
BankAccount accounts[10];//最多有10个账号
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
        sscanf(line, "账号：%[^,]，状态：%*s", accounts[account_count].account);
        // 解析状态
        if (strstr(line, "冻结")) {
            accounts[account_count].is_frozen = 1;
        } else {
            accounts[account_count].is_frozen = 0;
        }
        account_count++;
    }
    fclose(fp);
    printf("系统初始化：已加载 %d 条账号数据！\n", account_count);
}


// 数据备份：保存到文件
void backup_data() {
    FILE *fp = fopen("bank_backup.txt", "w");
    if (!fp) {
        printf("备份失败！\n");
        return;
    }
    for (int i = 0; i < account_count; i++) {
        fprintf(fp, "账号：%s,状态:%s\n", 
                accounts[i].account, 
                accounts[i].is_frozen ? "冻结" : "正常");
    }
    fclose(fp);
    printf("数据已备份！\n");
}
// 2.冻结账号
void freeze_account(const char *acc){
    for (int i=0;i<account_count;++i){
        if(strcmp(accounts[i].account,acc)==0){
            if (accounts[i].is_frozen)printf("账号 %s 已处于冻结状态! \n",acc);
            else{
            accounts[i].is_frozen = 1;
            printf("账号 %s 已冻结成功！\n",acc);
            }
            return;
        }    
    }
    printf("未找到账号 %s! \n",acc);
}
// 3.解冻账号
void unfreeze_account(const char *acc){
     for (int i=0;i<account_count;++i){
        if(strcmp(accounts[i].account,acc)==0){
            if (!accounts[i].is_frozen)printf("账号 %s 已处于正常状态! \n",acc);
            else{
            accounts[i].is_frozen = 0;
            printf("账号 %s 已解冻成功！\n",acc);
            }
            return;
        }    
    }
    printf("未找到账号 %s! \n",acc);
}
// 4.数据备份
void backup_data() {
    FILE *fp = fopen("bank_backup.txt", "w");
    if (!fp) {
        printf("备份失败：无法打开文件！\n");
        return;
    }
    for (int i = 0; i < account_count; i++) {
        fprintf(fp, "账号：%s,状态:%s\n", 
                accounts[i].account, 
                accounts[i].is_frozen ? "冻结" : "正常");
    }
    fclose(fp);
    printf("数据已备份到 bank_backup.txt!\n");
}