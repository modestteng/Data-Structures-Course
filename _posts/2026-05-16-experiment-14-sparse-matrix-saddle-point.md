---
title: "实验 14 · 稀疏矩阵 · 求鞍点"
date: 2026-05-16 09:14:00 +0800
categories: ["数据结构", "实验"]
tags: ["C", "矩阵", "三元组"]
math: false
pin: false
toc: true
comments: false
---

在三元组表稀疏矩阵上找「鞍点」：该位置既是所在行的最大值，又是所在列的最小值。

> 课程：《数据结构》（涂老师）。本文代码遵循仓库 [`AGENTS.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/AGENTS.md) 与 [`CLAUDE.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/CLAUDE.md) 的约定。

## 源代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 200

typedef struct {
    int i, j, v;
} TriTupleNode;

typedef struct {
    TriTupleNode data[MAX_SIZE];
    int m, n, t;
} TriTupleTable;

// 输入工具
// 反复读取直到拿到合法整数，避免非数字输入死循环
static int read_int(const char *prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) return x;
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("  [!] 不是整数，请重新输入\n");
    }
}

// 三元组表基础操作
static void table_init(TriTupleTable *a, int m, int n) {
    a->m = m;
    a->n = n;
    a->t = 0;
}

static int table_push(TriTupleTable *a, int i, int j, int v) {
    if (a->t >= MAX_SIZE) return 0;
    a->data[a->t].i = i;
    a->data[a->t].j = j;
    a->data[a->t].v = v;
    a->t++;
    return 1;
}

// 两种输入方式
static int input_by_grid(TriTupleTable *a) {
    int m = read_int("  行数 m: ");
    int n = read_int("  列数 n: ");
    if (m <= 0 || n <= 0) { printf("  [!] 行列数必须为正\n"); return 0; }
    table_init(a, m, n);
    printf("  请按行输入 %d 个整数（空格或换行分隔，0 不存入三元组表）：\n", m * n);
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int v;
            if (scanf("%d", &v) != 1) { printf("  [!] 输入中止\n"); return 0; }
            if (v != 0 && !table_push(a, i, j, v)) {
                printf("  [!] 非零元素超过 MAX_SIZE=%d\n", MAX_SIZE);
                return 0;
            }
        }
    }
    return 1;
}

static int input_by_triples(TriTupleTable *a) {
    int m = read_int("  行数 m: ");
    int n = read_int("  列数 n: ");
    if (m <= 0 || n <= 0) { printf("  [!] 行列数必须为正\n"); return 0; }
    table_init(a, m, n);
    printf("  逐项输入：先行 i 再列 j 再值 v；i 输入 0 结束\n");
    printf("  约定：1 <= i <= %d, 1 <= j <= %d，且需按「先行后列」递增顺序录入\n", m, n);
    while (1) {
        int i = read_int("  i (0=结束): ");
        if (i == 0) break;
        int j = read_int("  j: ");
        int v = read_int("  v: ");
        if (i < 1 || i > m || j < 1 || j > n) {
            printf("  [!] 下标越界，已忽略\n");
            continue;
        }
        if (v == 0) {
            printf("  [!] 值为 0 不存入\n");
            continue;
        }
        if (a->t > 0) {
            TriTupleNode *last = &a->data[a->t - 1];
            if (i < last->i || (i == last->i && j <= last->j)) {
                printf("  [!] 顺序不对，应在 (%d,%d) 之后，已忽略\n", last->i, last->j);
                continue;
            }
        }
        if (!table_push(a, i, j, v)) {
            printf("  [!] 容量已满\n");
            break;
        }
    }
    return 1;
}

static int input_matrix(TriTupleTable *a) {
    printf("\n>> 输入矩阵 A\n");
    printf("  1) 按行输入完整矩阵    2) 直接输入非零三元组\n");
    int mode = read_int("  选择输入方式 [1/2]: ");
    if (mode == 1) return input_by_grid(a);
    if (mode == 2) return input_by_triples(a);
    printf("  [!] 无效方式\n");
    return 0;
}

// 显示
static void print_grid(const TriTupleTable *a) {
    int k = 0;
    for (int i = 1; i <= a->m; i++) {
        for (int j = 1; j <= a->n; j++) {
            if (k < a->t && a->data[k].i == i && a->data[k].j == j) {
                printf("%5d", a->data[k].v);
                k++;
            } else {
                printf("%5d", 0);
            }
        }
        printf("\n");
    }
}

static void print_triples(const TriTupleTable *a) {
    printf("  m=%d  n=%d  t=%d\n", a->m, a->n, a->t);
    if (a->t == 0) { printf("  (无非零元素)\n"); return; }
    printf("  序号    i    j    v\n");
    for (int k = 0; k < a->t; k++) {
        printf("  %4d %4d %4d %4d\n",
               k + 1, a->data[k].i, a->data[k].j, a->data[k].v);
    }
}

static void print_matrix(const TriTupleTable *a, const char *title) {
    printf("\n--- %s ---\n", title);
    print_grid(a);
    printf("三元组表：\n");
    print_triples(a);
}

// 求鞍点
// 稀疏矩阵未存位置实际值是 0，必须算入行最小、列最大；隐式 0 也可能成为鞍点
static void find_saddles(const TriTupleTable *a) {
    int m = a->m, n = a->n;
    int *row_min = (int *)malloc((m + 1) * sizeof(int));
    int *col_max = (int *)malloc((n + 1) * sizeof(int));
    int *row_cnt = (int *)calloc(m + 1, sizeof(int));
    int *col_cnt = (int *)calloc(n + 1, sizeof(int));
    if (!row_min || !col_max || !row_cnt || !col_cnt) {
        printf("  [!] 内存分配失败\n");
        free(row_min); free(col_max); free(row_cnt); free(col_cnt);
        return;
    }

    for (int i = 1; i <= m; i++) row_min[i] = INT_MAX;
    for (int j = 1; j <= n; j++) col_max[j] = INT_MIN;

    for (int k = 0; k < a->t; k++) {
        int r = a->data[k].i, c = a->data[k].j, v = a->data[k].v;
        if (v < row_min[r]) row_min[r] = v;
        if (v > col_max[c]) col_max[c] = v;
        row_cnt[r]++;
        col_cnt[c]++;
    }

    // 把隐式 0 纳入：行/列没存满时，若 0 比当前极值更极端就替换
    for (int i = 1; i <= m; i++) {
        if (row_cnt[i] < n && 0 < row_min[i]) row_min[i] = 0;
    }
    for (int j = 1; j <= n; j++) {
        if (col_cnt[j] < m && 0 > col_max[j]) col_max[j] = 0;
    }

    // 中间产物，便于核对算法过程
    printf("\n--- 行最小 row_min ---\n");
    for (int i = 1; i <= m; i++) printf("  row_min[%d] = %d\n", i, row_min[i]);
    printf("--- 列最大 col_max ---\n");
    for (int j = 1; j <= n; j++) printf("  col_max[%d] = %d\n", j, col_max[j]);

    // 三元组按 (i,j) 升序，借一个游标 p 跟随推进即可单趟覆盖所有位置（含隐式 0）
    printf("\n--- 鞍点 ---\n");
    int found = 0;
    int p = 0;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int v;
            if (p < a->t && a->data[p].i == i && a->data[p].j == j) {
                v = a->data[p].v;
                p++;
            } else {
                v = 0;
            }
            if (v == row_min[i] && v == col_max[j]) {
                printf("  鞍点 A[%d][%d] = %d\n", i, j, v);
                found = 1;
            }
        }
    }
    if (!found) printf("  该矩阵不存在鞍点。\n");

    free(row_min); free(col_max); free(row_cnt); free(col_cnt);
}

// 菜单驱动
static void print_status(int has_a, const TriTupleTable *a) {
    printf("\n[状态] A: ");
    if (has_a) printf("%d x %d, %d 项", a->m, a->n, a->t); else printf("未输入");
    printf("\n");
}

static void print_menu(void) {
    printf("--------------------------------------------------\n");
    printf(" 1) 输入矩阵 A\n");
    printf(" 2) 显示矩阵 A\n");
    printf(" 3) 求矩阵 A 的所有鞍点\n");
    printf(" 0) 退出\n");
}

static void print_welcome(void) {
    printf("尊敬的涂老师，你好。\n");
    printf("=== 稀疏矩阵三元组表求鞍点 实验 14 ===\n");
    printf("说明：行列下标从 1 开始；非零元上限 MAX_SIZE=%d；未存位置视为 0\n", MAX_SIZE);
}

static void op_input(TriTupleTable *a, int *has) {
    if (input_matrix(a)) {
        *has = 1;
        print_matrix(a, "矩阵 A 录入完成");
    }
}

static void op_show(int has, const TriTupleTable *a) {
    if (!has) { printf("  [!] 矩阵 A 尚未输入\n"); return; }
    print_matrix(a, "矩阵 A");
}

static void op_saddle(int has, const TriTupleTable *a) {
    if (!has) { printf("  [!] 矩阵 A 尚未输入\n"); return; }
    find_saddles(a);
}

static void run(void) {
    TriTupleTable A;
    int has_a = 0;

    print_welcome();
    while (1) {
        print_status(has_a, &A);
        print_menu();
        int op = read_int("选择: ");
        switch (op) {
            case 1: op_input(&A, &has_a); break;
            case 2: op_show(has_a, &A); break;
            case 3: op_saddle(has_a, &A); break;
            case 0: printf("亲爱的涂老师，再见。\n"); return;
            default: printf("  [!] 无效选项\n");
        }
    }
}

int main(void) {
    run();
    return 0;
}
```

## 运行示例

在 VS Code 打开源文件后按 **Ctrl+Shift+B** 运行项目预置的 `Run current C file` 任务，或在命令行：

```powershell
gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 homework/experiment_14.c -o homework/experiment_14.exe
.\homework\experiment_14.exe
```

