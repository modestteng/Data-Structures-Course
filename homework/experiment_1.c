#include <stdio.h>
#include <string.h>

#define MaxSize 100
#define NumSize 64
#define NameSize 256

// 图书结点定义。每个结点保存一本书的书号、书名和价格。
typedef struct
{
    char num[NumSize];
    char name[NameSize];
    float price;
} Book;

// 顺序表定义。顺序表用数组保存数据，length 表示当前图书数量。
typedef struct
{
    Book elem[MaxSize];
    int length;
} SqList;

// 初始化顺序表。
void init_list(SqList *list)
{
    list->length = 0;
}

// 功能1。输入图书信息，遇到 0 0 0 结束，建立图书信息表。
void create_list(SqList *list)
{
    list->length = 0;
    while (1)
    {
        scanf("%s%s%f", list->elem[list->length].num, list->elem[list->length].name, &list->elem[list->length].price);
        if (strcmp(list->elem[list->length].num, "0") == 0 && strcmp(list->elem[list->length].name, "0") == 0 && list->elem[list->length].price == 0)
        {
            break;
        }
        list->length++;
    }
}

// 功能4输入。先输入图书数量，再顺序输入每本图书的信息。
void create_list_by_count(SqList *list, int count)
{
    int i;

    list->length = count;
    for (i = 0; i < count; i++)
    {
        scanf("%s%s%f", list->elem[i].num, list->elem[i].name, &list->elem[i].price);
    }
}

// 功能2。按顺序输出顺序表中的图书信息。
void print_list(SqList *list)
{
    int i;

    for (i = 0; i < list->length; i++)
    {
        printf("%s %s %.2f\n", list->elem[i].num, list->elem[i].name, list->elem[i].price);
    }
}

// 功能3。先求所有图书的平均价格。
float average_price(SqList *list)
{
    int i;
    float sum = 0;

    for (i = 0; i < list->length; i++)
    {
        sum = sum + list->elem[i].price;
    }

    return sum / list->length;
}

// 功能3。根据平均价格修改每本图书的价格。
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

// 功能4：将最前面的元素和最后面的元素交换，借助中间变量完成逆序。
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

// 功能1和功能2组合。先建立顺序表，再输出图书数量和图书信息。
void run_create_and_print(SqList *list)
{
    printf("\n【功能1-2：创建并输出图书信息表】\n");
    printf("请输入图书信息，格式：书号 书名 价格\n");
    printf("输入 0 0 0 表示结束输入。\n");
    create_list(list);
    printf("图书数量：%d\n", list->length);
    printf("图书信息如下：\n");
    print_list(list);
    printf("\n");
}

// 功能3组合。先建立顺序表，再求平均价格、修改价格并输出结果。
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

// 功能4组合。先读入图书数量并建立顺序表，再完成逆序和输出。
void run_reverse_storage(SqList *list)
{
    int count;

    printf("\n【功能4：逆序存储图书信息表】\n");
    printf("请输入图书数量：");
    scanf("%d", &count);
    printf("请依次输入每本图书的信息，格式：书号 书名 价格\n");
    create_list_by_count(list, count);
    reverse_list(list);
    printf("逆序后的图书信息如下：\n");
    print_list(list);
    printf("\n");
}

// 菜单输出。
void print_menu(void)
{
    printf("---------------- 菜单 ----------------\n");
    printf("1. 功能1-2：创建并输出图书信息表\n");
    printf("2. 功能3：修改图书价格\n");
    printf("3. 功能4：逆序存储图书信息表\n");
    printf("0. 结束程序\n");
    printf("--------------------------------------\n");
}

int main(void)
{
    SqList list;
    int choice;

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
            run_change_price(&list);
            break;
        case 3:
            run_reverse_storage(&list);
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
