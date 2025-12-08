#ifndef BILL_H
#define BILL_H

#include "global.h"
#include <stdio.h>

// 按账号查询交易记录并展示
void query_transactions(const char *acc_id);

// 从文件加载交易记录（系统初始化时调用）
void load_transactions_from_file();

// 将交易记录保存到文件（程序退出时调用）
void save_transactions_to_file();

// 释放所有交易记录（防止内存泄漏）
void free_all_transactions();

#endif
