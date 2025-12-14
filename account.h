#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "global.h"
#include <stdio.h>
#include <string.h>

// 生成唯一哈希值
int hash_acc_id(const char *acc_id);

// 账户查询函数
Account *find_account(const char *acc_id);

// 按哈希值插入账户节点
void insert_account(Account *acc);

// 删除账号（1失败/0成功）
int remove_account(const char *acc_id);

// 释放全部账户链表
void free_all_accounts(void);

// 重新加载账户缓存
void reload_accounts_cache(void);

// 保存全部账户
int save_accounts(void);

// 开户(0失败/1成功)
int create_account(const char *name, const char *password, double initial_balance, char *generated_id);

// 生成id
void generate_account_id(char *acc_id, size_t size);

// 销户
int close_account(const char *acc_id);

// 保存所有账户信息(失败-1/成功返回保存文件的个数)
int save_all_accounts(const char *filename);

// 修改账户信息
int update_account_info(const char *acc_id, const char *field, const char *new_value);

// 读取文件
int load_accounts(const char *filename);

#endif
