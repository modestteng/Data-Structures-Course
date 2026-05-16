# Claude Code Project Rules

Before making any changes to this project, always read `AGENTS.md` in the project root and in any relevant subdirectory. Follow all rules defined there without exception.

Key rules (from AGENTS.md):
- All code must be written in C only.
- File names and folder names must be in English only (no Chinese characters).
- Code identifiers (variables, functions, structs, macros) must be in English.
- Comments inside source code files may be written in Chinese.
- Comments must use only the `//` line-comment style. Do not use `/* ... */` block comments anywhere in source files (including multi-line notes — use stacked `//` lines instead).
- New files and folders use snake_case (lowercase English, underscores).
- Add concise block-by-block comments (no fixed prefixes like `// Block:`).
- For key functions/code blocks, comments should explain their role in the overall program, not just local behavior.

## 风格要求（写代码时必须满足）

### 交互友好
- 任何要求用户输入的程序，必须是菜单驱动或带明确提示，不能让用户对着空屏幕猜要输什么。
- 每次提示必须说清：要输什么、范围/格式是什么、特殊值（如 0 或 -1）的含义。例：`行 i (输入 0 结束):`。
- 关键操作前显示当前状态（已录入哪些数据、规模是多少），让用户知道自己在哪一步。
- 错误提示要具体：说明哪里错了、应该是什么样，不要只打 "error" 或 "输入错误"。
- 操作完成后给出确认信息或结果展示，不要静默执行。
- 菜单项编号从 1 开始，0 留给"退出"。

### 注释不带 AI 味
- 默认不写注释。只在 *为什么* 不显然时才写：隐藏约束、非平凡不变量、算法选型理由、教材知识点对照。
- 不要解释 *做了什么*，命名应当自解释；变量名 `num_nonzero` 比注释 `// 非零元素个数` 更有用。
- 禁止套话开头："这个函数用于…"、"首先我们…"、"接下来…"、"为了实现…"。
- 禁止三段式排比注释："不仅…而且…还…"、"既…又…也…"。
- **分组标题可以保留，但禁止任何装饰性横线/等号/星号包裹**。错误示例：`// ---------- 输入工具 ----------`、`// ===== 初始化 =====`、`// ********** 显示 **********`、`// --- xxx ---`。正确写法：直接 `// 输入工具` 单独一行放在该组第一个函数上方即可。装饰性横线是 AI 写代码最明显的痕迹之一，必须去掉。
- 一个函数一句话注释为上限，不要写多段式 docstring。算法核心可多写一两行说明思想。
- 注释里不要复述参数和返回值（C 函数签名已经说明）。
- 不要写"使用了 XX 数据结构"这种夸耀式注释，写"为什么用它"才有意义。

### 课程定制
- 本项目是数据结构课程作业，授课老师姓涂。所有交互式程序：
  - **启动欢迎语第一行**统一打印：`尊敬的涂老师，你好。`，再接程序自身的标题与说明。
  - **退出告别语**统一打印：`亲爱的涂老师，再见。`，对应 case 0 / 退出菜单分支。

### 健壮性
- 所有 `scanf` 必须检查返回值，非法输入要清缓冲并提示重输，不能让程序静默接受脏数据或死循环。
- 数组、容器写入前必须检查容量，溢出时给出明确提示并安全退出当前操作（不是 crash）。
- 下标必须做边界检查（特别是 1-indexed 与 0-indexed 混用的场景）。
- 整数溢出敏感的地方（如位置编码 `i*n+j`）显式用 `long` 或更大类型。
- 输入约束（如三元组按行优先序录入）要在录入时校验，不要等算法跑出错才暴露。
- 函数对外暴露的失败情况用返回值表达（0/1 或负错误码），不要只在内部 `printf` 完就 `return`。
- 调用前置条件未满足时（如 A、B 矩阵未输入就要相加），先检查再说话，不要让程序段错误。
- 内部辅助函数加 `static` 限定文件作用域，避免符号污染。
