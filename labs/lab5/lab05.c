#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    int data;
    struct _node* next;
} node;

//typedef node* nodeptr;

// void list_print(const node* head)
// {
//     while (head) {  // while(head!=0)
//         printf("%d,", head->data); //(*head).data
//         head = head->next;
//     }
//     printf("\n");
// }

//
// void list_print(const node* head)
// { // recursion - overkill!
//     if (!head) printf("\n");
//     else {
//         printf("%d,", head->data);
//         list_print(head->next);  // tail recursion
//     }
// }

void list_print(const node* head)
{ // recursion - overkill!
start:
    if (!head) printf("\n");
    else {
        printf("%d,", head->data);
        head = head->next;
        goto start;
    }
}

void list_push_front(node** head, int data)
{
    node* old_head = *head;
    *head = (node*)malloc(sizeof(node));
    (*head)->data = data;  // (*(*head)).data
    (*head)->next = old_head;
}

void list_push_back(node** head, int data)
{
    if (!*head) list_push_front(head, data);
    else {
        node* x = *head;
        while (x->next) x = x->next;
        x->next = (node*)malloc(sizeof(node));
        x->next->next = NULL;
        x->next->data = data;
    }
}

int list_empty(const node* head)
{
    return (head==NULL);
}

int list_pop_front(node** head)
{
    if (!*head) return -1;
    int ret = (*head)->data;
    node* old_head = *head;
    *head = (*head)->next;
    free(old_head);
    return ret;
}

// implement get_ith - extract the i-th element
// push_ith

// doubly-linked lists (next, prev) - push/pop front/back

// binary search trees
// AVL, ... - rotations
// ↑ all of the above for the test


// graphs - EXTRA
// node vectors of pointers (store the degree of each node too)

// B-trees

int main()
{
    node* head = (node*)0; // NULL;
    list_push_back(&head, 0);
    for (int i=1; i<=5; ++i)
        list_push_front(&head, i);
        // head = list_push_front(head, i);
    list_print(head);
    list_push_back(&head, 42);
    list_pop_front(&head);
    list_print(head);
    while (!list_empty(head))
        list_pop_front(&head);
    list_print(head);
    return 0;
}
