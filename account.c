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
    return 1;
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

// 生成账号
int make_account(char *acc_id, int num)
{
    sprintf(acc_id, "%s%04d", ACCOUNT_PREFIX, num);
    if (find_account(acc_id) == NULL)
        return 0;
    else
        return 1;
}

// 查找账号的流程如下：

// 先用账号字符串计算哈希值，得到槽位下标（比如 0~49）。
// 在对应槽位的链表里，从头到尾遍历每个节点，用 strcmp 比较账号字符串，找到完全匹配的那个账号。

// 哈希表里的链表不是只有一条，而是有 HASH_SIZE 条，每个哈希槽（acc_hash[0] ~ acc_hash[49]）都可以挂一条链表。
// 每条链表只存属于该哈希值的账户，所有链表是“并列”挂在哈希数组上的，而不是连成一条大链。这样查找时只需要遍历对应槽位的链表，效率很高。
