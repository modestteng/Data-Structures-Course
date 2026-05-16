#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 实验10：字符串操作演示系统 */

typedef char *STRING;

/* 串头表：串名数组与串首地址数组平行存放 */
struct strheadlist {
    STRING strhead[100];
    STRING strname[100];
    int curnum;
};

/* 一条命令经过分析后得到的参数信息 */
struct result {
    int num;
    int nametype[6];
    STRING str[6];
};

struct strheadlist head;
char cmd[40][80];

/* 把数字字符串转成整数 */
int strtoint(STRING s) {
    int i = 0;
    int total = 0;
    while (s[i] != '\0') {
        total = total * 10 + s[i] - 48;
        i++;
    }
    return total;
}

/* 按串名在串头表里查，找到返回下标，找不到返回 -1 */
int lookup(STRING s) {
    int i;
    int look = -1;
    for (i = 0; i < head.curnum; i++) {
        if (strcmp(head.strname[i], s) == 0) {
            look = i;
            break;
        }
    }
    if (look == head.curnum) {
        look = -1;
    }
    return look;
}

/* 命令行分析：用空格切分参数，顺便根据首字符判断参数类型
   1=命令动词  2=字符串常量（单引号包裹）  3=整数  4=串名 */
struct result cmdsyna(int row) {
    int i;
    struct result res;
    char c;
    int word = 0, start;
    res.num = -1;
    for (i = 0; (c = cmd[row][i]) != '\0'; i++) {
        if (c == ' ') {
            word = 0;
            cmd[row][i] = '\0';
        } else if (word == 0) {
            res.num++;
            start = i;
            word = 1;
            res.str[res.num] = &cmd[row][start];
            if (res.num == 0) {
                res.nametype[res.num] = 1;
            } else if (cmd[row][start] == 39) {
                /* 去掉首个单引号，让指针指向实际字符串内容 */
                cmd[row][start] = '\0';
                res.str[res.num] = &cmd[row][start + 1];
                res.nametype[res.num] = 2;
            } else {
                if (cmd[row][start] >= '0' && cmd[row][start] <= '9') {
                    res.nametype[res.num] = 3;
                } else {
                    res.nametype[res.num] = 4;
                }
            }
        } else {
            /* 碰到闭合单引号或整数常量里的非数字，就地截断 */
            if (res.nametype[res.num] == 2 && c == 39) {
                cmd[row][i] = '\0';
            }
            if (res.nametype[res.num] == 3 && (c < '0' || c > '9')) {
                cmd[row][i] = '\0';
            }
        }
    }
    return res;
}

int length(STRING s) {
    return strlen(s);
}

/* 从 s 的 start 位置起找模式串 t，找到返回首次出现位置，找不到返回 -1 */
int index_of(STRING s, STRING t, int start) {
    int i, j;
    if ((start < 0) || (start + length(t) > length(s)) || (length(t) == 0)) {
        return -1;
    }
    i = start;
    j = 0;
    while (i < length(s) && j < length(t)) {
        if (s[i] == t[j]) {
            i++;
            j++;
        } else {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j == length(t)) {
        return i - length(t);
    }
    return -1;
}

/* 求子串：从 s 的 start 处截取 len 个字符存进新串 v */
void substr(STRING s, int start, int len, STRING v) {
    int i;
    STRING buf;
    /* 修正 1：越界判断 && 改为 || */
    if (start < 0 || start >= length(s)) {
        printf("Cannot be substr!!!\n");
        return;
    }
    if (len >= 1 && len <= length(s) - start) {
        /* 修正 5：改用 malloc，避免和其他新串共用同一缓冲区 */
        buf = (STRING)malloc((size_t)(len + 1));
        if (buf == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }
        for (i = 0; i < len; i++) {
            buf[i] = s[start + i];
        }
        buf[i] = '\0';
        head.strname[head.curnum] = v;
        head.strhead[head.curnum] = buf;
        head.curnum++;
        printf("The string %s : %s\n", v, buf);
    } else {
        printf("Cannot be substr!!!\n");
    }
}

/* 串连接：把 s、t 依次拼到新串 v 里 */
void concat(STRING s, STRING t, STRING v) {
    int i, j;
    int total = (int)strlen(s) + (int)strlen(t);
    /* 修正 5：改用 malloc，避免和其他新串共用同一缓冲区 */
    STRING buf = (STRING)malloc((size_t)(total + 1));
    if (buf == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    for (i = 0; s[i] != '\0'; i++) {
        buf[i] = s[i];
    }
    for (j = i, i = 0; t[i] != '\0'; i++, j++) {
        buf[j] = t[i];
    }
    buf[j] = '\0';
    head.strname[head.curnum] = v;
    head.strhead[head.curnum] = buf;
    head.curnum++;
    printf("The string %s : %s\n", v, buf);
}

int equal(STRING s, STRING t) {
    if (strcmp(s, t) == 0) {
        return 1;
    }
    return 0;
}

/* assign s t：把 t 赋给 s */
void assign(STRING s, STRING t) {
    int look1, look2;
    if ((look1 = lookup(t)) == -1) {
        printf("The string %s is not found!\n", t);
        return;
    }
    if ((look2 = lookup(s)) == -1) {
        /* s 还没建过，新增一项 */
        head.strhead[head.curnum] = head.strhead[look1];
        head.strname[head.curnum] = s;
        head.curnum++;
        printf("The string %s : %s\n", s, head.strhead[look1]);
    } else {
        /* 修正 4：赋值方向改回来，把源串地址写入目标槽位 */
        head.strhead[look2] = head.strhead[look1];
        printf("The string %s : %s\n", s, head.strhead[look2]);
    }
}

/* 新建串 */
void creat(STRING s, STRING ss) {
    /* 修正 5：malloc 拷贝一份，防止 cmd 缓冲区被下一条命令覆盖 */
    size_t n = strlen(ss);
    STRING buf = (STRING)malloc(n + 1);
    if (buf == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    memcpy(buf, ss, n + 1);
    head.strhead[head.curnum] = buf;
    head.strname[head.curnum] = s;
    head.curnum++;
    printf("The string %s : %s\n", s, buf);
}

int main(void) {
    struct result comm;
    int row = 0, i;
    char c;
    head.curnum = 0;

    while (1) {
        printf("CMD>");
        /* 读一整行命令 */
        i = 0;
        while ((c = (char)getchar()) != '\n' && c != EOF && i < 79) {
            cmd[row][i++] = c;
        }
        cmd[row][i] = '\0';
        if (c == EOF && i == 0) {
            printf("\nBye Bye!\n");
            break;
        }

        comm = cmdsyna(row);
        if (comm.num < 0 || comm.str[0] == NULL || comm.str[0][0] == '\0') {
            continue;
        }

        /* 下面按命令动词依次匹配 */
        if (strcmp(comm.str[0], "creat") == 0) {
            if (comm.num != 2) {
                printf("Parameters is wrong!\n");
            } else if (lookup(comm.str[1]) != -1) {
                /* 修正 2：printf 补上缺失的 comm.str[1] */
                printf("String %s cannot be creat!\n", comm.str[1]);
            } else {
                if (comm.nametype[1] == 4 && comm.nametype[2] == 2) {
                    creat(comm.str[1], comm.str[2]);
                } else {
                    printf("Parameters is wrong!\n");
                }
            }
        }
        else if (strcmp(comm.str[0], "assign") == 0) {
            if (comm.num != 2) {
                printf("Parameters is wrong!\n");
            } else {
                assign(comm.str[1], comm.str[2]);
            }
        }
        else if (strcmp(comm.str[0], "equal") == 0) {
            if (comm.num != 2) {
                printf("Parameters is wrong!\n");
            } else if (lookup(comm.str[1]) == -1 || lookup(comm.str[2]) == -1) {
                printf("String %s or %s is not found!\n", comm.str[1], comm.str[2]);
            } else {
                if (equal(head.strhead[lookup(comm.str[1])],
                          head.strhead[lookup(comm.str[2])]) == 1) {
                    printf("Equal!!!\n");
                } else {
                    printf("Not equal!!!\n");
                }
            }
        }
        else if (strcmp(comm.str[0], "length") == 0) {
            if (comm.num != 1) {
                printf("Parameters is wrong!\n");
            } else if (lookup(comm.str[1]) == -1) {
                /* 修正 3：printf 补上缺失的 comm.str[1] */
                printf("The string %s connot be found!\n", comm.str[1]);
            } else {
                printf("String %s :%s length is %d\n",
                       comm.str[1],
                       head.strhead[lookup(comm.str[1])],
                       length(head.strhead[lookup(comm.str[1])]));
            }
        }
        else if (strcmp(comm.str[0], "concat") == 0) {
            if (comm.num != 3) {
                printf("Parameters is wrong!\n");
            } else if (lookup(comm.str[1]) == -1 || lookup(comm.str[2]) == -1) {
                printf("The string %s or %s is not found!\n", comm.str[1], comm.str[2]);
            } else if (lookup(comm.str[3]) != -1) {
                printf("String %s connot be creat!\n", comm.str[3]);
            } else {
                concat(head.strhead[lookup(comm.str[1])],
                       head.strhead[lookup(comm.str[2])],
                       comm.str[3]);
            }
        }
        else if (strcmp(comm.str[0], "substr") == 0) {
            if (comm.num != 4) {
                printf("Parameters wrong!\n");
            } else if (lookup(comm.str[1]) == -1) {
                printf("String %s is not found!\n", comm.str[1]);
            } else if (lookup(comm.str[4]) != -1) {
                printf("String %s connot be creat!\n", comm.str[4]);
            } else {
                int start, len;
                start = strtoint(comm.str[2]);
                len = strtoint(comm.str[3]);
                substr(head.strhead[lookup(comm.str[1])], start, len, comm.str[4]);
            }
        }
        else if (strcmp(comm.str[0], "index") == 0) {
            if (comm.num != 3) {
                printf("Parameters wrong!\n");
            } else if (lookup(comm.str[1]) == -1 || lookup(comm.str[2]) == -1) {
                printf("String %s or %s is not found!\n", comm.str[1], comm.str[2]);
            } else {
                int start, flag;
                start = strtoint(comm.str[3]);
                flag = index_of(head.strhead[lookup(comm.str[1])],
                                head.strhead[lookup(comm.str[2])],
                                start);
                if (flag == -1) {
                    printf("String %s start %d,String %s is not index!!!\n",
                           comm.str[1], start, comm.str[2]);
                } else {
                    printf("String %s start %d,String %s is index %d!!!\n",
                           comm.str[1], start, comm.str[2], flag);
                }
            }
        }
        else if (strcmp(comm.str[0], "quit") == 0) {
            printf("Bye Bye!\n");
            break;
        }
        else {
            printf("Bad filename or no command!\n");
        }

        row++;
        if (row >= 40) {
            row = 0;
        }
    }

    return 0;
}
