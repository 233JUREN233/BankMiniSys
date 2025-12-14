#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <conio.h>
#include "global.h"

// 清空输入缓存
void clear_input_buffer(void);

// 安全读取整数/浮点/文本
int read_int(const char *prompt);
double read_double(const char *prompt);
void read_line(char *buf, size_t size, const char *prompt);

// 实现无显示读取密码
void get_password(char *pwd, int maxlen);

void print_info(const char *acc_id);

#endif
