
#include <stdio.h>
#include "global.h"
#include "password.h"

int main()
{
    // 主程序

    // 哈希加密测试
    char pwd[100] = "123456";
    char hash[20];
    simple_hash(pwd, hash);
    printf("原始密码: %s\n加密后: %s\n", pwd, hash);

    return 0;
}

// 这个是该项目的主程序
