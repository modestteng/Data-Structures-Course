---
title: "实验 5 · 链栈 · 字符"
date: 2026-05-16 09:05:00 +0800
categories: ["数据结构", "实验"]
tags: ["C", "栈", "链表"]
math: false
pin: false
toc: true
comments: false
---

链式栈：每个节点存一个字符，无容量上限，入栈动态分配、出栈释放节点。

> 课程：《数据结构》（涂老师）。本文代码遵循仓库 [`AGENTS.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/AGENTS.md) 与 [`CLAUDE.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/CLAUDE.md) 的约定。

## 源代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 链式栈结点定义。每个结点保存一个字符，通过 next 指针链接形成栈结构。
   与顺序栈不同，链式栈没有容量上限，每次入栈动态分配结点，出栈释放结点。 */
typedef struct StackNode {
    char data;
    struct StackNode *next;
} StackNode;

/* 链式栈定义。top 指向栈顶结点，无需头结点，空栈时 top 为 NULL。 */
//每一个节点存储一个字符，在这个整个栈之中，又在整体，或者说外面有一个栈顶指针
typedef struct {
    StackNode *top;
} LinkStack;
//这个节点名称是一个静态的地址，节点指针，就是往上抽象了一层，就是一个外面的东西，指向这个静态的地址
/* 初始化链式栈，将栈顶指针置为 NULL 表示空栈 */
//无头
void init_stack(LinkStack *s)
{
    s->top = NULL;
}

/* 判断栈是否为空 */
int is_empty(LinkStack *s)
{
    return s->top == NULL;
}

/* 入栈操作。动态创建新结点，采用头插法插入栈顶。
   链式栈的入栈本质上就是单链表的头插，新结点的 next 指向原栈顶。 */
void push(LinkStack *s, char c)
{
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    node->data = c;
    node->next = s->top;
    s->top = node;
}

/* 出栈操作。取出栈顶结点的数据后释放该结点，栈顶下移。
   调用前需确保栈非空，否则会访问空指针。 */
char pop(LinkStack *s)
{
    //这一步取出栈顶指针
    StackNode *p = s->top;
    char c = p->data;
    //这一步下移
    s->top = p->next;
    free(p);
    return c;
}


/* 取栈顶元素但不弹出 */
char get_top(LinkStack *s)
{
    return s->top->data;
}

/* 销毁链式栈，释放所有剩余结点，防止内存泄漏 */
void destroy_stack(LinkStack *s)
{
    StackNode *p = s->top;
    while (p) {
        StackNode *q = p->next;
        free(p);
        //p为空之后重新这个进行这个赋值
        p = q;
    }
    s->top = NULL;
}

/* 判断左右括号是否配对：( 配 )，[ 配 ] */
int is_match(char left, char right)
{
    return (left == '(' && right == ')') ||
           (left == '[' && right == ']');
}

/* 括号匹配检测主逻辑。
   利用栈的后进先出特性：左括号入栈，遇到右括号时与栈顶左括号配对。
   算法结束时栈应恰好为空，否则说明有多余的左括号。 */
   //采用这个字符指针，只想这个字符串的首地址
void check_brackets(char *expr)
{
    LinkStack s;
    int i;
    //这就相当于是直接根据这个字符数组的首地址计算出来这个字符串的长度了
    int len = (int)strlen(expr);
    // 传这个指针还是这个地址都可以，只是引用的方式不同，引用这个成员
    init_stack(&s);

    for (i = 0; i < len; i++) {
        char c = expr[i];

        /* 遇到左括号，直接压栈等待后续匹配 */
        if (c == '(' || c == '[') {
            push(&s, c);
        }
        /* 遇到右括号，需要和栈顶的左括号进行配对 */
        else if (c == ')' || c == ']') {
            /* 栈为空说明右括号多余，没有对应的左括号 */
            if (is_empty(&s)) {
                printf("不匹配 —— 右括号 '%c' 多余（位置 %d）\n", c, i + 1);
                return;
            }

            /* 取出栈顶左括号检查是否与当前右括号类型一致 */
            //实际上也就改变了这一点
            char top = get_top(&s);
            if (is_match(top, c)) {
                pop(&s);
            } else {
                /* 括号类型不一致，例如 ( 遇到了 ] */
                printf("不匹配 —— 括号类型不匹配：'%c'（位置 %d）与 '%c' 不配对\n",top, i + 1, c);
                destroy_stack(&s);
                return;
            }
        }
        /* 非括号字符直接跳过，不影响匹配判断 */
    }

    /* 扫描结束后，如果栈中仍有左括号，说明左括号多余 */
    if (!is_empty(&s)) {
        printf("不匹配 —— 左括号 '%c' 多余\n", get_top(&s));
        destroy_stack(&s);
    } else {
        printf("匹配 —— 括号完全匹配！\n");
    }
}

/* 功能1：输入表达式并检测括号匹配 */
void task_check(void)
{
    char expr[1024];

    printf("请输入算术表达式（回车结束）: ");
    scanf("%s", expr);

    check_brackets(expr);
}

void menu(void)
{
    printf("\n========== 括号匹配检测 ==========\n");
    printf("  1. 输入表达式并检测括号匹配\n");
    printf("  0. 退出\n");
    printf("==================================\n");
    printf("请选择功能: ");
}

/* 主循环：显示菜单、读取用户选择、分发到对应功能 */
void run(void)
{
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);
        /* 清除 scanf 残留的换行符，避免影响下一次输入 */
        // 一直读，读到这个换行符为止
        while (getchar() != '\n');

        if (choice == 0) {
            printf("程序结束。\n");
            break;
        }

        switch (choice) {
        case 1:
            task_check();
            break;
        default:
            printf("无效选项，请重新选择。\n");
            break;
        }
    }
}

int main(void)
{
    run();
    return 0;
}
```

## 运行示例

在 VS Code 打开源文件后按 **Ctrl+Shift+B** 运行项目预置的 `Run current C file` 任务，或在命令行：

```powershell
gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 homework/experiment_5.c -o homework/experiment_5.exe
.\homework\experiment_5.exe
```

