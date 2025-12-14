#include "global.h"

Account *acc_hash[HASH_SIZE];
Transaction *trans_head = NULL;
char current_login_acc[20];

// 唯一定义全局变量
