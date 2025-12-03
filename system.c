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
// 1.系统初始化：重置所有账号数据
void system_init (){
    account_count = 0;
    memset(accounts,0,sizeof(accounts));
    printf("系统已初始化，所有数据已清空\n");
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
