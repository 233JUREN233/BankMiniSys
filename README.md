# BankMiniSys

小型银行账户管理系统

模拟银行核心业务流程，支持账户开户、存取款、转账、账单查询、密码管理等功能。

## 功能模块

- **账户管理模块**：负责开户 / 销户、账户信息修改
- **交易模块**：处理存款、取款、转账
- **账单模块**：生成交易流水记录、按时间 / 类型查询账单、导出账单文件
- **安全模块**：实现密码加密（简易 MD5 / 哈希）、挂失 / 解挂、登录锁定
- **系统管理模块**：管理员功能（账户冻结 / 解冻）、数据备份 / 恢复

---

## 分工

### 成员 1：账户管理模块

- 核心任务：开户（生成账号、录入基本信息）、销户（验证账户状态 / 余额）、账户信息查询（按账号查余额 / 信息）
- 辅助：账户数据的文件读写（仅保留基本存储）

### 成员 2：交易模块

- 核心任务：存款（余额增加）、取款（余额校验 + 扣除）、转账（转出 / 转入账户余额同步）
- 辅助：转账手续费计算（固定比例）

### 成员 3：账单模块

- 核心任务：生成交易记录（存取款 / 转账日志）、按账号查询交易记录
- 辅助：简单的交易记录展示（无需导出功能）

### 成员 4：安全模块

- 核心任务：密码加密（简易哈希）、登录验证（密码校验 + 失败锁定）
- 辅助：账户挂失状态标记（挂失后禁止交易）

### 成员 5：系统管理模块

- 核心任务：账户冻结 / 解冻、数据一键备份（复制数据文件）
- 辅助：系统初始化（启动时加载数据）

---

## 流程图

```mermaid
flowchart TD
  start([启动]) --> init[系统初始化<br/>加载账户/交易数据]
  init --> choice{选择身份}

  choice --> user_login[用户登录]
  choice --> admin_login[管理员登录]

  user_login --> v_user{密码校验 / 冻结校验}
  v_user -->|失败| block_user[拒绝登录 / 锁定] --> choice
  v_user -->|通过| menu_u{用户菜单}

  menu_u --> bal[查询余额]
  menu_u --> dep[存款]
  menu_u --> wd[取款]
  menu_u --> tf[转账]
  menu_u --> hist[查询交易记录]
  menu_u --> lost[挂失/解挂]
  menu_u --> exit_u([退出])
  bal --> menu_u
  dep --> menu_u
  wd --> menu_u
  tf --> menu_u
  hist --> menu_u
  lost --> menu_u

  admin_login --> v_admin{管理员校验}
  v_admin -->|失败| block_admin[拒绝登录] --> choice
  v_admin -->|通过| menu_a{管理员菜单}

  menu_a --> open[开户]
  menu_a --> freeze[冻结/解冻]
  menu_a --> backup[备份数据]
  menu_a --> exit_a([退出])
  open --> menu_a
  freeze --> menu_a
  backup --> menu_a

  exit_u --> save[保存数据并退出]
  exit_a --> save
```

---

```mermaid
flowchart TD
  start_tf([发起交易]) --> v_login{登录状态}
  v_login -->|未登录| deny_login[拒绝]
  v_login -->|已登录| v_state{账户状态正常?}
  v_state -->|冻结/挂失| deny_state[拒绝]
  v_state -->|正常| v_amount{金额合法?}
  v_amount -->|无效| deny_amount[拒绝]
  v_amount -->|有效| calc[计算手续费+合计扣款]
  calc --> v_balance{余额足够?}
  v_balance -->|不足| deny_balance[拒绝]
  v_balance -->|足够| apply[扣款/入账]
  apply --> log[写交易记录]
  log --> ok[完成返回]
```

---

## 编译与运行

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c account.c login.c md5.c render.c system.c transaction.c bill.c global.c -o BankMiniSys.exe
```

## 架构图

```mermaid
flowchart TB
  subgraph UI[界面层]
    MAIN[main.c<br/>菜单/交互]
  end

  subgraph APP[业务层]
    SYS[system.c<br/>启动/持久化]
    LOGIN[login.c<br/>认证/锁定]
    ACC[account.c<br/>账户管理]
    TX[transaction.c<br/>存取/转账]
    BILL[bill.c<br/>流水记录]
    MD5[md5.c<br/>哈希]
    RENDER[render.c<br/>输入/输出样式]
  end

  subgraph DATA[数据层]
    STORAGE[文件存储 / 备份]
  end

  MAIN --> SYS
  MAIN --> LOGIN
  MAIN --> ACC
  MAIN --> TX
  MAIN --> BILL

  SYS --> ACC
  SYS --> TX
  SYS --> BILL

  LOGIN --> ACC
  ACC --> TX
  TX --> BILL

  ACC --> STORAGE
  TX --> STORAGE
  BILL --> STORAGE
  SYS --> STORAGE

  LOGIN --> MD5
  MAIN --> RENDER
```
