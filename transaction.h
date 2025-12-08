#include <stdio.h>
#include "global.h"
#include "login.h"

//校验金额是否合法
static int isValidAmount(double amount);
// 合法返回1，否则返回0

// 获取当前时间（格式：YYYY-MM-DD HH:MM）
static void getCurrentTime(char *time_buf, int buf_len);
// 获取的时间格式是年（yyyy）-月（mm）-日（dd） 时（HH）：分（MM）


// 校验交易密码
static int verifyTradePwd(const char *input_pwd, const char *stored_pwd_hash);
/*创建一个能装 33 个字符的数组,因为 MD5 加密后会生成 32 位的十六进制字符串，并且需要
一个额外的字符来存储字符串的结束符 '\0';

用 strcmp 函数（字符串比对函数），把 “用户输入密码的哈希值” 和 “账户存储的哈希值” 对比；
如果一样，返回 1（密码正确）；不一样，返回 0（密码错误）。
*/


// 辅助功能：生成交易记录（插入全局链表）
void createTransactionRecord(const char *acc_id, TransType type, double amount, const char *target_acc);
/*内存分配：用malloc申请Transaction结构体内存，判空避免内存不足导致崩溃；

信息填充：通过->访问指针结构体成员，用strncpy安全复制账号（留\0位），直接赋值交易类型 / 金额，三元运算符处理目标账
号（空则传空串），调用函数获取当前时间；

链表插入：新节点next指向原链表头trans_head，更新trans_head为新节点（新记录插队首），链表适配动态交易数量；

打印输出：三元运算符将枚举类型交易类型转中文，%.2f格式化金额（保留 2 位小数），打印时间、类型、金额、目标账号。
*/