#include <stdio.h>
#include <stdlib.h>

// Block: 复用二叉树结点结构定义。
typedef struct BTreeNode {
    int data;
    struct BTreeNode *left;
    struct BTreeNode *right;
} BTreeNode;

// Block: 创建新结点。
BTreeNode *create_node(int value) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (node == NULL) {
        return NULL;
    }
    node->data  = value;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

// Block: 后序遍历（Post-order）：左子树 → 右子树 → 根。
// Idea: 后序遍历最后才访问根，天然适合"子树处理完再处理父节点"的场景，
//       比如释放树内存、计算目录大小、求子树高度等。
void postorder(const BTreeNode *root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);        // 先遍历左子树
    postorder(root->right);       // 再遍历右子树
    printf("%d ", root->data);    // 最后访问根
}

// Block: 释放整棵树的内存。
void free_tree(BTreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Block: 主函数，构建示例二叉树并执行后序遍历。
// 树的形状：
//         1
//        / \
//       2   3
//      / \
//     4   5
// 后序遍历预期输出：4 5 2 3 1
int main(void) {
    // Block: setup，创建结点。
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

    // Block: core logic，链接树结构。
    root->left  = n2;
    root->right = n3;
    n2->left    = n4;
    n2->right   = n5;

    // Block: output，执行并打印后序遍历结果。
    printf("Post-order traversal: ");
    postorder(root);
    printf("\n");

    // Block: cleanup，释放内存。
    free_tree(root);
    return 0;
}
