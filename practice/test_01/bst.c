#include <stdio.h>

typedef struct _Node {
    int val;
    struct _Node* left;
    struct _Node* right; 
} Node;

void is_empty(Node* root, int* is_empty){
    *is_empty = (root == NULL);
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