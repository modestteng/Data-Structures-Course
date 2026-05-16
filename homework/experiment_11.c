#include <stdio.h>
#include <stdlib.h>

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

static int input_matrix(TriTupleTable *a, char name) {
    printf("\n>> 输入矩阵 %c\n", name);
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



// 按行序快速转置：O(m*n)
// 朴素列序算法每列都要扫一遍三元组表，最坏 O(m*n^2)
// 这里先用 num 统计每列非零元个数，cpot 推出每列在结果中的起始下标，
// 再扫一遍原表即可让每个非零元一次到位
static void transpose_fast(const TriTupleTable *a, TriTupleTable *b) {
    int num[MAX_SIZE];
    int cpot[MAX_SIZE];

    b->m = a->n;
    b->n = a->m;
    b->t = a->t;
    if (a->t == 0) return;

    for (int c = 1; c <= a->n; c++) num[c] = 0;
    for (int p = 0; p < a->t; p++) num[a->data[p].j]++;

    cpot[1] = 0;
    for (int c = 2; c <= a->n; c++) cpot[c] = cpot[c - 1] + num[c - 1];

    for (int p = 0; p < a->t; p++) {
        int col = a->data[p].j;
        int q = cpot[col]++;
        b->data[q].i = a->data[p].j;
        b->data[q].j = a->data[p].i;
        b->data[q].v = a->data[p].v;
    }
}

// 两稀疏矩阵相加：利用三元组表已按 (行,列) 字典序有序的性质做归并
// 同位置求和，和为 0 不保留
static int add_matrix(const TriTupleTable *a, const TriTupleTable *b, TriTupleTable *c) {
    if (a->m != b->m || a->n != b->n) return 0;
    table_init(c, a->m, a->n);

    int p = 0, q = 0;
    while (p < a->t && q < b->t) {
        long ka = (long)a->data[p].i * (a->n + 1) + a->data[p].j;
        long kb = (long)b->data[q].i * (b->n + 1) + b->data[q].j;
        if (ka < kb) {
            table_push(c, a->data[p].i, a->data[p].j, a->data[p].v);
            p++;
        } else if (ka > kb) {
            table_push(c, b->data[q].i, b->data[q].j, b->data[q].v);
            q++;
        } else {
            int s = a->data[p].v + b->data[q].v;
            if (s != 0) table_push(c, a->data[p].i, a->data[p].j, s);
            p++;
            q++;
        }
    }
    while (p < a->t) {
        table_push(c, a->data[p].i, a->data[p].j, a->data[p].v);
        p++;
    }
    while (q < b->t) {
        table_push(c, b->data[q].i, b->data[q].j, b->data[q].v);
        q++;
    }
    return 1;
}



// 菜单驱动
static void print_status(int has_a, int has_b, const TriTupleTable *a, const TriTupleTable *b) {
    printf("\n[状态] A: ");
    if (has_a) printf("%d x %d, %d 项", a->m, a->n, a->t); else printf("未输入");
    printf("    B: ");
    if (has_b) printf("%d x %d, %d 项", b->m, b->n, b->t); else printf("未输入");
    printf("\n");
}

static void print_menu(void) {
    printf("--------------------------------------------------\n");
    printf(" 1) 输入矩阵 A           2) 输入矩阵 B\n");
    printf(" 3) 显示矩阵 A           4) 显示矩阵 B\n");
    printf(" 5) A 按行序快速转置  (O(m*n))\n");
    printf(" 6) 计算 A + B\n");
    printf(" 0) 退出\n");
}

static void print_welcome(void) {
    printf("尊敬的涂老师，你好。\n");
    printf("=== 稀疏矩阵三元组表 实验 11 ===\n");
    printf("说明：行列下标从 1 开始，三元组表上限 MAX_SIZE=%d\n", MAX_SIZE);
}

static void op_input(TriTupleTable *m, int *has, char name, const char *done_title) {
    if (input_matrix(m, name)) {
        *has = 1;
        print_matrix(m, done_title);
    }
}

static void op_show(int has, const TriTupleTable *m, char name, const char *title) {
    if (!has) { printf("  [!] 矩阵 %c 尚未输入\n", name); return; }
    print_matrix(m, title);
}

static void op_transpose(int has_a, const TriTupleTable *A, TriTupleTable *R) {
    if (!has_a) { printf("  [!] 矩阵 A 尚未输入\n"); return; }
    transpose_fast(A, R);
    print_matrix(R, "A 的转置 A^T");
}

static void op_add(int has_a, int has_b,
                   const TriTupleTable *A, const TriTupleTable *B, TriTupleTable *R) {
    if (!has_a || !has_b) { printf("  [!] 需先输入 A 和 B\n"); return; }
    if (!add_matrix(A, B, R)) {
        printf("  [!] A 与 B 形状不一致 (A: %d x %d, B: %d x %d)，无法相加\n",
               A->m, A->n, B->m, B->n);
        return;
    }
    print_matrix(R, "A + B");
}

// 菜单主循环：把所有命令分发集中在这里，main 只负责启动
static void run(void) {
    TriTupleTable A, B, R;
    int has_a = 0, has_b = 0;

    print_welcome();
    while (1) {
        print_status(has_a, has_b, &A, &B);
        print_menu();
        int op = read_int("选择: ");
        switch (op) {
            case 1: op_input(&A, &has_a, 'A', "矩阵 A 录入完成"); break;
            case 2: op_input(&B, &has_b, 'B', "矩阵 B 录入完成"); break;
            case 3: op_show(has_a, &A, 'A', "矩阵 A"); break;
            case 4: op_show(has_b, &B, 'B', "矩阵 B"); break;
            case 5: op_transpose(has_a, &A, &R); break;
            case 6: op_add(has_a, has_b, &A, &B, &R); break;
            case 0: printf("亲爱的涂老师，再见。\n"); return;
            default: printf("  [!] 无效选项\n");
        }
    }
}



int main(void) {
    run();
    return 0;
}
