#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "global.h"

void init_account_systems();//初始系统

int create_account(const char *name, const char *password, double initial_balnace, char *generate_id);//创建账户 

int close_account(const char *id);//销户

double query_balance(const char *acc_id);//查询余额



#endif