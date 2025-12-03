#include <windows.h>
#include <stdio.h>
#include "global.h"
#include "login.h"
#include "md5.h"

int main()
{
    // 主程序
    // part1 选择登录身份
    printf("欢迎登录BankMiniSys!\n");
    printf("请选择你的身份:\n");
    printf("1.用户 2.管理员\n");
    int situation;
    scanf("%d", &situation);
    // 用户身份
    if (situation == 1)
    {
    }
    // 管理员身份
    else if (situation == 2)
    {
    }
    // 不合法退出程序
    else
    {
        printf("请输入合法的数字\n");
        system("pause");
        return 1;
    }
}
