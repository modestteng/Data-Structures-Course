#include <stdio.h>
#include <string.h>

int main(void) {
    // 块说明：setup，定义并初始化一个字符数组，作为最基础的串存储方式（以 '\0' 结尾）。
    char text[50] = "Data";
    printf("Initial string: %s\n", text);

    // 块说明：core logic，演示串的基本操作：长度、拼接、比较。
    printf("Length: %zu\n", strlen(text));
    strcat(text, " Structure");
    printf("After concat: %s\n", text);
    printf("Compare with \"Data Structure\": %d\n", strcmp(text, "Data Structure"));
    // 思想：串本质是特殊的字符数组，掌握标准库函数可以高效完成常见文本处理。

    // 块说明：edge handling，演示查找字符是否存在。
    char target = 'S';
    char *pos = strchr(text, target);
    if (pos != NULL) {
        printf("Character '%c' found at index: %ld\n", target, (long)(pos - text));
    } else {
        printf("Character '%c' not found\n", target);
    }

    // 块说明：output，输出最终串内容，帮助观察各操作结果。
    printf("Final string: %s\n", text);

    // 块说明：cleanup，当前示例只使用栈上数组，无需手动释放资源。
    return 0;
}
