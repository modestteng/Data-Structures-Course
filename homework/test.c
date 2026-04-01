#include <stdio.h>
#include <stdlib.h>

// 错误：传的是指针的值（副本）
void alloc_wrong(int *p) {
     // 函数只改了自己的局部副本 p
    *p = 100;
    printf("函数内部：p指向的地址是 %p, 值是 %d\n", p, *p);
}

int main() {
    int *my_ptr = (int *)malloc(sizeof(int));; 
    alloc_wrong(my_ptr); 
    
    if (my_ptr == NULL) {
        printf("主函数：my_ptr 依然是 NULL！分配失败。\n");
    }
    return 0;
}
