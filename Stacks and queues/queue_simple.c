#include <stdio.h>
#include <stdbool.h>

// 块说明：定义循环队列容量（为区分队空和队满，实际最多存放 QUEUE_CAPACITY - 1 个元素）
#define QUEUE_CAPACITY 10

// 块说明：定义循环队列结构，front 指向队头，rear 指向队尾后一位置
typedef struct {
    int data[QUEUE_CAPACITY];
    int front;
    int rear;
} Queue;

// 块说明：初始化队列
void InitQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
    // 思想：通过“取模回绕”复用数组空间，实现循环存储
}

// 块说明：判断队列是否为空
bool IsEmpty(const Queue *q) {
    return q->front == q->rear;
}

// 块说明：判断队列是否为满（rear 的下一个位置等于 front）
bool IsFull(const Queue *q) {
    return (q->rear + 1) % QUEUE_CAPACITY == q->front;
}

// 块说明：入队操作
bool EnQueue(Queue *q, int value) {
    if (IsFull(q)) {
        return false;
    }

    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    return true;
}

// 块说明：出队操作
bool DeQueue(Queue *q, int *value) {
    if (IsEmpty(q)) {
        return false;
    }

    *value = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    return true;
}

// 块说明：输出队列中的所有元素（不改变队列）
void PrintQueue(const Queue *q) {
    int i = q->front;

    while (i != q->rear) {
        printf("%d ", q->data[i]);
        i = (i + 1) % QUEUE_CAPACITY;
    }
    printf("\n");
}

// 块说明：主函数测试
int main(void) {
    Queue q;
    int value;

    // 块说明：初始化队列并演示入队
    InitQueue(&q);
    for (int i = 1; i <= 9; i++) {
        if (EnQueue(&q, i * 10)) {
            printf("enqueue: %d\n", i * 10);
        }
    }

    // 块说明：输出当前队列
    printf("queue: ");
    PrintQueue(&q);

    // 块说明：演示出队
    if (DeQueue(&q, &value)) {
        printf("dequeue: %d\n", value);
    }
    if (DeQueue(&q, &value)) {
        printf("dequeue: %d\n", value);
    }

    // 块说明：继续入队，验证循环复用
    EnQueue(&q, 100);
    EnQueue(&q, 110);

    // 块说明：最终输出
    printf("queue: ");
    PrintQueue(&q);

    // 块说明：程序结束
    return 0;
}
