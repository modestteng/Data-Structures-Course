#include <stdio.h>
#include <string.h>

#define MaxSize 100
#define NumSize 64
#define NameSize 256

// 定义图书结点和顺序表。顺序表使用连续存储空间保存元素，便于按下标访问，也便于观察插入删除时的整体移动。
typedef struct
{
    char num[NumSize];
    char name[NameSize];
    float price;
} Book;

typedef struct
{
    Book elem[MaxSize];
    int length;
} SqList;

// 初始化顺序表。线性表初始化后长度为 0，表示当前还没有有效图书数据。
void init_list(SqList *list)
{
    list->length = 0;
}

// 判断图书数量是否在顺序表容量范围内。顺序表容量固定，这是顺序存储结构的典型特点。
int is_valid_count(int count)
{
    return count >= 0 && count <= MaxSize;
}

// 读入一本图书的信息。把重复输入过程单独封装，方便多个功能共享同一种输入格式。
void input_book(Book *book)
{
    scanf("%s%s%f", book->num, book->name, &book->price);
}

// 输出一本图书的信息。统一输出格式可以让所有功能的结果保持一致，便于调试和对照实验要求。
void print_book(Book *book)
{
    printf("%s %s %.2f\n", book->num, book->name, book->price);
}

// 读入以 0 0 0 结束的图书序列并建立顺序表。这对应顺序表最基础的建表操作。
void create_list(SqList *list)
{
    list->length = 0;
    while (list->length < MaxSize)
    {
        input_book(&list->elem[list->length]);
        if (strcmp(list->elem[list->length].num, "0") == 0 &&
            strcmp(list->elem[list->length].name, "0") == 0 &&
            list->elem[list->length].price == 0)
        {
            break;
        }
        list->length++;
    }
}

// 已知图书数量时顺序读入元素。按数量建表常用于逆序、查找等需要先完整读入数据的操作。
void create_list_by_count(SqList *list, int count)
{
    int i;

    list->length = count;
    for (i = 0; i < count; i++)
    {
        input_book(&list->elem[i]);
    }
}

// 顺序输出整个图书表。顺序表遍历的本质是下标从前到后的线性扫描。
void print_list(SqList *list)
{
    int i;

    for (i = 0; i < list->length; i++)
    {
        print_book(&list->elem[i]);
    }
}

// 计算所有图书的平均价格。顺序表的聚合统计通常通过一次遍历完成。
float average_price(SqList *list)
{
    int i;
    float sum = 0;

    if (list->length == 0)
    {
        return 0;
    }

    for (i = 0; i < list->length; i++)
    {
        sum = sum + list->elem[i].price;
    }

    return sum / list->length;
}

// 按实验要求修改所有图书价格。这里体现的是基于遍历的批量更新操作。
void change_price(SqList *list, float avg)
{
    int i;

    for (i = 0; i < list->length; i++)
    {
        if (list->elem[i].price < avg)
        {
            list->elem[i].price = list->elem[i].price * 1.2f;
        }
        else
        {
            list->elem[i].price = list->elem[i].price * 1.1f;
        }
    }
}

// 将图书按价格降序排序。这里使用简单选择排序，便于初学时观察交换过程。
void sort_list_by_price(SqList *list)
{
    int i;
    int j;
    int max_index;
    Book temp;

    for (i = 0; i < list->length - 1; i++)
    {
        max_index = i;
        for (j = i + 1; j < list->length; j++)
        {
            if (list->elem[j].price > list->elem[max_index].price)
            {
                max_index = j;
            }
        }
        if (max_index != i)
        {
            temp = list->elem[i];
            list->elem[i] = list->elem[max_index];
            list->elem[max_index] = temp;
        }
    }
}

// 原地逆序图书表。顺序表逆序常通过首尾元素对称交换来完成。
void reverse_list(SqList *list)
{
    int i;
    Book temp;

    for (i = 0; i < list->length / 2; i++)
    {
        temp = list->elem[i];
        list->elem[i] = list->elem[list->length - 1 - i];
        list->elem[list->length - 1 - i] = temp;
    }
}

// 查找最高价格。先找到关键字，再输出所有满足条件的图书，是顺序查找的常见写法。
float max_price(SqList *list)
{
    int i;
    float max;

    if (list->length == 0)
    {
        return 0;
    }

    max = list->elem[0].price;
    for (i = 1; i < list->length; i++)
    {
        if (list->elem[i].price > max)
        {
            max = list->elem[i].price;
        }
    }

    return max;
}

// 统计指定价格的图书数量。先计数再输出，是处理多重匹配结果时比较清晰的办法。
int count_books_by_price(SqList *list, float price)
{
    int i;
    int count = 0;

    for (i = 0; i < list->length; i++)
    {
        if (list->elem[i].price == price)
        {
            count++;
        }
    }

    return count;
}

// 输出所有价格等于指定值的图书。顺序表中相同关键字的所有匹配项需要逐个扫描。
void print_books_by_price(SqList *list, float price)
{
    int i;

    for (i = 0; i < list->length; i++)
    {
        if (list->elem[i].price == price)
        {
            print_book(&list->elem[i]);
        }
    }
}

// 统计指定书名的图书数量。按关键字顺序查找是顺序表最基本的检索方式之一。
int count_books_by_name(SqList *list, char name[])
{
    int i;
    int count = 0;

    for (i = 0; i < list->length; i++)
    {
        if (strcmp(list->elem[i].name, name) == 0)
        {
            count++;
        }
    }

    return count;
}

// 输出所有书名匹配的图书。这里演示的是顺序表的多结果查找。
void print_books_by_name(SqList *list, char name[])
{
    int i;

    for (i = 0; i < list->length; i++)
    {
        if (strcmp(list->elem[i].name, name) == 0)
        {
            print_book(&list->elem[i]);
        }
    }
}

// 按 1 开始的位置查找图书。顺序表支持 O(1) 级别的按下标访问，这是它的重要优势。
int locate_book_by_position(SqList *list, int position, Book *book)
{
    if (position < 1 || position > list->length)
    {
        return 0;
    }

    *book = list->elem[position - 1];
    return 1;
}

// 功能1，创建并输出图书表。这个功能对应顺序表最基础的建表与遍历。
void run_create_and_print(SqList *list)
{
    printf("\n【功能1：创建并输出图书信息表】\n");
    printf("请输入图书信息，格式：书号 书名 价格\n");
    printf("输入 0 0 0 表示结束输入。\n");
    create_list(list);
    printf("图书数量：%d\n", list->length);
    printf("图书信息如下：\n");
    print_list(list);
    printf("\n");
}

// 功能2，按价格降序排序并输出。排序后可以直观看到顺序表中元素位置的变化。
void run_sort_books(SqList *list)
{
    printf("\n【功能2：按价格降序排序】\n");
    printf("请输入图书信息，格式：书号 书名 价格\n");
    printf("输入 0 0 0 表示结束输入。\n");
    create_list(list);
    sort_list_by_price(list);
    printf("排序后的图书信息如下：\n");
    print_list(list);
    printf("\n");
}

// 功能3，求平均价格并批量修改。它体现了顺序表一次扫描统计、一次扫描更新的思路。
void run_change_price(SqList *list)
{
    float avg;

    printf("\n【功能3：修改图书价格】\n");
    printf("请输入图书信息，格式：书号 书名 价格\n");
    printf("输入 0 0 0 表示结束输入。\n");
    create_list(list);
    avg = average_price(list);
    change_price(list, avg);
    printf("平均价格：%.2f\n", avg);
    printf("修改后的图书信息如下：\n");
    print_list(list);
    printf("\n");
}

// 功能4，按输入顺序建立图书表后完成逆序。这里演示的是对称交换的原地处理方式。
void run_reverse_storage(SqList *list)
{
    int count;

    printf("\n【功能4：逆序存储图书信息表】\n");
    printf("请输入图书数量：");
    scanf("%d", &count);
    if (!is_valid_count(count))
    {
        printf("图书数量超出顺序表范围。\n\n");
        return;
    }
    printf("请依次输入每本图书的信息，格式：书号 书名 价格\n");
    create_list_by_count(list, count);
    reverse_list(list);
    printf("逆序后的图书信息如下：\n");
    print_list(list);
    printf("\n");
}

// 功能5，找出最贵的图书。先求最大值，再输出所有最大值对应的记录。
void run_find_most_expensive(SqList *list)
{
    int count;
    int expensive_count;
    float price;

    printf("\n【功能5：查找最贵图书】\n");
    printf("请输入图书数量：");
    scanf("%d", &count);
    if (!is_valid_count(count))
    {
        printf("图书数量超出顺序表范围。\n\n");
        return;
    }
    printf("请依次输入每本图书的信息，格式：书号 书名 价格\n");
    create_list_by_count(list, count);
    price = max_price(list);
    expensive_count = count_books_by_price(list, price);
    printf("最贵图书数量：%d\n", expensive_count);
    printf("最贵图书信息如下：\n");
    print_books_by_price(list, price);
    printf("\n");
}

// 功能6，按书名查找最爱图书。顺序表不具备按关键字的直接定位能力，所以需要逐项比较。
void run_find_favorite(SqList *list)
{
    int count;
    int query_count;
    int i;
    int match_count;
    char target_name[NameSize];

    printf("\n【功能6：查找最爱图书】\n");
    printf("请输入图书数量：");
    scanf("%d", &count);
    if (!is_valid_count(count))
    {
        printf("图书数量超出顺序表范围。\n\n");
        return;
    }
    printf("请依次输入每本图书的信息，格式：书号 书名 价格\n");
    create_list_by_count(list, count);
    printf("请输入要查找的次数：");
    scanf("%d", &query_count);
    printf("请依次输入每次要查找的书名：\n");

    for (i = 0; i < query_count; i++)
    {
        scanf("%s", target_name);
        match_count = count_books_by_name(list, target_name);
        printf("第%d次查找结果：\n", i + 1);
        printf("%d\n", match_count);
        if (match_count == 0)
        {
            printf("抱歉，没有你的最爱！\n");
        }
        else
        {
            print_books_by_name(list, target_name);
        }
    }
    printf("\n");
}

// 功能7，按位置查找图书。顺序表最适合这类按序号直接访问的操作。
void run_find_by_position(SqList *list)
{
    int count;
    int query_count;
    int i;
    int position;
    Book book;

    printf("\n【功能7：按位置查找图书】\n");
    printf("请输入图书数量：");
    scanf("%d", &count);
    if (!is_valid_count(count))
    {
        printf("图书数量超出顺序表范围。\n\n");
        return;
    }
    printf("请依次输入每本图书的信息，格式：书号 书名 价格\n");
    create_list_by_count(list, count);
    printf("请输入要查找的次数：");
    scanf("%d", &query_count);
    printf("请依次输入每次要查找的位置序号：\n");

    for (i = 0; i < query_count; i++)
    {
        scanf("%d", &position);
        if (locate_book_by_position(list, position, &book))
        {
            print_book(&book);
        }
        else
        {
            printf("抱歉，最佳位置上的图书不存在！\n");
        }
    }
    printf("\n");
}

// 输出实验2菜单。把前 7 个顺序表操作集中到统一入口，便于分功能练习和验证。
void print_menu(void)
{
    printf("---------------- 实验2菜单 ----------------\n");
    printf("1. 创建并输出图书信息表\n");
    printf("2. 按价格降序排序\n");
    printf("3. 修改图书价格\n");
    printf("4. 逆序存储图书信息表\n");
    printf("5. 查找最贵图书\n");
    printf("6. 查找最爱图书\n");
    printf("7. 按位置查找图书\n");
    printf("0. 结束程序\n");
    printf("------------------------------------------\n");
}

int main(void)
{
    SqList list;
    int choice;

    // 初始化主程序使用的顺序表，并通过循环菜单分派不同功能。
    init_list(&list);

    do
    {
        print_menu();
        printf("请输入功能编号：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            run_create_and_print(&list);
            break;
        case 2:
            run_sort_books(&list);
            break;
        case 3:
            run_change_price(&list);
            break;
        case 4:
            run_reverse_storage(&list);
            break;
        case 5:
            run_find_most_expensive(&list);
            break;
        case 6:
            run_find_favorite(&list);
            break;
        case 7:
            run_find_by_position(&list);
            break;
        case 0:
            printf("程序结束。\n");
            break;
        default:
            printf("输入的功能编号无效。\n");
            break;
        }
    } while (choice != 0);

    return 0;
}
