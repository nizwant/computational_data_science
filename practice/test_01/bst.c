#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int val;
    struct _Node* left;
    struct _Node* right; 
} Node;

void is_empty(Node* root, int* is_empty){
    *is_empty = (root == NULL);
}

int get_max_height(Node* node){
    if(node == NULL) return 0;

    int left = get_max_height(node->left);
    int right = get_max_height(node->right);

    int max = (left > right) ? left:right;

    return max + 1;
}

int tree_elements_number(Node* node){
    if(node == NULL) return 0;

    int left = tree_elements_number(node->left);
    int right = tree_elements_number(node->right);

    return left + right + 1;
}

void insert_to_tree(Node** root, int value){
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->val = value;

    if (*root == NULL) {
        *root = new_node;
        return;
    }
    
    Node* node = *root;
    Node* prev;

    while (node != NULL){
        prev = node;
        if (value > node->val) node = node->right;
        else node = node ->left;
    }

    if (value > prev->val) prev->right = new_node;
    if (value < prev->val) prev->left = new_node;
}

int main(){
    Node root = {1, NULL, NULL};
    Node* root_ptr = &root;
    int is_empty_val;
    is_empty(root_ptr, &is_empty_val);
    printf("%d\n", root_ptr->val);
    printf("%d\n", is_empty_val);

    root_ptr = NULL;
    is_empty(root_ptr, &is_empty_val);
    printf("%d\n", is_empty_val);
    return 0;
}