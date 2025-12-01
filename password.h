
// 密码加密函数声明
void simple_hash(const char *input, char *output);

// 密码加密函数实现
#include <stdio.h>
void simple_hash(const char *input, char *output)
{
	unsigned int hash = 5381;
	int c;
	while ((c = *input++))
		hash = ((hash << 5) + hash) + c;
	sprintf(output, "%08x", hash);
}
