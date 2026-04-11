#include <stdio.h>

typedef struct _Node {
    int val;
    struct _Node* left;
    struct _Node* right; 
} Node;

int main(){
    printf("Hello world\n");

    Node root = {1, NULL, NULL};

    Node* root_ptn = &root;

    printf("%d", root_ptn->val);
    return 0;
}