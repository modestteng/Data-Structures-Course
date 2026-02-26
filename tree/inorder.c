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

// Block: 中序遍历（In-order）：左子树 → 根 → 右子树。
// Idea: 对二叉搜索树做中序遍历，可以得到升序排列的序列——这是中序遍历最重要的应用场景。
void inorder(const BTreeNode *root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);          // 先遍历左子树
    printf("%d ", root->data);    // 再访问根
    inorder(root->right);         // 最后遍历右子树
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

// Block: 主函数，构建示例二叉树并执行中序遍历。
// 树的形状：
//         1
//        / \
//       2   3
//      / \
//     4   5
// 中序遍历预期输出：4 2 5 1 3
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

    // Block: output，执行并打印中序遍历结果。
    printf("In-order traversal: ");
    inorder(root);
    printf("\n");

    // Block: cleanup，释放内存。
    free_tree(root);
    return 0;
}
