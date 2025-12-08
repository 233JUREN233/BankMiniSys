#include "render.h"
#include "account.h"

void get_password(char *pwd, int maxlen)
{
    int i = 0;
    char ch;
    while (i < maxlen - 1)
    {
        ch = _getch(); // 输入但不显示
        if (ch == '\r' || ch == '\n')
        {
            // 回车结束qq
            break;
        }
        else if (ch == '\b' && i > 0)
        { // 处理退格
            i--;
            printf("\b \b");
        }
        else if (ch >= 32 && ch <= 126)
        { // 可见字符
            pwd[i++] = ch;
            printf("*");
        }
    }
    pwd[i] = '\0';
    printf("\n");
}

void print_info(const char *acc_id)
{
    Account *acc = find_account(acc_id);
    printf("尊敬的用户：%s\n", acc->acc_id);
    printf("您当前的账户余额为%lf\n", acc->balance);
}
