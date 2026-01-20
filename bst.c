#include <stdlib.h>
#include "bst.h"

// Allocates and initializes an empty binary search tree
BST* createBST(int (*cmp)(void*, void*), void (*print)(void*), void (*freeData)(void*)) {
    BST* tree = malloc(sizeof(BST));
    if (tree == NULL) {
        return NULL;
    }
    tree -> root = NULL;
    tree -> compare = cmp;
    tree -> print = print;
    tree -> freeData = freeData;
    return tree;
}

// Recursively inserts data into the BST
BSTNode* bstInsert(BSTNode* root, void* data, int (*cmp)(void*, void*)){
    if (root == NULL) {
        BSTNode* newNode = malloc(sizeof(BSTNode));
        if (newNode == NULL) {
            return NULL;
        }
        newNode -> data = data;
        newNode -> left = NULL;
        newNode -> right = NULL;
        return newNode;
    }
    int result = cmp(data, root->data);
    if (result < 0) {
        root -> left = bstInsert(root->left, data, cmp);
    }
    else if (result > 0) {
        root -> right = bstInsert(root->right, data, cmp);
    }
    return root;
}

// Find data in BST
void* bstFind(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    if (root == NULL) {
        return NULL;
    }
    int result = cmp(data, root->data);
    if (result == 0) {
        return root->data;
    }
    else if (result < 0) {
        return bstFind(root->left, data, cmp);
    }
    else {
        return bstFind(root->right, data, cmp);
    }
}


void bstInorder(BSTNode* root, void (*print)(void*)) {
    if (root == NULL) {
        return;
    }
    bstInorder(root -> left, print);
    print(root -> data);
    bstInorder(root -> right, print);
}


void bstPreorder(BSTNode* root, void (*print)(void*)) {
    if (root == NULL) {
        return;
    }
    print(root -> data);
    bstPreorder(root -> left, print);
    bstPreorder(root -> right, print);
}


void bstPostorder(BSTNode* root, void (*print)(void*)) {
    if (root == NULL) {
        return;
    }
    bstPostorder(root -> left, print);
    bstPostorder(root -> right, print);
    print(root -> data);

}


void bstFree(BSTNode* root, void (*freeData)(void*)) {
    if (root == NULL) {
        return;
    }
    bstFree(root -> left, freeData);
    bstFree(root -> right, freeData);
    freeData(root -> data);
    free(root);
}