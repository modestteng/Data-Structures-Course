#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ATOM,
    SUBLIST
} NodeType;

typedef struct GNode {
    NodeType type;
    char atom;
    struct GNode *sublist;
    struct GNode *next;
} GNode;

// 块说明：setup，创建原子结点（如 a、b、c）。
GNode *create_atom(char value) {
    GNode *node = (GNode *)malloc(sizeof(GNode));
    if (node == NULL) {
        return NULL;
    }
    node->type = ATOM;
    node->atom = value;
    node->sublist = NULL;
    node->next = NULL;
    return node;
}

// 块说明：setup，创建子表结点（如 (b,c) 这样的整体）。
GNode *create_sublist(GNode *sub) {
    GNode *node = (GNode *)malloc(sizeof(GNode));
    if (node == NULL) {
        return NULL;
    }
    node->type = SUBLIST;
    node->atom = '\0';
    node->sublist = sub;
    node->next = NULL;
    return node;
}

// 块说明：core logic，递归打印广义表内容。
void print_glist(const GNode *head) {
    printf("(");
    const GNode *current = head;
    while (current != NULL) {
        if (current->type == ATOM) {
            printf("%c", current->atom);
        } else {
            print_glist(current->sublist);
        }
        if (current->next != NULL) {
            printf(",");
        }
        current = current->next;
    }
    printf(")");
    // 思想：广义表是“元素可再是表”的递归结构，用递归处理最自然。
}

// 块说明：cleanup，递归释放广义表全部结点。
void free_glist(GNode *head) {
    GNode *current = head;
    while (current != NULL) {
        GNode *next = current->next;
        if (current->type == SUBLIST) {
            free_glist(current->sublist);
        }
        free(current);
        current = next;
    }
}

int main(void) {
    // 块说明：setup，手工构造广义表 (a,(b,c),d)。
    GNode *a = create_atom('a');
    GNode *b = create_atom('b');
    GNode *c = create_atom('c');
    GNode *d = create_atom('d');
    if (a == NULL || b == NULL || c == NULL || d == NULL) {
        free(a);
        free(b);
        free(c);
        free(d);
        return 1;
    }

    b->next = c;
    GNode *sub = create_sublist(b);
    if (sub == NULL) {
        free_glist(a);
        free_glist(b);
        free_glist(d);
        return 1;
    }

    a->next = sub;
    sub->next = d;

    // 块说明：output，打印广义表，观察层次关系。
    printf("Generalized list: ");
    print_glist(a);
    printf("\n");

    // 块说明：edge handling，判断第二个元素是否为子表并打印其首元素。
    if (a->next != NULL && a->next->type == SUBLIST && a->next->sublist != NULL) {
        printf("First atom in sublist: %c\n", a->next->sublist->atom);
    }

    // 块说明：cleanup，释放全部动态内存。
    free_glist(a);
    return 0;
}
