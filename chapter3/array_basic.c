#include <stdio.h>

int main(void) {
    // 块说明：setup，定义并初始化一个一维整型数组，表示连续存储结构。
    int numbers[5] = {10, 20, 30, 40, 50};
    int size = (int)(sizeof(numbers) / sizeof(numbers[0]));
    printf("Array size: %d\n", size);

    // 块说明：core logic，演示数组的遍历、随机访问、元素修改。
    printf("Traverse array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    printf("Element at index 2: %d\n", numbers[2]);
    numbers[2] = 99;
    printf("After update index 2: %d\n", numbers[2]);
    // 思想：数组的优势是按下标 O(1) 访问，代价是长度固定、插入删除不灵活。

    // 块说明：edge handling，进行简单下标边界判断，避免越界访问。
    int index = 6;
    if (index >= 0 && index < size) {
        printf("Safe access numbers[%d] = %d\n", index, numbers[index]);
    } else {
        printf("Index %d out of range [0, %d)\n", index, size);
    }

    // 块说明：output，输出修改后的完整数组，确认数据状态。
    printf("Final array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    // 块说明：cleanup，当前示例未动态申请内存，无需手动释放。
    return 0;
}
