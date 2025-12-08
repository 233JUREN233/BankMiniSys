#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "global.h"
#include "login.h"
#include "md5.h"
#include "render.h"

// 主程序
int main()

{
    // initialization
    srand((unsigned int)time(NULL));

    // part1 选择登录身份
    printf("欢迎登录BankMiniSys!\n");
    printf("请选择你的身份:\n");
    printf("1.用户 2.管理员\n");
    int situation;
    scanf("%d", &situation);
    // 用户身份
    if (situation == 1)
    {
        char id[20], pwd[33];
        while (1)
        {
            printf("请输入你的用户名：\n");
            scanf("%s", id);
            int login_result;
            do
            {
                printf("请输入密码:\n");
                get_password(pwd, 33);
                login_result = login(id, pwd);
                if (login_result == 0)
                {
                    printf("账号不存在，请再试一次！\n");
                    break;
                }
                else if (login_result == 2)
                {
                    printf("密码错误，请再试一次！\n");
                }
                else if (login_result == 3)
                {
                    printf("账户已被锁定，请联系管理员进行解锁\n");
                    return 0;
                }
            } while (login_result != 1);

            if (login_result == 1)
            {
                printf("登陆成功！\n");
                // 将已登录账号保存到全局变量，以便交易模块等使用
                strncpy(current_login_acc, id, sizeof(current_login_acc) - 1);
                current_login_acc[sizeof(current_login_acc) - 1] = '\0';
                break;
            }
        }
        // 进入用户界面
        Sleep(500);
        putchar('\n');
        printf("欢迎登陆，用户%s", id);
    }

    // 管理员身份登录
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
