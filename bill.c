#include <string.h>
#include <stdlib.h>
#include "bill.h"
#include "transaction.h"

// 按账号查询交易记录并展示
void query_transactions(const char *acc_id)
{
    if (!acc_id || strlen(acc_id) == 0)
    {
        printf("查询失败：账号不能为空！\n");
        return;
    }

    Account *acc = find_account(acc_id);
    if (!acc)
    {
        printf("查询失败：账号不存在！\n");
        return;
    }

    Transaction *curr = trans_head;
    int count = 0;
    printf("\n===== 账号 %s 的交易记录 =====", acc_id);

    if (!curr)
    {
        printf("\n暂无交易记录\n");
        return;
    }

    while (curr)
    {
        if (strcmp(curr->acc_id, acc_id) == 0)
        {
            count++;
            const char *type_str;
            switch (curr->type)
            {
            case TRANS_DEPOSIT:
                type_str = "存款";
                break;
            case TRANS_WITHDRAW:
                type_str = "取款";
                break;
            case TRANS_TRANSFER:
                type_str = (strcmp(curr->target_acc, "") == 0) ? "转账入账" : "转账出账";
                break;
            default:
                type_str = "未知交易";
            }

            printf("\n%d. 时间：%s | 类型：%s | 金额：%.2f | ",
                   count, curr->time, type_str, curr->amount);

            if (curr->type == TRANS_TRANSFER)
            {
                printf("对方账号：%s", curr->target_acc);
            }
        }
        curr = curr->next;
    }

    if (count == 0)
    {
        printf("\n暂无交易记录\n");
    }
    else
    {
        printf("\n===== 共 %d 条记录 =====", count);
    }
    printf("\n\n");
}

// 从文件加载交易记录
void load_transactions_from_file()
{
    FILE *fp = fopen(TRANS_FILE, "rb");
    if (!fp)
    {
        printf("交易记录文件不存在，将创建新文件\n");
        return;
    }

    Transaction *temp = NULL;
    while (1)
    {
        Transaction *trans = (Transaction *)malloc(sizeof(Transaction));
        if (!trans)
        {
            printf("内存分配失败，加载交易记录中断\n");
            break;
        }

        size_t read_size = fread(trans, sizeof(Transaction), 1, fp);
        if (read_size != 1)
        {
            free(trans);
            break;
        }

        trans->next = NULL;
        if (!temp)
        {
            trans_head = trans;
        }
        else
        {
            temp->next = trans;
        }
        temp = trans;
    }

    fclose(fp);
    printf("交易记录加载完成\n");
}

// 将交易记录保存到文件
void save_transactions_to_file()
{
    FILE *fp = fopen(TRANS_FILE, "wb");
    if (!fp)
    {
        printf("保存交易记录失败：无法打开文件\n");
        return;
    }

    Transaction *curr = trans_head;
    while (curr)
    {
        fwrite(curr, sizeof(Transaction), 1, fp);
        curr = curr->next;
    }

    fclose(fp);
    printf("交易记录已保存\n");
}

// 释放所有交易记录
void free_all_transactions()
{
    Transaction *curr = trans_head;
    while (curr)
    {
        Transaction *next = curr->next;
        free(curr);
        curr = next;
    }
    trans_head = NULL;
}
