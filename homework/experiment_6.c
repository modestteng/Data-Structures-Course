#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 100

// 这个是存数字用的栈，后面算后缀表达式的时候要用
typedef struct {
    int data[MAXSIZE];  // 用数组来模拟栈
    int top;            // 栈顶指针，指向下一个空位置
} IntStack;

// 初始化，top设成0就行
void int_stack_init(IntStack *s)
{
    s->top = 0;
}

// 判断栈是不是空的，top等于0就是空
int int_stack_empty(IntStack *s)
{
    return s->top == 0;
}

// 入栈：先把值放到top的位置，然后top再加1
int int_stack_push(IntStack *s, int val)
{
    if (s->top == MAXSIZE)  // 栈满了就放不进去了
        return 0;
    // top++是后置自增，所以是先用top当下标存值，然后top才+1
    s->data[s->top++] = val;
    return 1;
}

// 出栈：因为top指向的是空位置，所以要先减1才能拿到栈顶元素
int int_stack_pop(IntStack *s, int *val)
{
    if (int_stack_empty(s))  // 空栈没东西可弹
        return 0;
    // --s->top 先把top减1，然后再取那个位置的值
    *val = s->data[--s->top];
    return 1;
}

// 这个是存字符（运算符）用的栈，中缀转后缀的时候要用
// 和上面的整数栈逻辑一模一样，就是把int换成了char
typedef struct {
    char data[MAXSIZE];
    int top;
} CharStack;

void char_stack_init(CharStack *s)
{
    s->top = 0;
}

int char_stack_empty(CharStack *s)
{
    return s->top == 0;
}

int char_stack_push(CharStack *s, char c)
{
    if (s->top == MAXSIZE)
        return 0;
    s->data[s->top++] = c;
    return 1;
}

int char_stack_pop(CharStack *s, char *c)
{
    if (char_stack_empty(s))
        return 0;
    *c = s->data[--s->top];
    return 1;
}

// 看一下栈顶是什么，但不弹出来（只是偷看一眼）
char char_stack_top(CharStack *s)
{
    return s->data[s->top - 1];
}

// 给运算符定一个优先级，数字越大优先级越高
// 乘除是2，加减是1，左括号给0（在栈里它优先级最低，谁都能压过它）
int priority(char op)
{
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;  // 左括号走这里，返回0
}

// 中缀转后缀的核心函数（调度场算法）
// 思路：从左到右扫描中缀表达式，遇到数字直接输出，遇到运算符就用栈来调度顺序
void infix_to_postfix(const char *infix, char *postfix)
{
    CharStack op_stack;
    char_stack_init(&op_stack);

    int i = 0;   // i 是读infix的位置
    int j = 0;   // j 是写postfix的位置
    char c, top_op;

    while ((c = infix[i]) != '\0') {
        // 空格跳过不管
        if (c == ' ') {
            i++;
            continue;
        }

        // 遇到数字就直接写到输出里，注意可能是多位数比如123，要一直读到不是数字为止
        // 数字之间用空格隔开，不然分不清12和1 2
        if (isdigit(c)) {
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';  // 数字后面加个空格分隔
            continue;
        }

        // 左括号直接丢进栈里，等右括号来配对
        if (c == '(') {
            char_stack_push(&op_stack, c);
            i++;
            continue;
        }

        // 遇到右括号，就一直弹栈输出，直到碰到左括号为止
        // 左括号弹出来但是不输出（括号不出现在后缀表达式里）
        if (c == ')') {
            while (!char_stack_empty(&op_stack) && char_stack_top(&op_stack) != '(') {
                char_stack_pop(&op_stack, &top_op);
                postfix[j++] = top_op;
                postfix[j++] = ' ';
            }
            // 把左括号弹掉，扔了不要
            if (!char_stack_empty(&op_stack))
                char_stack_pop(&op_stack, &top_op);
            i++;
            continue;
        }

        // 遇到运算符：
        // 先看看栈顶有没有优先级>=自己的运算符，有的话先弹出来输出
        // 然后再把自己压进栈里
        // 这样就保证了优先级高的运算符先输出
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!char_stack_empty(&op_stack) &&
                   char_stack_top(&op_stack) != '(' &&
                   priority(char_stack_top(&op_stack)) >= priority(c)) {
                char_stack_pop(&op_stack, &top_op);
                postfix[j++] = top_op;
                postfix[j++] = ' ';
            }
            char_stack_push(&op_stack, c);
            i++;
            continue;
        }

        // 走到这说明遇到了不认识的字符
        printf("非法字符: '%c'\n", c);
        i++;
    }

    // 表达式扫完了，把栈里剩下的运算符全弹出来
    while (!char_stack_empty(&op_stack)) {
        char_stack_pop(&op_stack, &top_op);
        postfix[j++] = top_op;
        postfix[j++] = ' ';
    }

    // 把最后多出来的空格去掉，加上字符串结束符
    if (j > 0 && postfix[j - 1] == ' ')
        j--;
    postfix[j] = '\0';
}

// 后缀表达式求值
// 思路很简单：遇到数字就入栈，遇到运算符就弹两个数出来算，把结果再压回去
int eval_postfix(const char *postfix)
{
    IntStack stk;
    int_stack_init(&stk);

    int i = 0;
    char c;

    while ((c = postfix[i]) != '\0') {
        if (c == ' ') {
            i++;
            continue;
        }

        // 遇到数字，把字符转成真正的int数值再入栈
        // 比如 '1','2','3' 要变成 123
        if (isdigit(c)) {
            int num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');  // 字符转数字的经典写法
                i++;
            }
            int_stack_push(&stk, num);
            continue;
        }

        // 遇到运算符，弹两个数出来计算
        // 注意！先弹出来的是右操作数，后弹的是左操作数
        // 比如栈里是 3 2，算的是 3-2 不是 2-3，顺序不能搞反
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            int opd1, opd2, result;
            if (!int_stack_pop(&stk, &opd1) || !int_stack_pop(&stk, &opd2)) {
                printf("表达式语法错!\n");
                exit(1);
            }
            switch (c) {
            case '+': result = opd2 + opd1; break;
            case '-': result = opd2 - opd1; break;
            case '*': result = opd2 * opd1; break;
            case '/': result = opd2 / opd1; break;
            }
            // 算完把结果压回栈里继续参与后面的运算
            int_stack_push(&stk, result);
            i++;
            continue;
        }

        printf("后缀表达式中存在非法字符: '%c'\n", c);
        exit(1);
    }

    // 最后栈里应该就剩一个数，就是最终答案
    int result;
    if (!int_stack_pop(&stk, &result)) {
        printf("表达式语法错!\n");
        exit(1);
    }
    // 如果栈里还有多余的数，说明表达式写错了
    if (!int_stack_empty(&stk)) {
        printf("表达式语法错!\n");
        exit(1);
    }
    return result;
}

// 主要功能：输入一个中缀表达式，先转成后缀，再算出结果
void task_convert_and_eval(void)
{
    char infix[MAXSIZE * 4];
    char postfix[MAXSIZE * 4];

    printf("请输入中缀表达式: ");
    scanf("%s", infix);

    // 第一步：中缀转后缀
    infix_to_postfix(infix, postfix);
    printf("后缀表达式: %s\n", postfix);

    // 第二步：用后缀表达式算出结果
    int result = eval_postfix(postfix);
    printf("计算结果: %s = %d\n", postfix, result);
}

// 打印菜单
void menu(void)
{
    printf("\n===== 中缀转后缀并计算结果 =====\n");
    printf("  1. 输入中缀表达式并求值\n");
    printf("  0. 退出\n");
    printf("================================\n");
    printf("请选择功能: ");
}

// 主循环，一直跑直到用户选0退出
void run(void)
{
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);
        while (getchar() != '\n');  // 把scanf没读完的换行符吃掉

        if (choice == 0) {
            printf("程序结束。\n");
            break;
        }

        switch (choice) {
        case 1:
            task_convert_and_eval();
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
