#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1024


// 题目给的字母映射表，a~z 对应下面一串
// a b c d e f g h i j k l m n o p q r s t u v w x y z
// n g z q t c o b m u h e l k p d a w x f y i v r s j
static const char encrypt_map[26] = {
    'n','g','z','q','t','c','o','b','m','u','h','e','l',
    'k','p','d','a','w','x','f','y','i','v','r','s','j'
};

// 解密表由加密表反推，程序跑起来的时候建一次就行
static char decrypt_map[26];

// 把加密表反过来存一份，后面解密直接查
// 要是每次解密都现扫一遍加密表就太亏了
void build_decrypt_map(void)
{
    for (int i = 0; i < 26; i++) {
        int idx = encrypt_map[i] - 'a';
        decrypt_map[idx] = (char)('a' + i);
    }
}

// 加密：一个字符一个字符地扫，字母就查表，别的照抄
// 串说白了就是一串字符，遍历就是按下标往后走
void encrypt(const char *plain, char *cipher)
{
    int len = (int)strlen(plain);
    for (int i = 0; i < len; i++) {
        char c = plain[i];
        if (c >= 'a' && c <= 'z') {
            cipher[i] = encrypt_map[c - 'a'];
        } else if (c >= 'A' && c <= 'Z') {
            // 大写的先当小写查表，再把结果变回大写，这样大小写不会丢
            cipher[i] = (char)toupper((unsigned char)encrypt_map[c - 'A']);
        } else {
            // 空格、标点、数字这些不是字母的就原样放进去
            cipher[i] = c;
        }
    }
    cipher[len] = '\0';  // 别忘了 '\0'，C 里的串就靠它来收尾
}

// 解密逻辑和加密几乎一模一样，就是换张表查
void decrypt(const char *cipher, char *plain)
{
    int len = (int)strlen(cipher);
    for (int i = 0; i < len; i++) {
        char c = cipher[i];
        if (c >= 'a' && c <= 'z') {
            plain[i] = decrypt_map[c - 'a'];
        } else if (c >= 'A' && c <= 'Z') {
            plain[i] = (char)toupper((unsigned char)decrypt_map[c - 'A']);
        } else {
            plain[i] = c;
        }
    }
    plain[len] = '\0';
}

// 顺手把映射表打出来看一眼，方便核对
void print_map(void)
{
    printf("字母映射表（明文 -> 密文）：\n");
    printf("  明文: ");
    for (int i = 0; i < 26; i++) printf("%c ", 'a' + i);
    printf("\n  密文: ");
    for (int i = 0; i < 26; i++) printf("%c ", encrypt_map[i]);
    printf("\n\n");
}


int main(void)
{
    char plain[MAX_LEN];
    char cipher[MAX_LEN];
    char recovered[MAX_LEN];

    build_decrypt_map();
    print_map();

    // 用 fgets 是想让输入可以带空格，scanf("%s") 碰到空格就停了
    printf("请输入要加密的文本：");
    if (fgets(plain, MAX_LEN, stdin) == NULL) {
        printf("读取输入失败\n");
        return 1;
    }

    // fgets 会把回车也读进来，这里把末尾那个换行去掉
    size_t n = strlen(plain);
    if (n > 0 && plain[n - 1] == '\n') plain[n - 1] = '\0';

    encrypt(plain, cipher);
    printf("加密结果：%s\n", cipher);

    // 再解回去一遍，顺带验证一下加密和解密互为逆过程
    decrypt(cipher, recovered);
    printf("解密结果：%s\n", recovered);

    if (strcmp(plain, recovered) == 0) {
        printf("校验通过：解密结果与原文一致。\n");
    } else {
        printf("校验失败：解密结果与原文不一致。\n");
    }

    return 0;
}
