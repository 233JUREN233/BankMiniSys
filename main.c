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
#include "bill.h"
#include "transaction.h"
#include "account.h"
#include "system.h"







// 主程序
int main()

{
    // 系统初始化
    srand((unsigned int)time(NULL));
    system_init();

    // 选择登录身份
    printf("欢迎登录BankMiniSys!\n");
    printf("请选择你的身份:\n");
    printf("1.用户 2.管理员\n");
    int situation = read_int("请选择：");

    // 用户身份登录
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
                strncpy(current_login_acc, id, sizeof(current_login_acc) - 1);
                current_login_acc[sizeof(current_login_acc) - 1] = '\0';
                break;
            }
        }

        // 用户功能菜单
        while (1)
        {
            printf("\n用户菜单：\n");
            printf("1. 查询余额\n");
            printf("2. 存款\n");
            printf("3. 取款\n");
            printf("4. 转账\n");
            printf("5. 查询交易记录\n");
            printf("6. 挂失\n");
            printf("7. 解冻/解挂\n");
            printf("0. 退出\n");
            int choice = read_int("请选择：");

            if (choice == 0)
            {
                current_login_acc[0] = '\0';
                break;
            }

            if (choice == 1)
            {
                Account *acc = find_account(current_login_acc);
                if (acc)
                    printf("当前余额：%.2f\n", acc->balance);
                else
                    printf("账户不存在\n");
            }
            else if (choice == 2)
            {
                double amt = read_double("请输入存款金额：");
                deposit(amt, NULL);
            }
            else if (choice == 3)
            {
                double amt = read_double("请输入取款金额：");
                withdraw(amt, NULL);
            }
            else if (choice == 4)
            {
                char target[20];
                printf("转入账号：");
                scanf("%s", target);
                double amt = read_double("金额：");
                transfer(target, amt, NULL);
            }
            else if (choice == 5)
            {
                query_transactions(current_login_acc);
            }
            else if (choice == 6)
            {
                if (lost(current_login_acc) == 1)
                {
                    printf("挂失成功\n");
                }
                else
                    printf("挂失失败或状态异常\n");
            }
            else if (choice == 7)
            {
                if (unfreeze(current_login_acc) == 1)
                {
                    printf("已解冻/解挂\n");
                }
                else
                    printf("解冻失败或已是正常状态\n");
            }
            else
            {
                printf("无效选项，请重试。\n");
            }
        }
    }
    // 管理员身份
    else if (situation == 2)
    {
        // 管理员认证
        const char *ADMIN_USER = "admin";
        const char *ADMIN_PWD = "admin123";
        char admin_user[32];
        char admin_pwd[64];
        int authed = 0;
        for (int attempt = 1; attempt <= 3; attempt++)
        {
            printf("请输入管理员账号：");
            scanf("%31s", admin_user);
            printf("请输入管理员密码：\n");
            get_password(admin_pwd, sizeof(admin_pwd));
            if (strcmp(admin_user, ADMIN_USER) == 0 && strcmp(admin_pwd, ADMIN_PWD) == 0)
            {
                authed = 1;
                break;
            }
            printf("账号或密码错误，剩余尝试次数：%d\n", 3 - attempt);
        }
        if (!authed)
        {
            printf("管理员认证失败，程序结束。\n");
            return 0;
        }

        // 管理员菜单
        while (1)
        {
            printf("\n管理员菜单：\n");
            printf("1. 开户\n");
            printf("2. 冻结账户\n");
            printf("3. 解冻账户\n");
            printf("4. 挂失账户\n");
            printf("5. 备份数据\n");
            printf("0. 退出\n");
            int choice = read_int("请选择：");
            if (choice == 0)
                break;
            if (choice == 1)
            {
                char name[30], pwd[40], new_id[20];
                printf("姓名：");
                scanf("%s", name);
                printf("密码：");
                scanf("%s", pwd);
                double init = read_double("初始余额：");
                if (create_account(name, pwd, init, new_id))
                {
                    printf("开户成功，账号：%s\n", new_id);
                }
                else
                {
                    printf("开户失败\n");
                }
            }
            else if (choice == 2)
            {
                char acc[20];
                printf("账号：");
                scanf("%s", acc);
                if (freeze(acc) == 1)
                {
                    printf("冻结成功\n");
                }
                else
                    printf("冻结失败或状态异常\n");
            }
            else if (choice == 3)
            {
                char acc[20];
                printf("账号：");
                scanf("%s", acc);
                if (unfreeze(acc) == 1)
                {
                    printf("解冻成功\n");
                }
                else
                    printf("解冻失败或状态异常\n");
            }
            else if (choice == 4)
            {
                char acc[20];
                printf("账号：");
                scanf("%s", acc);
                if (lost(acc) == 1)
                {
                    printf("挂失成功\n");
                }
                else
                    printf("挂失失败或状态异常\n");
            }
            else if (choice == 5)
            {
                backup_data();
            }
            else
            {
                printf("无效选项，请重试。\n");
            }
        }
    }
    else
    {
        printf("请输入合法的数字\n");
        system("pause");
        return 1;
    }

    // 保存账户和交易记录
    system_save();

    // 释放内存
    free_all_transactions();
    free_all_accounts();
}
