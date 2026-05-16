#include <stdio.h>
#include <stdlib.h>

// 时间片大小，时间片用完作业没做完就重新排到队尾
#define QUANTUM 3


// 作业信息，remain 记录剩余时间，treat 保留原始时间用于算等待/周转
typedef struct {
    int id;
    int arrive;
    int treat;
    int remain;
} Job;

// 链式队列节点
typedef struct QNode {
    Job data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;   // 队头指针
    QNode *rear;    // 队尾指针
} Queue;

void queue_init(Queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}

int queue_empty(Queue *q)
{
    return q->front == NULL;
}

// 入队，新到达的作业和时间片用完的作业都走这里
void enqueue(Queue *q, Job job)
{
    QNode *node = (QNode *)malloc(sizeof(QNode));
    if (!node) {
        printf("内存分配失败\n");
        exit(-2);
    }
    node->data = job;
    node->next = NULL;

    if (queue_empty(q)) {
        // 空队列的时候 front 和 rear 都得指向这个新节点
        q->front = node;
        q->rear = node;
    } else {
        // 挂到尾巴后面，再把 rear 往后挪一格
        q->rear->next = node;
        q->rear = node;
    }
}

// 出队一个作业，用指针参数带出数据
int dequeue(Queue *q, Job *out)
{
    if (queue_empty(q))
        return 0;

    // 老规矩，先把头节点存一份，不然后面 free 就找不到了
    QNode *tmp = q->front;
    *out = tmp->data;

    q->front = tmp->next;
    if (q->front == NULL)
        q->rear = NULL;    // 队列空了 rear 也要置 NULL

    free(tmp);
    return 1;
}

void queue_destroy(Queue *q)
{
    Job j;
    while (dequeue(q, &j));
}


int main(void)
{
    char fname[128];
    FILE *fp;

    printf("enter file name:");
    scanf("%127s", fname);

    fp = fopen(fname, "r");
    if (!fp) {
        printf("cannot open file %s\n", fname);
        return 1;
    }

    // clock 当前时钟，idle 累计空闲时间，done 已完成作业数
    // 等待 = 周转 - 服务时间
    int clock = 0, idle = 0, done = 0;
    int total_wait = 0, total_turn = 0;

    Queue q;
    queue_init(&q);

    // temp 预读下一位作业，have==2 表示 temp 有效
    Job temp;
    int next_id = 1;
    int have = fscanf(fp, "%d %d", &temp.arrive, &temp.treat);
    if (have == 2) {
        temp.id = next_id++;
        temp.remain = temp.treat;
    }

    printf("\n调度轨迹 (QUANTUM=%d)：\n", QUANTUM);

    // 主循环，每轮处理一个时间片
    do {
        // 队列空但文件里还有作业，CPU 空转到下一个作业到达
        if (queue_empty(&q) && have == 2) {
            idle += temp.arrive - clock;
            clock = temp.arrive;
            enqueue(&q, temp);
            have = fscanf(fp, "%d %d", &temp.arrive, &temp.treat);
            if (have == 2) {
                temp.id = next_id++;
                temp.remain = temp.treat;
            }
        }

        // 从队头拉一个作业上 CPU
        Job curr;
        dequeue(&q, &curr);

        // 如果剩下的活不够一个时间片，那就按实际剩余时间跑，不然就跑满一片
        int run = (curr.remain < QUANTUM) ? curr.remain : QUANTUM;
        clock += run;
        curr.remain -= run;

        // 这一片跑的过程中新到的作业要先排进来
        // 不然一会 curr 没做完挂回队尾的时候，反而排到新来的前面了，就不公平了
        while (have == 2 && temp.arrive <= clock) {
            enqueue(&q, temp);
            have = fscanf(fp, "%d %d", &temp.arrive, &temp.treat);
            if (have == 2) {
                temp.id = next_id++;
                temp.remain = temp.treat;
            }
        }

        if (curr.remain == 0) {
            // 作业完成，结算周转和等待
            int turn = clock - curr.arrive;
            total_turn += turn;
            total_wait += turn - curr.treat;
            done++;
            printf("  t=%d  job%d  run=%d  完成 (周转=%d 等待=%d)\n",
                   clock, curr.id, run, turn, turn - curr.treat);
        } else {
            // 时间片到但没做完，挂回队尾
            printf("  t=%d  job%d  run=%d  剩余=%d  重新入队\n",
                   clock, curr.id, run, curr.remain);
            enqueue(&q, curr);
        }
    } while (!queue_empty(&q) || have == 2);

    fclose(fp);

    // 输出统计结果
    printf("\n===== 作业调度模拟结果 =====\n");
    printf("作业总数        : %d\n", done);
    printf("CPU 总空闲时间  : %d\n", idle);
    printf("总模拟时间      : %d\n", clock);
    if (done > 0) {
        printf("平均周转时间    : %.2f\n", (double)total_turn / done);
        printf("平均等待时间    : %.2f\n", (double)total_wait / done);
    }

    queue_destroy(&q);
    return 0;
}
