#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 单链表结点定义，每个结点存放大整数的一位(0-9) */
typedef struct node {
    int data;
    struct node *next;
} NodeType;

/* 将整数字符串转换为带头结点的单链表（尾插法，保持原始高位->低位顺序）
   a[] 为数字字符串，n 为字符串长度 */

   //要想修改这个一级指针的内容，我们需要传入这个二级指针，这是由于这个函数传参拷贝副本所导致的
void CreateLink(NodeType **h, char a[], int n) {
    *h = (NodeType *)malloc(sizeof(NodeType));
    (*h)->data = 0;
    (*h)->next = NULL;

    NodeType *tail = *h;
    for (int i = 0; i < n; i++) {
        NodeType *p = (NodeType *)malloc(sizeof(NodeType));
        // 数字转字符的操作
        p->data = a[i] - '0';
        //创建新节点的指针要置空
        p->next = NULL;
        //相当于是初始化这个新节点的指针域之后，将这个新节点连接到链表的尾部
        tail->next = p;
        tail = p;
    }
}

/* 释放单链表所有结点（含头结点） */
//想要操作这个一级指针的内容。必须要传入这个二级指针进行操作，这是函数的参数传递机制导致的
void DestroyLink(NodeType **h) {
    NodeType *p = *h;
    while (p) {
        //这个备份的思想很重要
        NodeType *q = p->next;
        free(p);
        p = q;
    }
    *h = NULL;
}

/* 输出链表中存储的大整数 */
void DispLink(NodeType *h) {
    //这难道是涂老师说的这个维护这个头指针，定义一个新的指针来遍历链表的思想吗
    NodeType *p = h->next;
    while (p) {
        printf("%d", p->data);
        p = p->next;
    }
    printf("\n");
}

/* 逆置带头结点的单链表（头插法，头结点不动） */
void Reverse(NodeType **h) {
    NodeType *p = (*h)->next;
    //如果不将这个头结点置空，那么在逆置的过程中就会形成一个环，导致死循环
    //这个环是最后一个节点自己和自己构成了一个环
    (*h)->next = NULL;

    while (p) {
        NodeType *q = p->next;
        p->next = (*h)->next;
        (*h)->next = p;
        p = q;
    }
}

/* 两个大整数单链表相加，结果存入新链表 h
   算法：先逆置两链表使低位在前，逐位相加处理进位，结果再逆置恢复高位在前 */
void Add(NodeType *h1, NodeType *h2, NodeType **h) {
    /* 逆置两个操作数链表，方便从低位开始相加 */
    Reverse(&h1);
    Reverse(&h2);

    /* 创建结果链表的头结点 */
    //你应该将这个*h看做是这个解引用，这样更好理解，就是这个第一层这个解引用
    *h = (NodeType *)malloc(sizeof(NodeType));
    (*h)->data = 0;
    (*h)->next = NULL;
    NodeType *tail = *h;

    NodeType *p1 = h1->next;
    NodeType *p2 = h2->next;
    //进位
    int carry = 0;

    /* 逐位相加，任一链表未遍历完或有进位则继续 */
    while (p1 || p2 || carry) {
        //真是精彩的代码，定义这个进位，定义这两个指针，分别为这两个链表的当前结点，定义这个sum来存储当前位的和
        int sum = carry;
        if (p1) {
            sum += p1->data;
            p1 = p1->next;
        }
        if (p2) {
            sum += p2->data;
            p2 = p2->next;
        }
        carry = sum / 10;

        NodeType *node = (NodeType *)malloc(sizeof(NodeType));
        // 取余取整存入数据，尾巴后移
        node->data = sum % 10;
        node->next = NULL;
        tail->next = node;
        tail = node;
    }

    /* 结果当前是低位在前，逆置回高位在前 */
    Reverse(h);

    /* 恢复原始链表的顺序 */
    Reverse(&h1);
    Reverse(&h2);
}

/* 快慢指针法查找链表中间位并输出
   奇数长度返回正中间，偶数长度返回第 n/2 位（中间偏左） */
void Mid(NodeType *h) {
    NodeType *fast = h->next;
    NodeType *slow = h->next;

    /* fast 每次走两步，slow 每次走一步
       当 fast 到达末尾时 slow 恰好在中间 */
       //真是精彩，通过这个快慢指针的两倍关系，来找到这个中间数值，判断条件是，其实是
       //这个快指针走两步不为空即可，其实不对，必须要对这个走一步和走两步都进行这个判断
       //因为这个可能导致报错，当这个是奇数个节点的时候
       //fast-next为NULL，而fast-next-next为报错
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    printf("%d\n", slow->data);
}

// Block: 功能1 —— 输入大整数，创建链表并输出
void task_create_and_print(void) {
    char str[1002];
    printf("请输入一个大整数: ");
    scanf("%s", str);

    // Idea: 负号单独处理，只把数字部分存入链表
    int neg = 0;
    char *digits = str;
    if (str[0] == '-') {
        neg = 1;
        digits = str + 1;
    }

    NodeType *h = NULL;
    CreateLink(&h, digits, (int)strlen(digits));

    if (neg) printf("-");
    DispLink(h);
    DestroyLink(&h);
}

// Block: 功能2 —— 输入两个大整数，相加并输出结果
void task_add(void) {
    char str1[1002], str2[1002];
    printf("请输入第一个大整数: ");
    scanf("%s", str1);
    printf("请输入第二个大整数: ");
    scanf("%s", str2);

    NodeType *h1 = NULL, *h2 = NULL, *result = NULL;
    CreateLink(&h1, str1, (int)strlen(str1));
    CreateLink(&h2, str2, (int)strlen(str2));

    Add(h1, h2, &result);

    printf("相加结果: ");
    DispLink(result);

    DestroyLink(&h1);
    DestroyLink(&h2);
    DestroyLink(&result);
}

// Block: 功能3 —— 查找大整数链表的中间位
void task_find_mid(void) {
    char str[1002];
    printf("请输入一个大整数: ");
    scanf("%s", str);

    NodeType *h = NULL;
    CreateLink(&h, str, (int)strlen(str));

    printf("中间位数字: ");
    Mid(h);

    DestroyLink(&h);
}

void menu(void) {
    printf("\n========== 大整数单链表运算 ==========\n");
    printf("  1. 大整数创建与输出\n");
    printf("  2. 两个大整数相加\n");
    printf("  3. 查找中间位数字\n");
    printf("  0. 退出\n");
    printf("======================================\n");
    printf("请选择功能: ");
}

int main(void) {
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);

        if (choice == 0) {
            printf("程序结束。\n");
            break;
        }
        //低耦合高内聚，功能模块化，代码清晰易读，维护性好
        switch (choice) {
        case 1: task_create_and_print(); break;
        case 2: task_add();              break;
        case 3: task_find_mid();         break;
        default: printf("无效选项，请重新选择。\n"); break;
        }
    }

    return 0;
}
