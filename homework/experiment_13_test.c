// 实验13的功能性 / 健壮性自测程序：编译后直接运行，会自动对比每个用例的预期输出
// 编译：gcc -std=c99 -Wall -Wextra -o experiment_13_test experiment_13_test.c
// 这个文件不参与正式作业提交，只用来验证 experiment_13.c 里的算法是否正确

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef enum { ATOM, LIST } Tag;

typedef struct Node {
    Tag tag;
    union {
        char atom;
        struct Node *list;
    } data;
    struct Node *next;
} Node, *GList;

typedef struct {
    const char *s;
    int idx;
} Parser;


static void skip_blank(Parser *p) {
    while (p->s[p->idx] == ' ' || p->s[p->idx] == '\t') p->idx++;
}

static GList new_node(Tag t) {
    GList n = (GList)malloc(sizeof(Node));
    if (!n) { fprintf(stderr, "OOM\n"); exit(1); }
    n->tag = t; n->next = NULL;
    if (t == ATOM) n->data.atom = '\0'; else n->data.list = NULL;
    return n;
}

void FreeGList(GList L) {
    if (!L) return;
    if (L->tag == LIST) FreeGList(L->data.list);
    FreeGList(L->next);
    free(L);
}

static GList parse_element(Parser *p);

static GList parse_sequence(Parser *p) {
    GList head = parse_element(p);
    if (!head) return NULL;
    GList tail = head;
    skip_blank(p);
    while (p->s[p->idx] == ',') {
        p->idx++;
        GList node = parse_element(p);
        if (!node) { FreeGList(head); return NULL; }
        tail->next = node; tail = node;
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
            wrap->data.list = NULL;
        } else {
            wrap->data.list = parse_sequence(p);
            if (wrap->data.list == NULL) { free(wrap); return NULL; }
        }
        skip_blank(p);
        if (p->s[p->idx] == ')') p->idx++;
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

GList Create(const char *str) {
    Parser p = { str, 0 };
    skip_blank(&p);
    if (p.s[p.idx] != '(') return NULL;
    GList g = parse_element(&p);
    skip_blank(&p);
    if (g == NULL || p.s[p.idx] != '\0') {
        FreeGList(g);
        return NULL;
    }
    return g;
}

void Print(GList L, char *buf, int *idx) {
    if (!L) { buf[(*idx)++] = '('; buf[(*idx)++] = ')'; return; }
    if (L->tag == ATOM) { buf[(*idx)++] = L->data.atom; return; }
    buf[(*idx)++] = '(';
    GList p = L->data.list;
    while (p) {
        Print(p, buf, idx);
        if (p->next) buf[(*idx)++] = ',';
        p = p->next;
    }
    buf[(*idx)++] = ')';
}

static char swap_case(char c) {
    if (c >= 'a' && c <= 'z') return (char)(c - 'a' + 'A');
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}

GList CopySwapCase(GList src) {
    if (!src) return NULL;
    GList dst = new_node(src->tag);
    if (src->tag == ATOM) dst->data.atom = swap_case(src->data.atom);
    else dst->data.list = CopySwapCase(src->data.list);
    dst->next = CopySwapCase(src->next);
    return dst;
}

int Length(GList L) {
    if (!L || L->tag != LIST) return 0;
    int cnt = 0;
    for (GList p = L->data.list; p; p = p->next) cnt++;
    return cnt;
}

int Depth(GList L) {
    if (!L) return 1;
    if (L->tag == ATOM) return 0;
    GList p = L->data.list;
    if (!p) return 1;
    int max = 0;
    while (p) {
        int d = (p->tag == ATOM) ? 0 : Depth(p);
        if (d > max) max = d;
        p = p->next;
    }
    return max + 1;
}

static int print_to_buf(GList L, char *buf) {
    int idx = 0;
    Print(L, buf, &idx);
    buf[idx] = '\0';
    return idx;
}


// ====================== 测试用例 ======================

static int g_pass = 0, g_fail = 0;

static void check_str(const char *name, const char *got, const char *expect) {
    if (strcmp(got, expect) == 0) {
        printf("  PASS  %-30s  -> %s\n", name, got);
        g_pass++;
    } else {
        printf("  FAIL  %-30s  expected=%s got=%s\n", name, expect, got);
        g_fail++;
    }
}

static void check_int(const char *name, int got, int expect) {
    if (got == expect) {
        printf("  PASS  %-30s  -> %d\n", name, got);
        g_pass++;
    } else {
        printf("  FAIL  %-30s  expected=%d got=%d\n", name, expect, got);
        g_fail++;
    }
}

static void test_valid(const char *input,
                       const char *expect_print,
                       const char *expect_copy,
                       int expect_len, int expect_depth) {
    printf("\n[CASE] %s\n", input);
    GList g = Create(input);
    if (!g) { printf("  FAIL  Create returned NULL\n"); g_fail++; return; }

    char buf[256];
    print_to_buf(g, buf);
    check_str("Print", buf, expect_print);
    check_int("Length", Length(g), expect_len);
    check_int("Depth",  Depth(g),  expect_depth);

    GList copy = CopySwapCase(g);
    print_to_buf(copy, buf);
    check_str("CopySwapCase", buf, expect_copy);
    // 副本应当与原表结构等价：长度和深度相同
    check_int("Length(copy)", Length(copy), expect_len);
    check_int("Depth(copy)",  Depth(copy),  expect_depth);

    FreeGList(g);
    FreeGList(copy);
}

static void test_invalid(const char *input) {
    printf("\n[CASE-INVALID] \"%s\"\n", input);
    GList g = Create(input);
    if (g == NULL) {
        printf("  PASS  rejected as invalid\n");
        g_pass++;
    } else {
        char buf[256];
        print_to_buf(g, buf);
        printf("  FAIL  should be rejected, got=%s\n", buf);
        g_fail++;
        FreeGList(g);
    }
}

int main(void) {
    printf("========== 实验13 自动化测试 ==========\n");

    // 标准用例
    test_valid("()",            "()",          "()",          0, 1);
    test_valid("(a)",           "(a)",         "(A)",         1, 1);
    test_valid("(a,b,c)",       "(a,b,c)",     "(A,B,C)",     3, 1);
    test_valid("(a,(b,c),d)",   "(a,(b,c),d)", "(A,(B,C),D)", 3, 2);
    test_valid("((a,b),(c,d))", "((a,b),(c,d))","((A,B),(C,D))", 2, 2);
    test_valid("(((a)))",       "(((a)))",     "(((A)))",     1, 3);
    test_valid("(())",          "(())",        "(())",        1, 2);
    test_valid("((),a)",        "((),a)",      "((),A)",      2, 2);
    test_valid("(a,(B,(c,d)),E)","(a,(B,(c,d)),E)","(A,(b,(C,D)),e)", 3, 3);
    test_valid("(A,b,C,d)",     "(A,b,C,d)",   "(a,B,c,D)",   4, 1);
    // 含空白
    test_valid("( a , ( b , c ) , d )", "(a,(b,c),d)", "(A,(B,C),D)", 3, 2);
    test_valid(" (a)",          "(a)",         "(A)",         1, 1);

    // 非法用例：必须被拒绝
    test_invalid("");           // 空串
    test_invalid("abc");        // 没有括号
    test_invalid("(");          // 没有右括号
    test_invalid("(a");         // 没有右括号
    test_invalid("(a))");       // 多余的右括号
    test_invalid(")(");         // 顺序错
    test_invalid("(a,)");       // 尾随逗号 ← 修复前会被静默接受为 ()
    test_invalid("(a,b,)");     // 尾随逗号
    test_invalid("(,a)");       // 前置逗号
    test_invalid("(a,,b)");     // 双逗号
    test_invalid("(a b)");      // 缺少分隔符
    test_invalid("(1,2)");      // 数字非法
    test_invalid("(a;b)");      // 含非法字符
    test_invalid("((a)");       // 子表未闭合
    test_invalid("(a))b");      // 尾部多余字符

    printf("\n========== 结果：%d 通过 / %d 失败 ==========\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
