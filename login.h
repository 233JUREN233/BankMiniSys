#ifndef LOGIN_H
#define LOGIN_H

#include "md5.h"
#include "global.h"
#include "account.h"
#include <string.h>
#include <stdio.h>

// md5加密函数
void password_md5(const char *input, char *output);

// 密码比较函数(相同返回1，不同返回0)
int cmp_password(const char *input, const char *password);

// 登录验证
/*
返回值：
0：账号不存在
1：登录成功
2：密码错误
3：账户已锁定
*/
int login(const char *acc_id, const char *pwd);

// 挂失
/*
返回值：
0：账号不存在
1：挂失成功
2：已挂失
3：已冻结
*/
int lost(const char *acc_id);

// 冻结
/*
返回值：
0：账号不存在
1：冻结成功
2：已挂失
3：已冻结
*/
int freeze(const char *acc_id);

// 解冻(挂失和冻结共用)
/*
返回值：
0：账号不存在
1：解冻成功
2：账户已经是正常状态
*/
int unfreeze(const char *acc_id);

#endif
