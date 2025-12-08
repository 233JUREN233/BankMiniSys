#include "global.h"

// 全局变量唯一定义
Account *acc_hash[HASH_SIZE];
Transaction *trans_head = NULL;
char current_login_acc[20];
