#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// 广义表节点：原子(单字母)或子表(指向其孩子链表头)
typedef enum { ATOM, LIST } Tag;

typedef struct Node {
    Tag tag;
    union {
        char atom;          // 原子的字符值
        struct Node *list;  // 子表第一个孩子
    } data;
    struct Node *next;      // 指向同一层的下一个兄弟
} Node, *GList;

// 解析时携带输入串和当前下标
typedef struct {
    const char *s;
    int idx;
} Parser;


static void skip_blank(Parser *p) {
    while (p->s[p->idx] == ' ' || p->s[p->idx] == '\t') p->idx++;
}

static GList new_node(Tag t) {
    GList n = (GList)malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    n->tag = t;
    n->next = NULL;
    if (t == ATOM) n->data.atom = '\0';
    else n->data.list = NULL;
    return n;
}

// 释放整棵广义表：先放子表，再放兄弟，最后放自己
void FreeGList(GList L) {
    if (L == NULL) return;
    if (L->tag == LIST) FreeGList(L->data.list);
    FreeGList(L->next);
    free(L);
}


// 解析采用两个互相调用的函数：
//   element  = 字母 | '(' sequence? ')'
//   sequence = element ( ',' element )*
// 让每个 element 只消耗自己那一段输入，避免吃掉别层的右括号

static GList parse_element(Parser *p);

static GList parse_sequence(Parser *p) {
    GList head = parse_element(p);
    if (head == NULL) return NULL;
    GList tail = head;

    skip_blank(p);
    while (p->s[p->idx] == ',') {
        p->idx++;
        GList node = parse_element(p);
        if (node == NULL) {
            // 逗号后没接到元素，释放已建好的半截链让上层报错
            FreeGList(head);
            return NULL;
        }
        tail->next = node;
        tail = node;
        skip_blank(p);
    }
    return head;
}

static GList parse_element(Parser *p) {
    skip_blank(p);
    char c = p->s[p->idx];

    if (c == '(') {
        p->idx++;
        skip_blank(p);
        GList wrap = new_node(LIST);
        if (p->s[p->idx] == ')') {
            wrap->data.list = NULL;             // 空表 ()
        } else {
            // 进了这个分支说明括号内不是 ()，只要 sequence 返回 NULL 就一定是语法错误
            // (例如 (a,) 或 (,a) 这种逗号位置不合法的输入)
            wrap->data.list = parse_sequence(p);
            if (wrap->data.list == NULL) {
                free(wrap);
                return NULL;
            }
        }
        skip_blank(p);
        if (p->s[p->idx] == ')') {
            p->idx++;
        }
        // 没匹配到右括号留给 Create 通过 idx 判断
        return wrap;
    }

    if (isalpha((unsigned char)c)) {
        GList atom = new_node(ATOM);
        atom->data.atom = c;
        p->idx++;
        return atom;
    }

    return NULL;
}

// 对外入口：成功返回广义表头节点，失败返回 NULL 并把出错位置写回 *err_pos
GList Create(const char *str, int *err_pos) {
    Parser p = { str, 0 };
    skip_blank(&p);
    if (p.s[p.idx] != '(') {
        if (err_pos) *err_pos = p.idx;
        return NULL;
    }
    GList g = parse_element(&p);
    skip_blank(&p);
    if (g == NULL || p.s[p.idx] != '\0') {
        if (err_pos) *err_pos = p.idx;
        FreeGList(g);
        return NULL;
    }
    return g;
}


// 按广义表书写形式输出：原子直写字母，子表用 '(' 包裹孩子链，孩子间以 ',' 分隔
void Print(GList L) {
    if (L == NULL) {
        printf("()");
        return;
    }
    if (L->tag == ATOM) {
        putchar(L->data.atom);
        return;
    }
    putchar('(');
    GList p = L->data.list;
    while (p != NULL) {
        Print(p);
        if (p->next != NULL) putchar(',');
        p = p->next;
    }
    putchar(')');
}


// 大小写互换：非字母原样返回
static char swap_case(char c) {
    if (c >= 'a' && c <= 'z') return (char)(c - 'a' + 'A');
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}

// 复制广义表：原子复制时互换大小写，子表递归复制 data.list，
// 最后再递归处理 next 把整条兄弟链拷下来
GList CopySwapCase(GList src) {
    if (src == NULL) return NULL;

    GList dst = new_node(src->tag);
    if (src->tag == ATOM) {
        dst->data.atom = swap_case(src->data.atom);
    } else {
        dst->data.list = CopySwapCase(src->data.list);
    }
    dst->next = CopySwapCase(src->next);
    return dst;
}

// 长度：第一层元素的个数，沿 data.list 的 next 链计数
int Length(GList L) {
    if (L == NULL || L->tag != LIST) return 0;
    int cnt = 0;
    for (GList p = L->data.list; p != NULL; p = p->next) cnt++;
    return cnt;
}

// 深度：原子 0；空表 1；非空表 1 + max{ 一级元素的 depth }
int Depth(GList L) {
    if (L == NULL) return 1;
    if (L->tag == ATOM) return 0;

    GList p = L->data.list;
    if (p == NULL) return 1;

    int max = 0;
    while (p != NULL) {
        int d = (p->tag == ATOM) ? 0 : Depth(p);
        if (d > max) max = d;
        p = p->next;
    }
    return max + 1;
}


// 词法层面挡掉非法字符和括号不配平，结构合法性留给 parse_*
static int prevalidate(const char *s, int *err_pos) {
    int balance = 0;
    int started = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c == ' ' || c == '\t' || c == ',') continue;
        if (c == '(') { balance++; started = 1; }
        else if (c == ')') {
            balance--;
            if (balance < 0) { if (err_pos) *err_pos = i; return 0; }
        } else if (!isalpha((unsigned char)c)) {
            if (err_pos) *err_pos = i;
            return 0;
        }
    }
    if (!started || balance != 0) {
        if (err_pos) *err_pos = (int)strlen(s);
        return 0;
    }
    return 1;
}


// 读一整行并去掉行尾换行；EOF 返回 -1
static int read_line(char *buf, int cap) {
    if (fgets(buf, cap, stdin) == NULL) return -1;
    int n = (int)strlen(buf);
    while (n > 0 && (buf[n-1] == '\n' || buf[n-1] == '\r')) buf[--n] = '\0';
    return n;
}

// 读一个整数选项，允许前后空白
static int read_int(int *out) {
    char buf[64];
    int n = read_line(buf, sizeof(buf));
    if (n < 0) return -1;
    char *end;
    long v = strtol(buf, &end, 10);
    if (end == buf) return 0;
    while (*end == ' ' || *end == '\t') end++;
    if (*end != '\0') return 0;
    *out = (int)v;
    return 1;
}

// 在出错列下方画一个 '^' 方便定位
static void caret_at(const char *line, int col) {
    printf("  %s\n  ", line);
    for (int i = 0; i < col && line[i] != '\0'; i++) putchar(' ');
    printf("^\n");
}


static void show_banner(void) {
    printf("\n========== 广义表实验 13 ==========\n");
    printf("功能：递归创建广义表，并复制（原子大小写互换）、求长度、求深度\n");
    printf("说明：原子为单个英文字母；子表用英文括号 '()' 包围；元素间用英文逗号 ','\n");
    printf("示例：(a,(b,C),d)    或    ((a),B,(c,(d,E)))\n");
}

static void show_menu(int has_g, int has_copy) {
    printf("\n--- 操作菜单 ---\n");
    printf("1. 创建广义表%s\n", has_g ? "（将覆盖当前广义表）" : "");
    printf("2. 打印当前广义表\n");
    printf("3. 复制广义表（递归，复制时原子大小写互换）\n");
    printf("4. 打印副本%s\n", has_copy ? "" : "（尚未复制）");
    printf("5. 求当前广义表的长度\n");
    printf("6. 求当前广义表的深度\n");
    printf("7. 一次性完成本实验任务（复制 + 长度 + 深度）\n");
    printf("0. 退出\n");
    printf("请选择：");
}

void menu(void) {
    GList g = NULL;
    GList copy = NULL;       // 大小写互换后的副本
    char input[512];

    show_banner();

    for (;;) {
        show_menu(g != NULL, copy != NULL);

        int choice;
        int r = read_int(&choice);
        if (r < 0) {
            printf("\n输入结束，退出。\n");
            break;
        }
        if (r == 0) {
            printf("无效输入，请输入菜单中的数字编号。\n");
            continue;
        }

        if (choice == 0) {
            printf("退出程序。\n");
            break;
        }

        if (choice == 1) {
            printf("请输入广义表（如 (a,(b,C),d)，可以含空格）：");
            int len = read_line(input, sizeof(input));
            if (len < 0) { printf("\n输入结束。\n"); break; }
            if (len == 0) {
                printf("输入为空，未创建。\n");
                continue;
            }

            int err = 0;
            if (!prevalidate(input, &err)) {
                printf("非法字符或括号不配平。\n");
                caret_at(input, err);
                continue;
            }

            err = 0;
            GList ng = Create(input, &err);
            if (ng == NULL) {
                printf("结构错误：第 %d 列附近无法解析\n", err + 1);
                caret_at(input, err);
                continue;
            }

            // 旧表和旧副本一起换掉，避免副本指向已释放节点
            if (g) FreeGList(g);
            if (copy) { FreeGList(copy); copy = NULL; }
            g = ng;
            printf("创建成功：");
            Print(g);
            putchar('\n');
            continue;
        }

        if (choice == 2) {
            if (g == NULL) { printf("尚未创建广义表。\n"); continue; }
            printf("当前广义表：");
            Print(g);
            putchar('\n');
            continue;
        }

        if (choice == 3) {
            if (g == NULL) { printf("尚未创建广义表，无法复制。\n"); continue; }
            if (copy) FreeGList(copy);
            copy = CopySwapCase(g);
            printf("复制完成（原子大小写已互换）：");
            Print(copy);
            putchar('\n');
            continue;
        }

        if (choice == 4) {
            if (copy == NULL) { printf("尚未复制，副本为空。\n"); continue; }
            printf("当前副本：");
            Print(copy);
            putchar('\n');
            continue;
        }

        if (choice == 5) {
            if (g == NULL) { printf("尚未创建广义表。\n"); continue; }
            printf("长度 = %d\n", Length(g));
            continue;
        }

        if (choice == 6) {
            if (g == NULL) { printf("尚未创建广义表。\n"); continue; }
            printf("深度 = %d\n", Depth(g));
            continue;
        }

        if (choice == 7) {
            if (g == NULL) { printf("尚未创建广义表，无法执行任务。\n"); continue; }
            if (copy) FreeGList(copy);
            copy = CopySwapCase(g);
            printf("原表：");  Print(g);    putchar('\n');
            printf("副本：");  Print(copy); putchar('\n');
            printf("长度 = %d\n", Length(g));
            printf("深度 = %d\n", Depth(g));
            continue;
        }

        printf("无效选项，请重新选择。\n");
    }

    if (g) FreeGList(g);
    if (copy) FreeGList(copy);
}

int main(void) {
    menu();
    return 0;
}
