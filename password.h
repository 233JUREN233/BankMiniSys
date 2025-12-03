#include "md5.h"
#include <string.h>
#include <stdio.h>

// md5加密函数
void password_md5(const char *input, char *output)
{
	MD5_CTX ctx;
	unsigned char digest[16];
	MD5Init(&ctx); // 初始化MD5计算上下文
	MD5Update(&ctx, (unsigned char *)input, strlen(input));
	MD5Final(&ctx, digest); // 结束MD5计算
	for (int i = 0; i < 16; i++)
		sprintf(output + i * 2, "%02x", digest[i]);
	output[32] = '\0';
}

// 密码比较函数(相同返回1，不同返回0)
int cmp_password(const char *input, const char *password)
{
	char input_md5[33];
	password_md5(input, input_md5);
	if (strcmp(input_md5, password) == 0)
		return 1;
	else
		return 0;
}

// 密码加密实现
