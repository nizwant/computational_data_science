#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
   int val;
   struct Node* next;
} node;

node* putFront(int val, node* head){
    struct Node* node = malloc(sizeof(node));
    node->val = val;
    node->next = head;
    return node;
}

node* removeFront(node* head){
    node* new_head = head->next;
    free(head);
    return new_head;
}

int main(){
    node* head = NULL;

    for(int i = 0; i < 100; i++){
        head = putFront(i, head);
    }

    while (head)
    {
        printf("%d\n", head->val);
        head = removeFront(head);
    }
}

