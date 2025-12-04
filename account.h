#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "global.h"
#include <stdio.h>
#include <string.h>

// 生成唯一哈希值
static int hash_acc_id(const char *acc_id);

// 账户查询函数
Account *find_account(const char *acc_id);

// 按哈希值插入账户节点
void insert_account(Account *acc);

// 删除账号（返回 1 成功 / 0 失败）
int remove_account(const char *acc_id);

// 释放全部账户链表（程序退出时调用，防止leak）
void free_all_accounts(void);

#endif
