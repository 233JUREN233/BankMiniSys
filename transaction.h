#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>
#include "global.h"
#include "login.h"
#include "account.h"

// 辅助功能：生成交易记录（插入全局链表）
void createTransactionRecord(const char *acc_id, TransType type, double amount, const char *target_acc);

// 交易主功能
int deposit(double amount, const char *trade_pwd);
int withdraw(double amount, const char *trade_pwd);
int transfer(const char *target_acc_id, double amount, const char *trade_pwd);

#endif // TRANSACTION_H
