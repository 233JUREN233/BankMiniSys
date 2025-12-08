#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "login.h"
#include "account.h"

// 账号递增计数器与计数器文件
static int g_account_counter = 10001;
static const char *ACCOUNT_COUNTER_FILE = "account_counter.dat";

// 磁盘存储结构
typedef struct DiskAccount
{
    char acc_id[20];
    char name[30];
    char pwd_hash[33];
    double balance;
    AccountStatus status;
    int login_fail_count;
} DiskAccount;

// 从文件加载计数器；若不存在返回 -1
static int load_counter_from_file(void)
{
    FILE *fp = fopen(ACCOUNT_COUNTER_FILE, "rb");
    if (!fp)
        return -1;
    int val = -1;
    if (fread(&val, sizeof(int), 1, fp) != 1)
        val = -1;
    fclose(fp);
    return val;
}

// 将计数器写入文件
static void save_counter_to_file(int counter)
{
    FILE *fp = fopen(ACCOUNT_COUNTER_FILE, "wb");
    if (!fp)
        return;
    fwrite(&counter, sizeof(int), 1, fp);
    fclose(fp);
}

// 计算哈希值
int hash_acc_id(const char *acc_id)
{
    unsigned int hash = 0;

    for (int i = 0; acc_id[i] != '\0'; i++)
    {
        hash = hash * 131 + (unsigned char)acc_id[i];
    }

    return hash % HASH_SIZE;
}

// 按哈希值插入账户节点
void insert_account(Account *acc)
{
    int idx = hash_acc_id(acc->acc_id);
    acc->next = acc_hash[idx];
    acc_hash[idx] = acc;
}

// 按账号查找账户
Account *find_account(const char *acc_id)
{
    int idx = hash_acc_id(acc_id);
    Account *curr = acc_hash[idx];

    while (curr)
    {
        if (strcmp(curr->acc_id, acc_id) == 0)
        {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

// 删除账号
int remove_account(const char *acc_id)
{
    int idx = hash_acc_id(acc_id);
    Account *curr = acc_hash[idx], *prev = NULL;

    while (curr)
    {
        if (strcmp(curr->acc_id, acc_id) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                acc_hash[idx] = curr->next;
            free(curr);
            return 0; // 成功返回0
        }
        prev = curr;
        curr = curr->next;
    }
    return 1; // 失败返回1
}

// 释放全部账户链表
void free_all_accounts(void)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Account *curr = acc_hash[i];
        while (curr)
        {
            Account *next = curr->next;
            free(curr);
            curr = next;
        }
        acc_hash[i] = NULL;
    }
}

// 生成id
void generate_account_id(char *acc_id, size_t size)
{
    if (size >= 20)
    {
        sprintf(acc_id, "%s%08d", ACCOUNT_PREFIX, g_account_counter);
        g_account_counter++;
    }
}

// 开户(返回0失败，返回1成功)
int create_account(const char *name, const char *password, double initial_balance, char *generated_id)
{
    if (!name || !password || initial_balance < 0)
    {
        return 0;
    }

    int persisted = load_counter_from_file();
    if (persisted > g_account_counter)
    {
        g_account_counter = persisted;
    }

    char new_id[20];
    generate_account_id(new_id, sizeof(new_id));

    if (find_account(new_id) != NULL)
    {
        return 0;
    }

    Account *new_acc = malloc(sizeof(Account));
    if (!new_acc)
    {
        return 0;
    }

    strcpy(new_acc->acc_id, new_id);
    strcpy(new_acc->name, name);
    password_md5(password, new_acc->pwd_hash);

    new_acc->balance = initial_balance;
    new_acc->status = ACC_NORMAL;
    new_acc->login_fail_count = 0;
    new_acc->next = NULL;

    // 写入磁盘
    DiskAccount da;
    memcpy(da.acc_id, new_acc->acc_id, sizeof(da.acc_id));
    memcpy(da.name, new_acc->name, sizeof(da.name));
    memcpy(da.pwd_hash, new_acc->pwd_hash, sizeof(da.pwd_hash));
    da.balance = new_acc->balance;
    da.status = new_acc->status;
    da.login_fail_count = new_acc->login_fail_count;

    FILE *fp = fopen(ACCOUNT_FILE, "ab");
    if (!fp)
    {
        free(new_acc);
        return 0;
    }
    if (fwrite(&da, sizeof(DiskAccount), 1, fp) != 1)
    {
        fclose(fp);
        free(new_acc);
        return 0;
    }
    fclose(fp);

    insert_account(new_acc);

    save_counter_to_file(g_account_counter);

    if (generated_id)
    {
        strcpy(generated_id, new_id);
    }

    return 1;
}

// 销户
int close_account(const char *acc_id)
{
    int ret = remove_account(acc_id);
    if (ret == 0)
    {
        save_accounts();
    }
    return ret;
}

// 启动时加载账户
int load_accounts(void)
{
    FILE *fp = fopen(ACCOUNT_FILE, "rb");
    if (!fp)
    {
        int persisted = load_counter_from_file();
        if (persisted > 0)
        {
            g_account_counter = persisted;
        }
        return 1; // 没有文件视作成功加载0个
    }

    DiskAccount da;
    while (fread(&da, sizeof(DiskAccount), 1, fp) == 1)
    {
        Account *acc = malloc(sizeof(Account));
        if (!acc)
        {
            fclose(fp);
            return 0;
        }
        memcpy(acc->acc_id, da.acc_id, sizeof(acc->acc_id));
        memcpy(acc->name, da.name, sizeof(acc->name));
        memcpy(acc->pwd_hash, da.pwd_hash, sizeof(acc->pwd_hash));
        acc->balance = da.balance;
        acc->status = da.status;
        acc->login_fail_count = da.login_fail_count;
        acc->next = NULL;
        insert_account(acc);

        if (strncmp(acc->acc_id, ACCOUNT_PREFIX, strlen(ACCOUNT_PREFIX)) == 0)
        {
            const char *suffix = acc->acc_id + strlen(ACCOUNT_PREFIX);
            int num = atoi(suffix);
            if (num >= g_account_counter)
            {
                g_account_counter = num + 1;
            }
        }
    }
    fclose(fp);

    int persisted = load_counter_from_file();
    if (persisted > g_account_counter)
    {
        g_account_counter = persisted;
    }
    else
    {
        // 更新持久化文件，保证计数与现有账号最大值一致
        save_counter_to_file(g_account_counter);
    }
    return 1;
}

// 保存所有账户到文件
int save_accounts(void)
{
    FILE *fp = fopen(ACCOUNT_FILE, "wb");
    if (!fp)
    {
        return 0;
    }

    for (int i = 0; i < HASH_SIZE; i++)
    {
        Account *curr = acc_hash[i];
        while (curr)
        {
            DiskAccount da;
            memcpy(da.acc_id, curr->acc_id, sizeof(da.acc_id));
            memcpy(da.name, curr->name, sizeof(da.name));
            memcpy(da.pwd_hash, curr->pwd_hash, sizeof(da.pwd_hash));
            da.balance = curr->balance;
            da.status = curr->status;
            da.login_fail_count = curr->login_fail_count;
            fwrite(&da, sizeof(DiskAccount), 1, fp);
            curr = curr->next;
        }
    }

    fclose(fp);

    // 也持久化计数器，防止倒退
    save_counter_to_file(g_account_counter);

    return 1;
}
