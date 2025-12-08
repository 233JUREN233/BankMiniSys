#include "account.h"
#include <string.h>
#include <math.h>

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
    static int counter = 10001;

    if (size >= 20)
    {
        sprintf(acc_id, "%s%08d", ACCOUNT_PREFIX, counter);
        counter++;
    }
}

// 开户(返回0失败，返回1成功)
int create_account(const char *name, const char *password, double initial_balance, char *generated_id)
{
    if (!name || !password || initial_balance < 0)
    {
        return 0;
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

    FILE *fp = fopen(ACCOUNT_FILE, "ab");
    if (!fp)
    {
        free(new_acc);
        return 0;
    }
    if (fwrite(new_acc, sizeof(Account), 1, fp) != 1)
    {
        fclose(fp);
        free(new_acc);
        return 0;
    }
    fclose(fp);

    insert_account(new_acc);

    if (generated_id)
    {
        strcpy(generated_id, new_id);
    }

    return 1;
}

// 销户
int close_account(const char *acc_id)
{
    Account *acc = find_account(acc_id);
    if (acc = NULL)
    {
        printf("账户不存在！");
        return 0;
    }

    if (acc->balance != 0)
    {
        printf("余额不为零，不能销户！");
        return 2;
    
    }

    if (acc->status != ACC_NORMAL)
    {
        printf("账号状态异常！");
        return 3;
    }

    if (remove_account(acc))
    {
        save_all_accounts (ACCOUNT_FILE);
        printf("销户成功！");
        return 1;
    }else printf("移除失败！");
        return 4;


}

//全量保存文件 （每次更新账户信息后保存所有账户信息（覆盖））,失败返回-1，成功返回保存文件的个数
int save_all_accounts (const char* filename)
{
    FILE *fp = fopen(filename, "wb"); 
    if (!fp) {
        printf("错误：无法打开文件 %s\n", filename);
        return -1;
    }

    int count = 0;

    for (int i = 0; i < HASH_SIZE; i++)
    {
        Account *curr = acc_hash[i];
        while (curr != NULL)
        {
            Account temp = *curr;
            temp.next = NULL;
            fwrite(&temp,sizeof(Account),1,fp);
            count++;
            curr = curr->next;
        }
    }
    fclose(fp);

    return count;
}

//查询余额
double get_balance(const char *acc_id)
{
    Account *acc = find_account(acc_id);
    if (acc = NULL)
    {
        printf("未找到账户！");
        return -1;
    }

    double account_balance = acc->balance;
    return account_balance;
}

//读取文件
int load_accounts(const char *filename)
{
    FILE *fp = fopen (filename , "rb");
    if (fp == NULL)
    {
        printf("文件不存在！");
        return 0;
    }

    int count = 0;
    Account temp;
    
    while ((fread(&temp,sizeof(Account),1,fp)) == 1)
    {
        Account *new_acc = (Account*)malloc(sizeof(Account));
        if (new_acc == NULL) {
            printf("内存不足！\n");
            break;
        }

        *new_acc = temp;
        insert_account(new_acc);
        count++;
    }
    fclose(fp);

    return count;
}

//修改账号信息
int update_account_info(const char *acc_id, const char *field, const char *new_value) {
    // 1. 查找账户
    Account *acc = find_account(acc_id);
    if (acc == NULL) {
        printf("错误：账户 %s 不存在\n", acc_id);
        return 0;
    }
    
    // 2. 备份旧值（用于回滚）
    char old_name[30] = "";
    char old_pwd_hash[33] = "";
    double old_balance = acc->balance;
    AccountStatus old_status = acc->status;
    
    strcpy(old_name, acc->name);
    strcpy(old_pwd_hash, acc->pwd_hash);
    
    // 3. 根据字段名修改
    int success = 1;
    
    if (strcmp(field, "name") == 0) {
        // 修改姓名
        if (strlen(new_value) >= sizeof(acc->name)) {
            printf("错误：姓名太长\n");
            return 0;
        }
        strcpy(acc->name, new_value);
        
    } else if (strcmp(field, "password") == 0) {
        // 修改密码（需要加密）
        char encrypted[33];
        password_md5(new_value, encrypted);
        strcpy(acc->pwd_hash, encrypted);
        
    } else if (strcmp(field, "balance") == 0) {
        // 修改余额
        double new_balance = atof(new_value);
        if (new_balance < 0) {
            printf("错误：余额不能为负数\n");
            return 0;
        }
        acc->balance = new_balance;
        
    } else if (strcmp(field, "status") == 0) {
        // 修改状态
        if (strcmp(new_value, "正常") == 0 || strcmp(new_value, "0") == 0) {
            acc->status = ACC_NORMAL;
            acc->login_fail_count = 0;  // 重置登录失败次数
        } else if (strcmp(new_value, "冻结") == 0 || strcmp(new_value, "1") == 0) {
            acc->status = ACC_FROZEN;
        } else if (strcmp(new_value, "挂失") == 0 || strcmp(new_value, "2") == 0) {
            acc->status = ACC_LOST;
        } else {
            printf("错误：无效状态（正常/冻结/挂失）\n");
            return 0;
        }
        
    } else {
        printf("错误：不支持的字段 %s\n", field);
        printf("支持字段：name, password, balance, status\n");
        return 0;
    }
    
    // 4. 保存到文件
    if (save_all_accounts(ACCOUNT_FILE) <= 0) {
        // 保存失败，回滚修改
        printf("错误：保存失败，已恢复原值\n");
        
        if (strcmp(field, "name") == 0) {
            strcpy(acc->name, old_name);
        } else if (strcmp(field, "password") == 0) {
            strcpy(acc->pwd_hash, old_pwd_hash);
        } else if (strcmp(field, "balance") == 0) {
            acc->balance = old_balance;
        } else if (strcmp(field, "status") == 0) {
            acc->status = old_status;
        }
        
        return 0;
    }
    
    printf(" 账户 %s 的 %s 已修改为：%s\n", acc_id, field, new_value);
    return 1;
}