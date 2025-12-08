#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <conio.h>
#include "global.h"

// 实现无显示读取密码
void get_password(char *pwd, int maxlen);

void print_info(const char *acc_id);

// 清空输入缓冲，防止脏输入残留
void clear_input_buffer(void);

// 安全读取整数
int read_int(const char *prompt);

// 安全读取浮点数
double read_double(const char *prompt);

#endif // RENDER_H
