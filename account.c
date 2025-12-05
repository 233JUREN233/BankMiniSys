#include "account.h"
#include <string.h>

// 计算哈希值
static int hash_acc_id(const char *acc_id)
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
            return 1;
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


