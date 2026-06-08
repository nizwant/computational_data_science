#include <stdlib.h>
#include <stdio.h>

typedef struct _node
{
    int val;
    struct _node *left;
    struct _node *right;
} Node;

void insert_to_tree(Node **root_ptr, int val){
    Node* new_node = (Node *)malloc(sizeof(Node));
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;

    Node *root = *root_ptr;
    Node *root_parent = *root_ptr;

    while (root != NULL){
        root_parent = root;

        if ((root->val) > val){
            root = root -> left;
        }
        else{
            root = root -> right;
        }
    }

    if (root_parent == NULL) *root_ptr = new_node;
    else if (root_parent -> val > val)
    {
        root_parent -> left = new_node;
    }
    else{
        root_parent -> right = new_node;
    }
    
}


void print_inorder(Node **root_ptr){
    Node *root = *root_ptr;
    if (root == NULL) return;
    print_inorder(&(root -> left));
    printf("%d ", root -> val);
    print_inorder(&(root -> right));
}

int number_of_elems(Node **root_ptr){
    Node *root = *root_ptr;
    if (root == NULL) return 0;
    int left = number_of_elems(&(root -> left));
    int right = number_of_elems(&(root -> right));
    return left + right + 1;
}

void delete_all(Node **root_ptr){
    Node *root = *root_ptr;
    if (root == NULL) return;
    delete_all(&(root -> left));
    delete_all(&(root -> right));
    free(root);
    *root_ptr = NULL;
}

Node* search(Node *root, int val){
    Node *tmp = root;

    while(tmp != NULL && tmp ->val != val){
        if(tmp ->val > val) tmp = tmp -> left;
        else tmp = tmp -> right;
    }

    return tmp;
}

int main(){

    Node * root_ptr = NULL;

    int list[] = {1,2,4,5,3,-1,6};

    for(int i=0; i<7; i++){
        insert_to_tree(&root_ptr, list[i]);
    }

    print_inorder(&root_ptr);

    printf("\n%d\n", number_of_elems(&root_ptr));

    return 0;
}