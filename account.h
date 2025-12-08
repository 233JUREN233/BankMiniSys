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

// 删除账号（返回 1：失败 0：成功）
int remove_account(const char *acc_id);

// 释放全部账户链表（程序退出时调用，防止leak）
void free_all_accounts(void);

// 开户
int create_account(const char *name, const char *password, double initial_balance, char *generated_id);

// 生成id
void generate_account_id(char *acc_id, size_t size);

// 销户
int close_account(const char *acc_id);

//全量保存文件 （每次更新账户信息后保存所有账户信息（覆盖））,失败返回-1，成功返回保存文件的个数
int save_all_accounts (const char* filename);

//修改账户基本信息
int update_account_info(const char *acc_id, const char *field, const char *new_value);

//读取文件
int load_accounts(const char *filename);
#endif
