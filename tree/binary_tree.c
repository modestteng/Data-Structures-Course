#include <stdio.h>
#include <stdlib.h>

// Block: 定义二叉树结点结构。
// 每个结点存储一个整数值，以及指向左右子结点的指针。
// Idea: 二叉树的递归定义——每个结点本身就是一棵子树的根。
typedef struct BTreeNode {
    int data;
    struct BTreeNode *left;
    struct BTreeNode *right;
} BTreeNode;

// Block: 创建一个新结点，分配内存并初始化字段。
BTreeNode *create_node(int value) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Block: 释放整棵二叉树的内存，采用后序方式递归释放。
// Idea: 后序释放——先释放子树，再释放自身，避免悬空指针。
void free_tree(BTreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Block: 主函数，手动构建一棵示例二叉树并打印根结点值。
// 树的形状：
//         1
//        / \
//       2   3
//      / \
//     4   5
int main(void) {
    // Block: setup，创建各结点。
    BTreeNode *root = create_node(1);
    BTreeNode *n2   = create_node(2);
    BTreeNode *n3   = create_node(3);
    BTreeNode *n4   = create_node(4);
    BTreeNode *n5   = create_node(5);

    if (root == NULL || n2 == NULL || n3 == NULL || n4 == NULL || n5 == NULL) {
        free(root);
        free(n2);
        free(n3);
        free(n4);
        free(n5);
        return 1;
    }

    // Block: core logic，将各结点链接成树形结构。
    root->left  = n2;
    root->right = n3;
    n2->left    = n4;
    n2->right   = n5;

    // Block: output，打印根结点值确认树已构建成功。
    printf("Root: %d\n", root->data);
    printf("Left child of root: %d\n", root->left->data);
    printf("Right child of root: %d\n", root->right->data);

    // Block: cleanup，释放所有结点内存。
    free_tree(root);
    return 0;
}
