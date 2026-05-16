---
title: "实验 16 · 二叉树 · 基本操作"
date: 2026-05-16 09:16:00 +0800
categories: ["数据结构", "实验"]
tags: ["C", "二叉树", "遍历", "队列"]
math: false
pin: false
toc: true
comments: false
---

二叉链表 + 顺序栈 + 链队列三件套：先/中/后序遍历（递归 + 非递归），层序遍历，最大宽度（队列节点附带 level 字段）。

> 课程：《数据结构》（涂老师）。本文代码遵循仓库 [`AGENTS.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/AGENTS.md) 与 [`CLAUDE.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/CLAUDE.md) 的约定。

## 源代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STK_MAX 200
#define PATH_MAX_LEN 200
#define LINE_MAX_LEN 256

typedef char TreeData;

// 二叉链表：左右孩子指针即可表达任意二叉结构，区别于线性表的双链
typedef struct btnode {
    TreeData data;
    struct btnode *lchild, *rchild;
} BTNode, *BTree;

// 顺序栈元素附带 visited 标志，专门给非递归后序判断这是回溯还是初次入栈
typedef struct {
    BTNode *node;
    int visited;
} StkElem;

typedef struct {
    StkElem data[STK_MAX];
    int top;
} SeqStack;

// 链队列结点带 level，便于「最大宽度」一边出队一边按层计数
typedef struct qnode {
    BTNode *node;
    int level;
    struct qnode *next;
} QNode;

typedef struct {
    QNode *front, *rear;
} LinkQueue;

// 输入工具
// 反复读取直到拿到合法整数，避免菜单选项被脏输入卡死
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

// 读一行（含先序串）：fgets 不会像 scanf("%s") 那样遇空格中断
// 先清掉上一次 scanf("%d") 留下的换行符，再用 fgets 读整行
static int read_line(const char *prompt, char *buf, int n) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    printf("%s", prompt);
    if (!fgets(buf, n, stdin)) { buf[0] = '\0'; return 0; }
    int len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[--len] = '\0';
    return len;
}

// 顺序栈
static void stk_init(SeqStack *s) { s->top = -1; }
static int stk_empty(const SeqStack *s) { return s->top < 0; }

static int stk_push(SeqStack *s, BTNode *node, int visited) {
    if (s->top >= STK_MAX - 1) return 0;
    s->top++;
    s->data[s->top].node = node;
    s->data[s->top].visited = visited;
    return 1;
}

static int stk_pop(SeqStack *s, StkElem *out) {
    if (stk_empty(s)) return 0;
    *out = s->data[s->top];
    s->top--;
    return 1;
}

// 链队列：层次遍历的天然容器，FIFO 保证按层序号递增出队
static void q_init(LinkQueue *q) { q->front = q->rear = NULL; }
static int q_empty(const LinkQueue *q) { return q->front == NULL; }

static int q_enqueue(LinkQueue *q, BTNode *node, int level) {
    QNode *p = (QNode *)malloc(sizeof(QNode));
    if (!p) return 0;
    p->node = node;
    p->level = level;
    p->next = NULL;
    if (q->rear) q->rear->next = p; else q->front = p;
    q->rear = p;
    return 1;
}

static int q_dequeue(LinkQueue *q, BTNode **node, int *level) {
    if (q_empty(q)) return 0;
    QNode *p = q->front;
    *node = p->node;
    *level = p->level;
    q->front = p->next;
    if (!q->front) q->rear = NULL;
    free(p);
    return 1;
}

static void q_destroy(LinkQueue *q) {
    BTNode *n; int lv;
    while (q_dequeue(q, &n, &lv)) {}
}

// 二叉树构造与销毁
// 先序串 + '#' 标空：唯一确定一棵二叉树；递归过程隐含「根 - 左 - 右」顺序
static BTNode *build_preorder(const char *s, int *idx) {
    char ch = s[*idx];
    if (ch == '\0') return NULL;
    (*idx)++;
    if (ch == '#') return NULL;
    BTNode *t = (BTNode *)malloc(sizeof(BTNode));
    if (!t) return NULL;
    t->data = ch;
    t->lchild = build_preorder(s, idx);
    t->rchild = build_preorder(s, idx);
    return t;
}

// 后序释放：必须先 free 子树再 free 自己，否则丢失指针
static void destroy_tree(BTNode *t) {
    if (!t) return;
    destroy_tree(t->lchild);
    destroy_tree(t->rchild);
    free(t);
}

// 教材常见样例 ABD##E##C##，根 A，左子 B(D,E)，右子 C
static BTNode *load_demo_tree(void) {
    const char *demo = "ABD##E##C##";
    int idx = 0;
    return build_preorder(demo, &idx);
}

// 递归遍历
static void preorder_rec(const BTNode *t) {
    if (!t) return;
    printf("%c ", t->data);
    preorder_rec(t->lchild);
    preorder_rec(t->rchild);
}

static void inorder_rec(const BTNode *t) {
    if (!t) return;
    inorder_rec(t->lchild);
    printf("%c ", t->data);
    inorder_rec(t->rchild);
}

static void postorder_rec(const BTNode *t) {
    if (!t) return;
    postorder_rec(t->lchild);
    postorder_rec(t->rchild);
    printf("%c ", t->data);
}

// 层次遍历用队列：根入队，出一个就把它的左右孩子按序入队
static void levelorder(BTNode *t) {
    if (!t) return;
    LinkQueue q; q_init(&q);
    q_enqueue(&q, t, 1);
    BTNode *cur; int lv;
    while (q_dequeue(&q, &cur, &lv)) {
        printf("%c ", cur->data);
        if (cur->lchild) q_enqueue(&q, cur->lchild, lv + 1);
        if (cur->rchild) q_enqueue(&q, cur->rchild, lv + 1);
    }
    q_destroy(&q);
}

// 非递归遍历
// 先序非递归：栈模拟递归调用栈，先压右后压左，弹出顺序即「根左右」
static void preorder_iter(BTNode *t) {
    if (!t) return;
    SeqStack s; stk_init(&s);
    stk_push(&s, t, 0);
    StkElem e;
    while (stk_pop(&s, &e)) {
        printf("%c ", e.node->data);
        if (e.node->rchild) stk_push(&s, e.node->rchild, 0);
        if (e.node->lchild) stk_push(&s, e.node->lchild, 0);
    }
}

// 中序非递归：沿左链一路压栈，然后弹出访问，再转向右子树重复
static void inorder_iter(BTNode *t) {
    SeqStack s; stk_init(&s);
    BTNode *p = t;
    while (p || !stk_empty(&s)) {
        while (p) {
            stk_push(&s, p, 0);
            p = p->lchild;
        }
        StkElem e = {0};
        if (!stk_pop(&s, &e)) break;
        printf("%c ", e.node->data);
        p = e.node->rchild;
    }
}

// 后序非递归：栈元素带 visited 标志，第一次出栈代表「该轮到右子树了」，
// 第二次（visited=1）才真正访问该结点，避免双栈或额外指针变量
static void postorder_iter(BTNode *t) {
    if (!t) return;
    SeqStack s; stk_init(&s);
    stk_push(&s, t, 0);
    StkElem e;
    while (stk_pop(&s, &e)) {
        if (e.visited) {
            printf("%c ", e.node->data);
        } else {
            stk_push(&s, e.node, 1);
            if (e.node->rchild) stk_push(&s, e.node->rchild, 0);
            if (e.node->lchild) stk_push(&s, e.node->lchild, 0);
        }
    }
}

// 度量
static int tree_depth(const BTNode *t) {
    if (!t) return 0;
    int dl = tree_depth(t->lchild);
    int dr = tree_depth(t->rchild);
    return (dl > dr ? dl : dr) + 1;
}

static int tree_count(const BTNode *t) {
    if (!t) return 0;
    return 1 + tree_count(t->lchild) + tree_count(t->rchild);
}

static int tree_leaf_count(const BTNode *t) {
    if (!t) return 0;
    if (!t->lchild && !t->rchild) return 1;
    return tree_leaf_count(t->lchild) + tree_leaf_count(t->rchild);
}

// 结构变换：先序递归镜像；交换在「访问当前结点」位置完成即可
static void swap_children(BTNode *t) {
    if (!t) return;
    BTNode *tmp = t->lchild;
    t->lchild = t->rchild;
    t->rchild = tmp;
    swap_children(t->lchild);
    swap_children(t->rchild);
}

// 双序遍历：每个非空结点恰好被访问 2 次，
// 顺序为 访问根 → 双序左 → 再访问根 → 双序右
static void double_order(const BTNode *t) {
    if (!t) return;
    printf("%c ", t->data);
    double_order(t->lchild);
    printf("%c ", t->data);
    double_order(t->rchild);
}

// 最大宽度：层次遍历同时记录层号，扫描时按层累计计数
static int max_width(BTNode *t) {
    if (!t) return 0;
    LinkQueue q; q_init(&q);
    q_enqueue(&q, t, 1);
    BTNode *cur; int lv;
    int cur_lv = 0, cur_cnt = 0, best = 0;
    while (q_dequeue(&q, &cur, &lv)) {
        if (lv != cur_lv) {
            if (cur_cnt > best) best = cur_cnt;
            cur_lv = lv;
            cur_cnt = 0;
        }
        cur_cnt++;
        if (cur->lchild) q_enqueue(&q, cur->lchild, lv + 1);
        if (cur->rchild) q_enqueue(&q, cur->rchild, lv + 1);
    }
    if (cur_cnt > best) best = cur_cnt;
    q_destroy(&q);
    return best;
}

// 第一条最长路径：DFS 维护当前路径栈
// 「第一条」由先序顺序（左子树优先）天然保证，>= 改成 > 即可保留最早的
static void dfs_longest(BTNode *t,
                        BTNode *cur_path[], int cur_len,
                        BTNode *best_path[], int *best_len) {
    if (!t) return;
    cur_path[cur_len++] = t;
    if (!t->lchild && !t->rchild) {
        if (cur_len > *best_len) {
            *best_len = cur_len;
            for (int i = 0; i < cur_len; i++) best_path[i] = cur_path[i];
        }
        return;
    }
    dfs_longest(t->lchild, cur_path, cur_len, best_path, best_len);
    dfs_longest(t->rchild, cur_path, cur_len, best_path, best_len);
}

static void show_first_longest_path(BTNode *t) {
    if (!t) { printf("  (空树)\n"); return; }
    BTNode *cur_path[PATH_MAX_LEN];
    BTNode *best_path[PATH_MAX_LEN];
    int best_len = 0;
    dfs_longest(t, cur_path, 0, best_path, &best_len);
    printf("  路径长度（结点数）= %d，边数 = %d\n", best_len, best_len - 1);
    printf("  路径：");
    for (int i = 0; i < best_len; i++) {
        printf("%c", best_path[i]->data);
        if (i + 1 < best_len) printf(" -> ");
    }
    printf("\n");
}

// 树形显示：右子树在上、左子树在下、按层缩进的横向打印，便于人工核对
static void print_tree(const BTNode *t, int depth) {
    if (!t) return;
    print_tree(t->rchild, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%c\n", t->data);
    print_tree(t->lchild, depth + 1);
}

// 菜单 / 主流程
static void print_status(BTNode *t) {
    printf("\n[状态] 二叉树: ");
    if (!t) { printf("未构建\n"); return; }
    printf("已构建 (n=%d, h=%d, leaves=%d)\n",
           tree_count(t), tree_depth(t), tree_leaf_count(t));
}

static void print_menu(void) {
    printf("--------------------------------------------------\n");
    printf("  1) 构建二叉树（先序输入，# 表示空）\n");
    printf("  2) 加载示例树 ABD##E##C##\n");
    printf("  3) 递归遍历（先序/中序/后序/层次）\n");
    printf("  4) 非递归遍历（先序/中序/后序）\n");
    printf("  5) 求 深度 / 结点数 / 叶子数\n");
    printf("  6) 交换所有结点的左右子树\n");
    printf("  7) 双序遍历\n");
    printf("  8) 最大宽度\n");
    printf("  9) 第一条最长路径\n");
    printf(" 10) 显示树形结构\n");
    printf("  0) 退出\n");
}

static void print_welcome(void) {
    printf("尊敬的涂老师，你好。\n");
    printf("=== 二叉树基本操作 实验 16 ===\n");
    printf("说明：先序串以 # 表示空指针，例如 ABD##E##C## 对应根 A，左子 B(D,E)，右子 C\n");
}

// 把先序串里的空白与制表跳过，避免用户多敲了空格就解析失败
static void strip_blanks(char *s) {
    int j = 0;
    for (int i = 0; s[i]; i++) {
        if (!isspace((unsigned char)s[i])) s[j++] = s[i];
    }
    s[j] = '\0';
}

static void op_build(BTree *t) {
    char buf[LINE_MAX_LEN];
    read_line("  输入先序串（含 #，回车结束）: ", buf, LINE_MAX_LEN);
    strip_blanks(buf);
    if (buf[0] == '\0') { printf("  [!] 输入为空，已忽略\n"); return; }
    int idx = 0;
    BTNode *new_t = build_preorder(buf, &idx);
    if (buf[idx] != '\0') {
        printf("  [!] 提示：剩余字符 \"%s\" 未被使用（先序串过长）\n", buf + idx);
    }
    if (*t) destroy_tree(*t);
    *t = new_t;
    if (new_t) printf("  [√] 构建成功\n");
    else printf("  [√] 构建为空树\n");
}

static void op_demo(BTree *t) {
    if (*t) destroy_tree(*t);
    *t = load_demo_tree();
    printf("  [√] 已加载示例树\n");
}

static int need_tree(BTree t) {
    if (!t) {
        printf("  [!] 当前为空树，请先用 1) 构建 或 2) 加载示例\n");
        return 0;
    }
    return 1;
}

static void op_traverse_rec(BTree t) {
    if (!need_tree(t)) return;
    printf("  1) 先序  2) 中序  3) 后序  4) 层次\n");
    int k = read_int("  选择: ");
    switch (k) {
        case 1: printf("  先序: "); preorder_rec(t); printf("\n"); break;
        case 2: printf("  中序: "); inorder_rec(t); printf("\n"); break;
        case 3: printf("  后序: "); postorder_rec(t); printf("\n"); break;
        case 4: printf("  层次: "); levelorder(t); printf("\n"); break;
        default: printf("  [!] 无效选项\n");
    }
}

static void op_traverse_iter(BTree t) {
    if (!need_tree(t)) return;
    printf("  1) 先序  2) 中序  3) 后序\n");
    int k = read_int("  选择: ");
    switch (k) {
        case 1: printf("  先序(非递归): "); preorder_iter(t); printf("\n"); break;
        case 2: printf("  中序(非递归): "); inorder_iter(t); printf("\n"); break;
        case 3: printf("  后序(非递归): "); postorder_iter(t); printf("\n"); break;
        default: printf("  [!] 无效选项\n");
    }
}

static void op_metrics(BTree t) {
    if (!need_tree(t)) return;
    printf("  深度 = %d\n", tree_depth(t));
    printf("  结点数 = %d\n", tree_count(t));
    printf("  叶子数 = %d\n", tree_leaf_count(t));
}

static void op_swap(BTree t) {
    if (!need_tree(t)) return;
    swap_children(t);
    printf("  [√] 已交换左右子树（所有结点）\n");
    printf("  交换后先序: "); preorder_rec(t); printf("\n");
}

static void op_double(BTree t) {
    if (!need_tree(t)) return;
    printf("  双序: "); double_order(t); printf("\n");
}

static void op_width(BTree t) {
    if (!need_tree(t)) return;
    printf("  最大宽度 = %d\n", max_width(t));
}

static void op_longest(BTree t) {
    if (!need_tree(t)) return;
    show_first_longest_path(t);
}

static void op_show(BTree t) {
    if (!need_tree(t)) return;
    printf("  树形（右上左下，每层缩进 4 空格）:\n");
    print_tree(t, 0);
}

static void run(void) {
    BTree t = NULL;
    print_welcome();
    while (1) {
        print_status(t);
        print_menu();
        int op = read_int("选择: ");
        switch (op) {
            case 1:  op_build(&t); break;
            case 2:  op_demo(&t); break;
            case 3:  op_traverse_rec(t); break;
            case 4:  op_traverse_iter(t); break;
            case 5:  op_metrics(t); break;
            case 6:  op_swap(t); break;
            case 7:  op_double(t); break;
            case 8:  op_width(t); break;
            case 9:  op_longest(t); break;
            case 10: op_show(t); break;
            case 0:
                if (t) destroy_tree(t);
                printf("亲爱的涂老师，再见。\n");
                return;
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
gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 homework/experiment_16.c -o homework/experiment_16.exe
.\homework\experiment_16.exe
```

