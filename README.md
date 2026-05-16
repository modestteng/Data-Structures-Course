# 数据结构课程实验仓库

**Data Structures Course Lab Repository**

> 本仓库是涂老师《数据结构》课程的个人作业合集，按周完成的实验代码、教材章节练习与学习笔记。  
> A personal coursework repository for 涂老师's *Data Structures* course — weekly C labs, chapter exercises, and study notes.

---

## 项目概览 / Overview

- **课程 / Course**：数据结构 (Data Structures)
- **语言 / Language**：纯 C (C only — 强制要求，见 [AGENTS.md](AGENTS.md))
- **范围 / Scope**：15 个交互式实验程序 + 教材章节跟读代码 + 学习笔记

---

## 目录结构 / Directory Layout

```
Data-Structures-Course/
├── homework/                       # 每周实验 / weekly experiments
│   ├── experiment_1.c ~ experiment_16.c     # 单文件实验
│   ├── experiment_12/              # 多文件实验：广义表算法库
│   │   ├── main.c
│   │   ├── glist.c
│   │   └── glist.h
│   ├── experiment_13_test.c        # 实验 13 自动化测试程序
│   ├── jobs.txt                    # 实验 8 测试数据
│   └── *.exe                       # 编译产物 / build artifacts
├── linear list/                    # 线性表练习 / linear-list practice
├── Stacks and queues/              # 栈与队列练习 / stack & queue practice
├── chapter3/                       # 串/数组/广义表 基础练习 / string·array·glist
├── tree/                           # 二叉树遍历练习 / binary-tree traversals
├── data.txt                        # 实验 7 队列输入样本
├── notes.md                        # 学习笔记 / study notes
├── Summary of each chapter.md      # 章节小结 / chapter summaries
├── AGENTS.md                       # 项目硬规则 / project rules
├── CLAUDE.md                       # 风格与交互要求 / style guide
└── .vscode/tasks.json              # 一键编译运行任务 / one-click build task
```

> 实验 15 暂未在仓库中（跳号）。Experiment 15 is intentionally absent.

---

## 实验清单 / Experiment Index

| #  | 主题 / Topic                                | 数据结构 / Structure          | 要点 / Highlights                                                              |
|----|---------------------------------------------|-------------------------------|-------------------------------------------------------------------------------|
| 1  | 顺序表 · 图书管理 / Sequential list         | `SqList<Book>`                | 顺序表基础 CRUD、按位置/书号查找                                              |
| 2  | 顺序表 · 菜单系统 / Sequential list menu    | `SqList<Book>`                | 统一菜单入口、输入校验、容量检查                                              |
| 3  | 单链表 · 图书 / Singly linked list          | 带头/尾指针的链表             | 头插/尾插、节点遍历                                                           |
| 4  | 大数链表 / Big-integer linked list          | 每个结点存一位                | 字符串→链表、链表→数值                                                        |
| 5  | 链栈 · 字符 / Linked stack                  | `LinkStack<char>`             | 无容量上限、动态分配/释放                                                     |
| 6  | 中缀转后缀 + 求值 / Infix → postfix         | 双栈（数字 + 运算符）         | 运算符优先级、表达式求值                                                      |
| 7  | 链队列 · 医院叫号 / Hospital queue sim      | `LinkQueue<Patient>`          | 到达/处理时间、FIFO 模拟                                                      |
| 8  | 时间片轮转调度 / Round-robin scheduling     | `LinkQueue<Job>` + quantum    | 剩余时间、平均周转/等待                                                       |
| 9  | 字符串加解密 / String encrypt · decrypt     | 字母映射表                    | 加密表反推、查表 O(1) 解密                                                    |
| 10 | 字符串操作系统 / String operations          | 串头表 (并行数组)             | 命令解析、多串管理                                                            |
| 11 | 稀疏矩阵 (三元组表) / Sparse matrix         | `TriTupleTable`               | 行优先录入校验、整数溢出用 `long` 显式处理                                    |
| 12 | 广义表算法库 / Generalized list (multi-file)| `GList` (ATOM/LIST 联合)      | 多文件项目（`glist.h`）、递归解析嵌套括号                                     |
| 13 | 广义表 · 复制/长度/深度 / Generalized list  | 单文件递归实现                | 配套 [experiment_13_test.c](homework/experiment_13_test.c) 自动化测试         |
| 14 | 稀疏矩阵求鞍点 / Saddle-point on triplet    | `TriTupleTable`               | 行 max + 列 min 同时落点的位置                                                |
| 16 | 二叉树基本操作 / Binary tree basics         | 二叉链表 + 顺序栈 + 链队列    | 先/中/后序、层序、最大宽度（队列附 level 字段）                               |

---

## 如何编译运行 / How to Build & Run

### 工具链 / Toolchain
- **MinGW-w64 gcc 14.2.0**，路径见 [`.vscode/tasks.json`](.vscode/tasks.json)：  
  `C:\mingw_x64_14_2_0\mingw64\bin\gcc.exe`
- 控制台代码页：UTF-8（任务里会自动 `chcp 65001`），保证中文菜单不乱码。

### 推荐方式 / Recommended (VS Code)
打开任意 `homework/experiment_*.c`，按 **Ctrl+Shift+B** 触发 `Run current C file` 任务。该任务会：

1. 检查当前文件所在目录有没有 `*.h`；  
2. 若有（如 `experiment_12/`）→ 把整目录的 `.c` 一起编译为 `<dir>.exe`；  
3. 若无 → 只编译当前文件为 `<file>.exe`；  
4. 编译参数固定为 `-finput-charset=UTF-8 -fexec-charset=UTF-8`；  
5. 编译成功后自动运行。

### 命令行方式 / CLI fallback

单文件实验 / single-file experiment：

```powershell
gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 `
    homework\experiment_1.c `
    -o homework\experiment_1.exe
.\homework\experiment_1.exe
```

多文件实验 (实验 12) / multi-file experiment：

```powershell
gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 `
    homework\experiment_12\*.c `
    -o homework\experiment_12\experiment_12.exe
.\homework\experiment_12\experiment_12.exe
```

---

## 代码与交互规范 / Code & UX Conventions

详细规则见 [AGENTS.md](AGENTS.md) 与 [CLAUDE.md](CLAUDE.md)；以下是核心要点：

### 代码 / Code
- 全 C，文件名/标识符使用 **英文 snake_case**；注释允许中文。  
  All C; filenames and identifiers in English snake_case; comments may be Chinese.
- **只用 `//`** 行注释，禁止 `/* */` 块注释，禁止装饰性横线 (`====` / `----` / `****`)。  
  Line comments only; no block comments; no decorative banner lines.
- 一句话注释为上限；解释 **为什么**，不复述 **做了什么**。  
  One-sentence comment max; explain *why*, not *what*.

### 交互 / Interaction
- 所有交互程序统一开场白：`尊敬的涂老师，你好。`，统一结束语：`亲爱的涂老师，再见。`
- 菜单驱动，编号从 1 开始，`0` 留给退出。  
  Menu-driven; options start at 1, `0` reserved for exit.
- 每个输入提示必须说明：要输什么、范围、特殊值含义（如 `输入 0 结束`）。  
  Every prompt must state: what to enter, range, sentinel value meaning.

### 健壮性 / Robustness
- `scanf` 必须检查返回值；非法输入清缓冲并提示重输。  
  Always check `scanf` return value; flush stdin and re-prompt on invalid input.
- 数组写入前检查容量；下标做边界检查（注意 1-indexed / 0-indexed 混用）。  
  Bounds-check before writes; mind 1-indexed vs 0-indexed.
- 位置编码等大整数运算（如 `i*n+j`）显式用 `long`，防溢出。  
  Use `long` for index encoding to prevent overflow.

---

## 学习资料 / Study Notes

- [`notes.md`](notes.md) — 持续更新的学习日志（复杂度分析、数据结构设计原则等）  
  Ongoing study journal — complexity analysis, design principles.
- [`Summary of each chapter.md`](Summary%20of%20each%20chapter.md) — 各章小结 / chapter summaries.
- 章节练习文件夹（[`linear list/`](linear%20list/)、[`Stacks and queues/`](Stacks%20and%20queues/)、[`chapter3/`](chapter3/)、[`tree/`](tree/)）：教材跟读代码。  
  Per-chapter folders contain textbook follow-along code (kept around for reference).

---

## 博客 / Blog

本仓库同时是一个 Jekyll + [Chirpy](https://github.com/cotes2020/jekyll-theme-chirpy) 主题的中文技术博客，由 GitHub Actions 自动构建并部署到 GitHub Pages。  
This repo also doubles as a Jekyll + Chirpy blog, built and deployed via GitHub Actions.

- **线上地址 / Live site**：<https://modestteng.github.io/Data-Structures-Course/>
- **构建流程 / Workflow**：[`.github/workflows/pages-deploy.yml`](.github/workflows/pages-deploy.yml) — `push` 到 `main` 自动触发。
- **博文生成 / Post generation**：每篇实验博文由 [`tools/generate_posts.py`](tools/generate_posts.py) 从 `homework/experiment_*.c` 自动生成到 [`_posts/`](_posts/)；脚本幂等，重跑会覆盖旧文。

### 首次启用 GitHub Pages

在仓库 **Settings → Pages → Source** 选择 **GitHub Actions** 后，把当前改动 push 到 `main`，等待 Actions 跑完即可访问上面的线上地址。

### 本地预览 / Local preview

需 Ruby 3.x：

```powershell
gem install bundler
bundle install
bundle exec jekyll serve --livereload
# 浏览器访问 http://127.0.0.1:4000/Data-Structures-Course/
```

### 新增 / 修改实验后

```powershell
python tools/generate_posts.py    # 重新生成所有实验博文
git add _posts/ homework/
git commit -m "新增/更新实验"
git push                          # Actions 自动重新部署
```

---

## 作者与用途 / Author & Usage

个人课程作业仓库，仅用于学习交流，不接受外部 PR。  
Personal coursework repository for study use only; external contributions are not accepted.
