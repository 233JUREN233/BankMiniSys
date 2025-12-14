#ifndef BILL_H
#define BILL_H

#include "global.h"
#include <stdio.h>

// 按账号查询交易记录并展示
void query_transactions(const char *acc_id);

// 重新加载交易记录
void reload_transactions_cache(void);

// 从文件加载交易记录
void load_transactions_from_file();

// 将交易记录保存到文件
void save_transactions_to_file();

// 释放所有交易记录
void free_all_transactions();

#endif
