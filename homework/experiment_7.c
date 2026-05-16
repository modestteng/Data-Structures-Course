#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TIME 60  // 模拟总时长：60分钟


// 队列节点：每个节点代表一个病人
typedef struct QNode {
    int arrive_time;    // 到达时间（分钟）
    int treat_time;     // 所需处理时间（分钟）
    struct QNode *next;
} QNode;

//链表是头指针进行维护
//队列是这个头指针和尾指针进行维护，所以后者要再设置一个结构体队列
// 链式队列：用front和rear指针实现先进先出
typedef struct {
    QNode *front;  // 队头指针，出队从这里取
    QNode *rear;   // 队尾指针，入队从这里加
} Queue;

// 初始化队列，front和rear都指向NULL表示空队列
void queue_init(Queue *q)
{
    q->front = NULL;
    q->rear = NULL;
}

// 判断队列是否为空
int queue_empty(Queue *q)
{
    return q->front == NULL;
}

// 入队：在队尾插入新节点（病人排到队伍最后面）
void enqueue(Queue *q, int arrive, int treat)
{
    QNode *node = (QNode *)malloc(sizeof(QNode));
    node->arrive_time = arrive;
    node->treat_time = treat;
    node->next = NULL;
//两种情况的考量
    if (queue_empty(q)) {
        // 空队列时，front和rear都指向新节点
        q->front = node;
        q->rear = node;
    } else {
        // 非空时，新节点接到队尾后面，然后更新rear
        q->rear->next = node;
        q->rear = node;
    }
}
//医生空闲了，就取出一个病人
// 出队：从队头取出一个病人，返回1成功，0失败
//通过这种方式一下子间接返回两个值，这才真是精彩，通过指针的方式
//因为指针指向的空间的内容发生了这个改变，真是精彩
int dequeue(Queue *q, int *arrive, int *treat)
{
    if (queue_empty(q))
        return 0;
//这种操作才是真的有意思，总是先备份
    QNode *tmp = q->front;
    *arrive = tmp->arrive_time;
    *treat = tmp->treat_time;

    // front指向下一个节点，如果队列变空了rear也要置NULL
    //真是精彩的健壮性考量
    q->front = tmp->next;
    if (q->front == NULL)
        q->rear = NULL;

    free(tmp);
    return 1;
}

// 释放队列中剩余的所有节点，防止内存泄漏
void queue_destroy(Queue *q)
{
    int a, t;
    while (dequeue(q, &a, &t));
}

// 随机生成病人数据，写入data.txt
// 返回实际生成的病人数量
int generate_data(int arrivals[], int treats[], int max_patients)
{
    int count = 0;
    int current_time = 0;
//用两个这个数组来存储这个人的到达时间和处理时间，真是精彩的设计，并且是对应的
    // 在60分钟内随机生成病人
    // 每次随机间隔1~15分钟来一个病人，直到超过60分钟
    while (current_time < MAX_TIME && count < max_patients) {
        int gap = rand() % 15 + 1;  // 间隔1~15分钟，从零开始数的
        current_time += gap;
        if (current_time >= MAX_TIME)
            break;
// 记录病人到达时间和处理时间
        arrivals[count] = current_time;
        treats[count] = rand() % 9 + 1;  // 处理时间1~9分钟
        count++;
    }

    return count;
}

// 将病人数据写入data.txt
void write_data(const char *filename, int arrivals[], int treats[], int count)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    // 第一行：病人总数
    fprintf(fp, "%d\n", count);

    // 后续每行：到达时间  处理时间
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d\t%d\n", arrivals[i], treats[i]);
    }

    fclose(fp);
    printf("病人数据已写入 %s\n", filename);
}

// 模拟就诊过程并追加结果到文件
// 核心思路：逐分钟推进时间，病人到达时入队，医生空闲时从队头取病人处理
void simulate(const char *filename, int arrivals[], int treats[], int count)
{
    Queue q;
    queue_init(&q);

    int doctor_idle = 0;       // 医生在60分钟内的总空闲时间
    int total_wait = 0;        // 所有病人的等待时间之和
    int doctor_free_at = 0;    // 医生完成当前病人的时刻
    int patient_idx = 0;       // 下一个将到达的病人编号

    // 离散时间模拟：以1分钟为最小粒度推进整个就诊过程
    // 每次循环代表"又过了一分钟"，在这一分钟里检查病人到达情况和医生状态
    // 这是本程序把"连续时间"转换成"可计算流程"的核心手法
    for (int t = 0; t < MAX_TIME; t++) {
        // 把到达时刻正好等于 t 的病人全部入队
        // 用 while 而不是 if，是为了处理"同一分钟来多个病人"的情况
        // patient_idx 单调递增，保证每个病人只会被入队一次
        while (patient_idx < count && arrivals[patient_idx] == t) {
            enqueue(&q, arrivals[patient_idx], treats[patient_idx]);
            patient_idx++;
        }

        // 医生状态分支：t >= doctor_free_at 表示上一位病人已经看完，医生空闲
        // 空闲时又分两种子情况——队列里有人就叫号看病，没人就记一分钟空闲
        // 这里正是"医生节奏 vs 病人节奏"不匹配时，空闲时间和等待时间此消彼长的核心
        if (t >= doctor_free_at) {
            if (!queue_empty(&q)) {
                int arr, trt;
                dequeue(&q, &arr, &trt);

                // 等待时间 = 开始看病时刻 - 病人到达时刻
                total_wait += (t - arr);

                // 医生开始处理，更新完成时刻
                doctor_free_at = t + trt;
            } else {
                // 队列为空，医生这一分钟在空闲
                doctor_idle++;
            }
        }
        // 医生正在忙（t < doctor_free_at），这一分钟不做处理
    }

    // 60分钟到了，但队列里可能还有没看完的病人，继续处理
    // 把剩余病人也入队（虽然按题意所有病人都在60分钟内到达，这里做兜底）
    while (patient_idx < count) {
        enqueue(&q, arrivals[patient_idx], treats[patient_idx]);
        patient_idx++;
    }

    // 处理队列中剩余的病人（超出60分钟的部分不算医生空闲）
    while (!queue_empty(&q)) {
        int arr, trt;
        dequeue(&q, &arr, &trt);

        // 开始时刻取 doctor_free_at（此时肯定 >= MAX_TIME 或之前的值）
        int start = doctor_free_at;
        total_wait += (start - arr);
        doctor_free_at = start + trt;
    }

    double avg_wait = (count > 0) ? (double)total_wait / count : 0.0;

    // 打印结果到控制台
    printf("\n===== 模拟结果 =====\n");
    printf("1小时内共有 %d 位病人来看病。\n", count);
    printf("医生的总空余时间为 %d 分钟。\n", doctor_idle);
    printf("病人的平均等待时间为 %.2f 分钟。\n", avg_wait);

    // 追加结果到data.txt
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    fprintf(fp, "\n1小时内共有 %d 位病人来看病。\n", count);
    fprintf(fp, "医生的总空余时间为 %d 分钟。\n", doctor_idle);
    fprintf(fp, "病人的平均等待时间为 %.2f 分钟。\n", avg_wait);
    fclose(fp);

    printf("结果已追加到 %s\n", filename);
}

int main(void)
{
    srand((unsigned)time(NULL));

    int arrivals[100];  // 最多100个病人
    int treats[100];

    // 生成随机病人数据
    int count = generate_data(arrivals, treats, 100);

    // 写入data.txt
    write_data("data.txt", arrivals, treats, count);

    // 模拟就诊并追加结果
    simulate("data.txt", arrivals, treats, count);

    return 0;
}
