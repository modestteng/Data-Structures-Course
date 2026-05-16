#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "glist.h"

#define BUF_SIZE 256

// 输入工具
// 非整数输入时清缓冲再重提，避免 scanf 卡死
static int read_int(const char *prompt) {
    int x;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &x) == 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            return x;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("  [!] 不是整数，请重新输入\n");
    }
}

static int read_gl_string(char *buf, int size) {
    int c, i = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ' && c != '\t' && c != '\r' && i < size - 1) {
            buf[i++] = (char)c;
        }
    }
    buf[i] = '\0';
    return i > 0 || c == '\n';
}

// 文法 GL = '(' [ELEM (',' ELEM)*] ')'，原子限单字符字母数字
// prev 记上一个 token：0 起始 / 1 '(' / 2 元素结束 / 3 ','
static int validate_gl_string(const char *s, char *err_buf, int err_size) {
    const char *orig = s;
    int depth = 0;
    int prev = 0;

    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;
        long pos = (long)(s - orig) + 1;

        if (c >= 128 || c < 32) {
            snprintf(err_buf, err_size, "位置 %ld：含非 ASCII 或控制字符（仅允许 ( ) , 字母 数字）", pos);
            return 0;
        }

        if (c == '(') {
            if (prev == 2) {
                snprintf(err_buf, err_size, "位置 %ld：'(' 前缺少分隔符 ','", pos);
                return 0;
            }
            depth++;
            prev = 1;
        } else if (c == ')') {
            if (prev == 3) {
                snprintf(err_buf, err_size, "位置 %ld：',' 后不能直接 ')'", pos);
                return 0;
            }
            if (depth == 0) {
                snprintf(err_buf, err_size, "位置 %ld：右括号过多", pos);
                return 0;
            }
            depth--;
            prev = 2;
        } else if (c == ',') {
            if (prev != 2) {
                snprintf(err_buf, err_size, "位置 %ld：',' 必须紧跟一个完整元素", pos);
                return 0;
            }
            prev = 3;
        } else if (isalnum(c)) {
            if (depth == 0) {
                snprintf(err_buf, err_size, "位置 %ld：括号外不允许出现内容", pos);
                return 0;
            }
            if (prev == 2) {
                snprintf(err_buf, err_size,
                         "位置 %ld：原子 '%c' 前缺少分隔符（原子限定单字符，多字符请用 ',' 分开）",
                         pos, (char)c);
                return 0;
            }
            prev = 2;
        } else {
            snprintf(err_buf, err_size,
                     "位置 %ld：非法字符 '%c'（仅允许 ( ) , 字母 数字）", pos, (char)c);
            return 0;
        }
    }

    if (prev == 0) {
        snprintf(err_buf, err_size, "空输入");
        return 0;
    }
    if (depth != 0) {
        snprintf(err_buf, err_size, "括号不匹配（缺 %d 个 ')'）", depth);
        return 0;
    }
    if (prev == 3) {
        snprintf(err_buf, err_size, "结尾多了一个 ','");
        return 0;
    }
    return 1;
}

// 显示与菜单
static void print_status(int has, const char *src) {
    printf("\n[状态] ");
    if (has) {
        printf("当前广义表 = %s\n", src);
    } else {
        printf("广义表未输入\n");
    }
}

static void print_menu(void) {
    printf("--------------------------------------------------\n");
    printf(" 1) 输入广义表（括号表示法，例如 (a,b,(c,d)) ）\n");
    printf(" 2) 显示广义表 DispGL\n");
    printf(" 3) 求长度 GLLength\n");
    printf(" 4) 求深度 GLDepth\n");
    printf(" 5) 销毁广义表\n");
    printf(" 6) 跑一组内置测试用例\n");
    printf(" 0) 退出\n");
}

static void print_welcome(void) {
    printf("尊敬的涂老师，你好。\n");
    printf("=== 广义表算法库 实验 12 ===\n");
    printf("说明：\n");
    printf("  - 原子用单个字符表示（建议字母或数字）\n");
    printf("  - 用 '(' 开头、')' 结尾，用 ',' 分隔元素\n");
    printf("  - 例：()、(a)、(a,b,(c,d))、((a,b),(c))、(a,(b,(c,(d))))\n");
}

// 操作分发
static void op_input(GLNode **g, char *src_buf, int *has) {
    char buf[BUF_SIZE];
    char err[128];
    char *p;
    GLNode *new_g;

    printf("请输入括号表示法字符串（空格自动忽略）：\n> ");
    if (!read_gl_string(buf, BUF_SIZE)) {
        printf("  [!] 读取失败\n");
        return;
    }
    if (buf[0] == '\0') {
        printf("  [!] 空输入\n");
        return;
    }
    if (!validate_gl_string(buf, err, sizeof(err))) {
        printf("  [!] %s\n", err);
        return;
    }

    p = buf;
    new_g = CreateGL(&p);

    // 校验通过后还检查一遍残余字符——防御 CreateGL 自身的实现 bug
    if (*p != '\0') {
        printf("  [!] 解析未消费完整个输入，剩余：%s（请反馈作业作者）\n", p);
        if (new_g != NULL) DestroyGL(new_g);
        return;
    }
    if (new_g == NULL) {
        printf("  [!] 解析得到空结构\n");
        return;
    }

    if (*has) {
        DestroyGL(*g);
        *g = NULL;
        *has = 0;
    }
    strncpy(src_buf, buf, BUF_SIZE - 1);
    src_buf[BUF_SIZE - 1] = '\0';
    *g = new_g;
    *has = 1;
    printf("  解析完成：");
    DispGL(*g);
    printf("\n");
}

static void op_disp(GLNode *g, int has) {
    if (!has) { printf("  [!] 广义表尚未输入\n"); return; }
    printf("  广义表：");
    DispGL(g);
    printf("\n");
}

static void op_length(GLNode *g, int has) {
    if (!has) { printf("  [!] 广义表尚未输入\n"); return; }
    printf("  长度 = %d\n", GLLength(g));
}

static void op_depth(GLNode *g, int has) {
    if (!has) { printf("  [!] 广义表尚未输入\n"); return; }
    printf("  深度 = %d\n", GLDepth(g));
}

static void op_destroy(GLNode **g, int *has) {
    if (!*has) { printf("  [!] 广义表尚未输入\n"); return; }
    DestroyGL(*g);
    *g = NULL;
    *has = 0;
    printf("  已销毁\n");
}

// 内置测试：空表 / 单原子 / 嵌套 / 深嵌套
static void run_builtin_tests(void) {
    const char *cases[] = {
        "()",
        "(a)",
        "(a,b,(c,d))",
        "((a,b),(c))",
        "(a,(b,(c,(d))))",
        "(((x)))"
    };
    int n = (int)(sizeof(cases) / sizeof(cases[0]));
    char buf[BUF_SIZE];

    printf("\n  内置测试用例：\n");
    for (int i = 0; i < n; i++) {
        strncpy(buf, cases[i], BUF_SIZE - 1);
        buf[BUF_SIZE - 1] = '\0';
        char *p = buf;
        GLNode *g = CreateGL(&p);
        printf("  输入: %-22s  ->  DispGL: ", cases[i]);
        DispGL(g);
        printf("   长度=%d   深度=%d\n", GLLength(g), GLDepth(g));
        DestroyGL(g);
    }
}

// 菜单主循环
static void run(void) {
    GLNode *g = NULL;
    int has = 0;
    char src[BUF_SIZE] = "";

    print_welcome();
    while (1) {
        print_status(has, src);
        print_menu();
        int op = read_int("选择: ");
        switch (op) {
            case 1: op_input(&g, src, &has); break;
            case 2: op_disp(g, has); break;
            case 3: op_length(g, has); break;
            case 4: op_depth(g, has); break;
            case 5: op_destroy(&g, &has); break;
            case 6: run_builtin_tests(); break;
            case 0:
                if (has) DestroyGL(g);
                printf("亲爱的涂老师，再见。\n");
                return;
            default:
                printf("  [!] 无效选项\n");
        }
    }
}

int main(void) {
    run();
    return 0;
}
