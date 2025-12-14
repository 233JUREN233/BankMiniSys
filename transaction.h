#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>
#include "global.h"
#include "login.h"
#include "account.h"

// 生成交易记录
void createTransactionRecord(const char *acc_id, TransType type, double amount, const char *target_acc);

// 交易主要功能
int deposit(double amount, const char *trade_pwd);
int withdraw(double amount, const char *trade_pwd);
int transfer(const char *target_acc_id, double amount, const char *trade_pwd);

#endif
