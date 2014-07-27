typedef struct node
{
    int capacity;

    struct node *prev;
    struct node *next;
    void *payload;   // Then use pointer arithmetic to iterate through the nodes to find the correct node. (Should be + sizeof(Node))
} Node;

#include <stdio.h>
#include <unistd.h>
#include "mymalloc.h"

Node *head;
Node *tail;

/*
Head
Tail
Sandwich
Fitting In



 */

int main(int argc, const char * argv[])
{

    printf("\n~~~~~~~~~~ HEAD ~~~~~~~~~~\n");
    int *h = my_firstfit_malloc(1);
    int *t = my_firstfit_malloc(2);

    printList();

    my_free(h);
    printList();

    my_free(t);
    printList();

    printf("\n~~~~~~~~~~ TAIL ~~~~~~~~~~\n");
    int *h1 = my_firstfit_malloc(1);
    int *t1 = my_firstfit_malloc(2);

    printList();

    my_free(t1);
    printList();

    my_free(h1);
    printList();

    printf("\n~~~~~~~~~~ PREVIOUS ~~~~~~~~~~\n");
    int *a1 = my_firstfit_malloc(1);
    int *b1 = my_firstfit_malloc(2);
    int *c1 = my_firstfit_malloc(3);
    int *d1 = my_firstfit_malloc(4);
    int *e1 = my_firstfit_malloc(5);

    printList();

    my_free(b1);
    printList();

    my_free(c1);
    printList();

    my_free(a1);
    printList();

    my_free(d1);
    printList();

    my_free(e1);
    printList();

    printf("\n~~~~~~~~~~ NEXT ~~~~~~~~~~\n");
    int *a2 = my_firstfit_malloc(1);
    int *b2 = my_firstfit_malloc(2);
    int *c2 = my_firstfit_malloc(3);
    int *d2 = my_firstfit_malloc(4);
    int *e2 = my_firstfit_malloc(5);

    printList();

    my_free(d2);
    printList();

    my_free(c2);
    printList();

    my_free(a2);
    printList();

    my_free(b2);
    printList();

    my_free(e2);
    printList();



    printf("\n~~~~~~~~~~ SANDWICH ~~~~~~~~~~\n");
    int *a = my_firstfit_malloc(1);
    int *b = my_firstfit_malloc(2);
    int *c = my_firstfit_malloc(3);
    int *d = my_firstfit_malloc(4);
    int *e = my_firstfit_malloc(5);

    printList();

    my_free(b);
    printList();

    my_free(d);
    printList();

    my_free(c);
    printList();

    my_free(a);
    printList();

    my_free(e);
    printList();



    printf("\n~~~~~~~~~~ HEAD NEXT ~~~~~~~~~~\n");
    int *hello = my_firstfit_malloc(1);
    int *world = my_firstfit_malloc(2);
    char *my = my_firstfit_malloc(3);
    char *name = my_firstfit_malloc(4);

    printList();

    my_free(world);
    printList();

    my_free(hello);
    printList();

    my_free(my);
    printList();

    my_free(name);
    printList();


    return 0;
}

void my_free(void *ptr)
{
    if(head == tail)
    {
        sbrk(sizeof(Node) + head -> capacity);
        head = NULL;
        tail = NULL;
    }
    else
    {
        if(tail -> payload == ptr)
        { // If you're releasing the node at the end of the list.

            if(tail -> prev -> payload == NULL)
            {

                Node *oldTail = tail;
                if(tail -> prev -> prev != NULL)
                {
                    tail = tail -> prev -> prev;
                    tail -> next = NULL;
                }
                else if (tail -> prev == head)
                {
                    sbrk(-1 * (tail -> capacity + sizeof(Node) + head -> capacity + sizeof(Node)));
                    head = NULL;
                    tail = NULL;

                }

                sbrk(-1 * (oldTail -> capacity + sizeof(Node) + oldTail -> prev -> capacity + sizeof(Node)));


            }
            else
            {
                Node *oldTail = tail;

                tail = tail -> prev; // Move the tail back one
                tail -> next = NULL; // Set the new tail's next equal to null

                sbrk(-1 * (oldTail -> capacity + sizeof(Node))); // Take the old memory and node off the stack.
            }
        }
        else if (head -> payload == ptr)
        {  // If you want to free the head.


            if(head -> next != NULL)
            { // If a node other than head exists
              if(head -> next -> payload == NULL)
              { // Check to see if it's free
                  head -> payload = NULL;
                  head -> capacity = head -> capacity + head -> next -> capacity;
                  head -> next = head -> next -> next;
                  head -> next -> prev = head;
              }

            }

            head -> payload = NULL;


        } // If you're releasing the head
        else
        { // If the node you are releasing is neither the head nor the tail...
            Node *curr = ptr - sizeof(Node);
            curr -> payload = NULL;

            if(curr -> prev -> payload == NULL && curr -> next -> payload == NULL)
            {  // If you've got a free sandwich
                curr -> prev -> next = curr -> next;
                curr -> next -> prev = curr -> prev;
                curr -> prev -> capacity = curr -> prev -> capacity + curr -> capacity;

                curr = curr -> next -> prev;
                curr -> capacity = curr -> capacity + curr -> next -> capacity;
                curr -> next -> next -> prev = curr;
                curr -> next = curr -> next -> next;
            }
            else if(curr -> prev -> payload == NULL)
            { // PREV NODE ALSO NULL
                curr -> prev -> next = curr -> next;
                curr -> next -> prev = curr -> prev;
                curr -> prev -> capacity = curr -> prev -> capacity + curr -> capacity;

            }
            else if (curr -> next -> payload == NULL)
            { // NEXT NODE ALSO NULL
                curr -> capacity = curr -> capacity + curr -> next -> capacity;
                curr -> next -> next -> prev = curr;
                curr -> next = curr -> next -> next;
            }
        }
    }
}


void *my_firstfit_malloc(int size)
{
    if(head == NULL)
    {   // If this is the first element in your list.
        head = sbrk(sizeof(Node));
        tail = head; // Because we've only got 1 node.
        void *pay = sbrk(size);
        head -> capacity = size;
        head -> payload = pay;
        return head -> payload;
    }
    else
    { // If there is more than 1 element in your list...
        Node *curr = head;
        while(curr -> next != NULL)
        { // For every node
            if(curr -> payload == NULL)
            { // If it is representing a free space
                if(curr -> capacity >= size)
                { // And it is large enough to fit the requested size...
                    if(curr -> capacity == size)
                    { // If there is a perfect match
                        void* toReturn = sbrk(size);
                        curr -> payload = toReturn;
                        return toReturn;
                    }
                    else
                    { // If there isn't a perfect match
                        Node *anotherNode = curr + curr->capacity;
                        anotherNode -> next = curr -> next;
                        anotherNode -> prev = curr;
                        curr -> next = anotherNode;
                        anotherNode -> capacity = curr -> capacity - size;
                        curr -> capacity = size;
                    }


                    curr -> payload = curr + sizeof(Node); // Then set the payload as taken.
                    return curr -> payload; // return the space.

                } else curr = curr -> next;

            } else {
                curr = curr -> next;
            }
        } // If there is no space available in the list...

        tail -> next = sbrk(sizeof(Node)); // Create a new node at the end of the list.
        tail -> next -> capacity = size;
        tail -> next -> prev = tail; // Make the new tail point back to the old one.
        tail -> next -> payload = sbrk(size);

        tail = tail -> next; // Set tail to the correct location.

        return tail -> payload;
    }



}

void printList()
{
    printf("Printing list: ");

    if(head == NULL)
    {
        printf("Head is null. The list does not exist.\n");
        return;
    }

    Node *curr = head;
    while(curr -> next != NULL)
    {
        if(curr -> payload == NULL) printf("\tF(%d)\t",curr->capacity);
        else printf("\tX(%d)\t", curr->capacity);

        curr = curr -> next;

    }

    if(curr -> payload == NULL) printf("\tF(%d)\t",curr -> capacity);
    else printf("\tX(%d)\t", curr->capacity);

    printf("\n");
}