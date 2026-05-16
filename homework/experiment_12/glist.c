#include <stdio.h>
#include <stdlib.h>
#include "glist.h"

// 递归下降：每次消费一个 token，遇 '(' 下钻、遇 ')' 回溯
GLNode *CreateGL(char **s) {
    char ch = **s;
    GLNode *g = NULL;

    if (ch == '\0') return NULL;
    (*s)++;

    if (ch == ')') return NULL;

    g = (GLNode *)malloc(sizeof(GLNode));
    if (g == NULL) return NULL;

    if (ch == '(') {
        g->tag = 1;
        g->val.sublist = CreateGL(s);
    } else {
        g->tag = 0;
        g->val.data = ch;
    }

    ch = **s;
    if (ch == ',') {
        (*s)++;
        g->link = CreateGL(s);
    } else if (ch == ')') {
        (*s)++;
        g->link = NULL;
    } else {
        g->link = NULL;
    }

    return g;
}

int GLLength(GLNode *g) {
    GLNode *p;
    int n = 0;
    if (g == NULL || g->tag == 0) return 0;
    for (p = g->val.sublist; p != NULL; p = p->link) {
        n++;
    }
    return n;
}

// 原子 0；空表 1；非空表 1 + max(子表深度)
int GLDepth(GLNode *g) {
    GLNode *p;
    int max_dep, sub_dep;

    if (g == NULL) return 1;
    if (g->tag == 0) return 0;
    if (g->val.sublist == NULL) return 1;

    max_dep = 0;
    for (p = g->val.sublist; p != NULL; p = p->link) {
        if (p->tag == 1) {
            sub_dep = GLDepth(p);
            if (sub_dep > max_dep) max_dep = sub_dep;
        }
    }
    return max_dep + 1;
}

void DispGL(GLNode *g) {
    if (g == NULL) return;
    if (g->tag == 0) {
        printf("%c", g->val.data);
    } else {
        printf("(");
        if (g->val.sublist != NULL) {
            DispGL(g->val.sublist);
        }
        printf(")");
    }
    if (g->link != NULL) {
        printf(",");
        DispGL(g->link);
    }
}

void DestroyGL(GLNode *g) {
    if (g == NULL) return;
    if (g->tag == 1) {
        DestroyGL(g->val.sublist);
    }
    DestroyGL(g->link);
    free(g);
}
