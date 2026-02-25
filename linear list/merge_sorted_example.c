#include <stdio.h>

// 块说明：定义结果数组最大容量，确保示例中的归并结果可以存下
#define MAXN 100

// 块说明：归并两个升序数组，并返回“元素间比较次数”
int merge_sorted(const int a[], int n, const int b[], int m, int c[]) {
    // 块说明：初始化三个指针，i/j 分别扫描两个输入数组，k 写入结果数组
    int i = 0, j = 0, k = 0;
    // 块说明：记录 a[i] 与 b[j] 的比较总次数
    int cmp = 0;

    // 块说明：主归并循环，每轮比较后把较小元素放入结果数组
    while (i < n && j < m) {
        cmp++;
        if (a[i] <= b[j]) {
            c[k++] = a[i++];
        } else {
            c[k++] = b[j++];
        }
    }
    // 思想：局部最优选择（每次取当前较小值）可保证整体有序

    // 块说明：收尾处理，直接复制尚未耗尽数组的剩余元素
    while (i < n) c[k++] = a[i++];
    while (j < m) c[k++] = b[j++];
    // 思想：比较只发生在双指针并行阶段，尾部复制不再比较

    // 块说明：返回比较次数，供复杂度分析使用
    return cmp;
}

// 块说明：打印数组内容，便于观察归并结果
void print_arr(const int x[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

// 块说明：演示“最小比较次数”场景，并输出对应复杂度结论
int main(void) {
    // 块说明：设置两个等长有序数组长度 n
    int n = 4;

    // 块说明：构造最小比较次数场景，a 的所有元素都小于 b 的所有元素
    int a[] = {1, 2, 3, 4};
    int b[] = {10, 20, 30, 40};
    int c[MAXN];
    // 思想：通过构造边界输入验证理论结论（最少比较次数为 n）

    // 块说明：执行归并并获取比较次数
    int cmp = merge_sorted(a, n, b, n, c);

    // 块说明：输出归并结果与最小时间复杂度分析
    printf("Merged(min case): ");
    print_arr(c, 2 * n);
    printf("Comparisons(min case) = %d\n", cmp);
    printf("Min comparisons formula: n = %d\n", n);
    printf("Time complexity in min case: O(n)\n");
    // 思想：即使比较次数最少，归并仍需线性写入结果数组，因此时间复杂度下界是线性

    return 0;
}
