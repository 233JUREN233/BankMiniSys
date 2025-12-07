#include "account.h"
#include <string.h>
#include <math.h>

// 计算哈希值
int hash_acc_id(const char *acc_id)
{
    unsigned int hash = 0;

    for (int i = 0; acc_id[i] != '\0'; i++)
    {
        hash = hash * 131 + (unsigned char)acc_id[i];
    }

    return hash % HASH_SIZE;
}

// 按哈希值插入账户节点
void insert_account(Account *acc)
{
    int idx = hash_acc_id(acc->acc_id);
    acc->next = acc_hash[idx];
    acc_hash[idx] = acc;
}

// 按账号查找账户
Account *find_account(const char *acc_id)
{
    int idx = hash_acc_id(acc_id);
    Account *curr = acc_hash[idx];

    while (curr)
    {
        if (strcmp(curr->acc_id, acc_id) == 0)
        {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

// 删除账号
int remove_account(const char *acc_id)
{
    int idx = hash_acc_id(acc_id);
    Account *curr = acc_hash[idx], *prev = NULL;

    while (curr)
    {
        if (strcmp(curr->acc_id, acc_id) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                acc_hash[idx] = curr->next;
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }

    return 0;
}

// 释放全部账户链表
void free_all_accounts(void)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Account *curr = acc_hash[i];
        while (curr)
        {
            Account *next = curr->next;
            free(curr);
            curr = next;
        }
        acc_hash[i] = NULL;
    }
}

//生成id
void generate_account_id(char *acc_id, size_t size)
{
    static int counter = 10001;

    if(size >= 20)
    {
        sprintf(acc_id,"%s%08d",ACCOUNT_PREFIX,counter);
        counter++;
    }

}

//开户(返回0失败，返回1成功)
int create_account(const char *name, const char *password, double initial_balance, char *generated_id)
{
    if (!name || !password || initial_balance < 0) {
        return 0; 
    }
    char new_id[20];
    generate_account_id(new_id, sizeof(new_id));

    Account *new_acc = malloc(sizeof(Account));
    strcpy(new_acc->acc_id, new_id);
    strcpy(new_acc->name, name);
    password_md5(password, new_acc->pwd_hash);

    new_acc->balance = initial_balance;
    new_acc->status = ACC_NORMAL;
    new_acc->login_fail_count = 0;
    new_acc->next = NULL; 

    void insert_account(new_acc);
    
    FILE *fp = fopen(ACCOUNT_FILE, "ab");  
    if (fp) {
        fwrite(new_acc, sizeof(Account), 1, fp);
        fclose(fp);
    }

    if(new_id){
        strcpy(generated_id,new_id);
    }

    return 1;
}

//销户
int close_account(const char *acc_id)
{
    
}
