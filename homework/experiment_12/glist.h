#ifndef GLIST_H
#define GLIST_H

// tag=0 原子 / tag=1 子表，val 据此二选一；link 接同层下一个兄弟
typedef struct gl_node {
    int tag;
    union {
        char data;
        struct gl_node *sublist;
    } val;
    struct gl_node *link;
} GLNode;

// 教材原型 char *&s 在 C 中改为二级指针，让递归调用共享同一游标
GLNode *CreateGL(char **s);

int GLLength(GLNode *g);

// 原子 0；空表 1
int GLDepth(GLNode *g);

void DispGL(GLNode *g);

void DestroyGL(GLNode *g);

#endif
