#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LEN 1000

int max(int a, int b)
{
    return (a > b) ? a : b;
}

typedef struct Node
{
    int data;
    struct Node *left, *right;
} Node;

typedef struct Queue
{
    Node *arr[MAX_LEN];
    int front, back;
} Queue;

// Function Prototypes
Node *newNode(int data);

// BST Functions
Node *insertBST(Node *root, int j);
Node *createBST(int arr[], int n);
Node *minValueNodeBST(Node *node);
Node *deleteBST(Node *root, int key);

// Traversal Functions
void morrisInorder(Node *root);
void morrisPreorder(Node *root);
void printLevelOrder(Node *root);

// Tree Reconstruction Functions
Node *buildTreeInPost(int in[], int post[], int inStart, int inEnd, int *postIndex);
Node *buildTreeInPre(int in[], int pre[], int inStart, int inEnd, int *preIndex);

// Queue Functions
Queue init();
bool isEmpty(Queue q);
bool isFull(Queue q);
Queue enqueue(Queue q, Node *node);
Queue dequeue(Queue q);
Node *front(Queue q);

Node *insertBST(Node *root, int j)
{
    if (root == NULL)
        return newNode(j);

    if (j > root->data)
        root->right = insertBST(root->right, j);
    else
        root->left = insertBST(root->left, j);

    return root;
}

Node *createBST(int arr[], int n)
{
    Node *root = newNode(arr[0]);
    for (int i = 1; i < n; i++)
    {
        insertBST(root, arr[i]);
    }
    return root;
}

Node *minValueNodeBST(Node *node)
{
    Node *current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

Node *deleteBST(Node *root, int key)
{
    if (root == NULL)
        return root;

    if (key > root->data)
        root->right = deleteBST(root->right, key);
    else if (key < root->data)
        root->left = deleteBST(root->left, key);
    else
    {
        if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        Node *temp = minValueNodeBST(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data);
    }

    return root;
}

void morrisInorder(Node *root)
{
    Node *curr = root;
    while (curr != NULL)
    {
        if (curr->left == NULL)
        {
            printf("%d ", curr->data);
            curr = curr->right;
        }
        else
        {
            Node *pre = curr->left;
            while (pre->right != NULL && pre->right != curr)
                pre = pre->right;
            if (pre->right == curr)
            {
                pre->right = NULL;
                printf("%d ", curr->data);
                curr = curr->right;
            }
            else
            {
                pre->right = curr;
                curr = curr->left;
            }
        }
    }
}

void morrisPreorder(Node *root)
{
    Node *curr = root;
    while (curr != NULL)
    {
        if (curr->left == NULL)
        {
            printf("%d ", curr->data);
            curr = curr->right;
        }
        else
        {
            Node *pre = curr->left;
            while (pre->right != NULL && pre->right != curr)
                pre = pre->right;
            if (pre->right == curr)
            {
                pre->right = NULL;
                curr = curr->right;
            }
            else
            {
                printf("%d ", curr->data);
                pre->right = curr;
                curr = curr->left;
            }
        }
    }
}

void printLevelOrder(Node *root)
{
    if (root == NULL)
        return;
    Queue q = init();
    q = enqueue(q, root);
    while (!isEmpty(q))
    {
        Node *current = front(q);
        q = dequeue(q);
        if (current == NULL)
        {
            printf("-1 ");
            continue;
        }
        q = enqueue(q, current->left);
        q = enqueue(q, current->right);
        printf("%d ", current->data);
    }
    printf("\n");
}
int searchInorder(int in[], int start, int end, int value)
{
    for (int i = start; i <= end; i++)
        if (in[i] == value)
            return i;
    return -1;
}

Node *buildTreeInPost(int in[], int post[], int inStart, int inEnd, int *postIndex)
{
    if (inStart > inEnd)
        return NULL;

    // Pick current node from Postorder traversal
    // and decrement postIndex
    Node *node = newNode(post[*postIndex]);
    (*postIndex)--;

    // If this node has no children, return it
    if (inStart == inEnd)
        return node;

    // Else find the index of this node in Inorder traversal
    int inIndex = searchInorder(in, inStart, inEnd, node->data);

    // Using index in Inorder traversal, construct right and left subtrees
    node->right = buildTreeInPost(in, post, inIndex + 1, inEnd, postIndex);
    node->left = buildTreeInPost(in, post, inStart, inIndex - 1, postIndex);

    return node;
}

Node *buildTreeInPre(int in[], int pre[], int inStart, int inEnd, int *preIndex)
{
    if (inStart > inEnd)
        return NULL;
    Node *root = newNode(pre[*preIndex]);
    (*preIndex)++;
    if (inStart == inEnd)
        return root;
    int ind = searchInorder(in, inStart, inEnd, root->data);
    root->left = buildTreeInPre(in, pre, inStart, ind - 1, preIndex);
    root->right = buildTreeInPre(in, pre, ind + 1, inEnd, preIndex);
    return root;
}

// Main Function to Test All Functionalities
int main()
{
    // Sample Data for BST
    int arr[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("=== Creating BST ===\n");
    Node *root = createBST(arr, n);
    printf("BST created with elements: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    // Level Order Traversal
    printf("=== Level Order Traversal ===\n");
    printLevelOrder(root);
    printf("\n");

    // Morris Inorder Traversal
    printf("=== Morris Inorder Traversal ===\n");
    morrisInorder(root);
    printf("\n\n");

    // Morris Preorder Traversal
    printf("=== Morris Preorder Traversal ===\n");
    morrisPreorder(root);
    printf("\n\n");

    // Find Minimum Value Node
    printf("=== Finding Minimum Value Node ===\n");
    Node *minNode = minValueNodeBST(root);
    if (minNode != NULL)
    {
        printf("Minimum value in BST: %d\n\n", minNode->data);
    }

    // Delete a Node from BST
    int keyToDelete = 30;
    printf("=== Deleting Node %d from BST ===\n", keyToDelete);
    root = deleteBST(root, keyToDelete);
    printf("BST after deletion:\n");
    printLevelOrder(root);
    printf("\n");

    // Tree Reconstruction from Inorder and Postorder
    printf("=== Reconstructing Tree from Inorder and Postorder ===\n");
    int inOrder[] = {20, 30, 40, 50, 60, 70, 80};
    int postOrder[] = {20, 40, 30, 60, 80, 70, 50};
    int postIndex = n - 1;
    Node *postConstructed = buildTreeInPost(inOrder, postOrder, 0, n - 1, &postIndex);
    printf("Level Order Traversal of Postorder Constructed Tree:\n");
    printLevelOrder(postConstructed);
    printf("\n");

    // Tree Reconstruction from Inorder and Preorder
    printf("=== Reconstructing Tree from Inorder and Preorder ===\n");
    int preOrder[] = {50, 30, 20, 40, 70, 60, 80};
    int preIndex = 0;
    Node *preConstructed = buildTreeInPre(inOrder, preOrder, 0, n - 1, &preIndex);
    printf("Level Order Traversal of Preorder Constructed Tree:\n");
    printLevelOrder(preConstructed);
    printf("\n");

    // Morris Traversals on Reconstructed Trees
    printf("=== Morris Inorder Traversal of Postorder Constructed Tree ===\n");
    morrisInorder(postConstructed);
    printf("\n\n");

    printf("=== Morris Preorder Traversal of Preorder Constructed Tree ===\n");
    morrisPreorder(preConstructed);
    printf("\n\n");

    return 0;
}

Node *newNode(int data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

Queue init()
{
    Queue q;
    q.front = 0;
    q.back = MAX_LEN - 1;
    return q;
}

bool isEmpty(Queue q)
{
    return q.front == (q.back + 1) % MAX_LEN;
}

bool isFull(Queue q)
{
    return q.front == (q.back + 2) % MAX_LEN;
}

Node *front(Queue q)
{
    if (isEmpty(q))
    {
        printf("front: empty Queue");
        return NULL;
    }
    return q.arr[q.front];
}

Queue enqueue(Queue q, Node *node)
{
    if (isFull(q))
    {
        printf("enqueue: Queue is full\n");
        return q;
    }
    q.back = (++q.back) % MAX_LEN;
    q.arr[q.back] = node;
    return q;
}

Queue dequeue(Queue q)
{
    if (isEmpty(q))
    {
        printf("dequeue: empty Queue\n");
        return q;
    }
    q.front = (++q.front) % MAX_LEN;
    return q;
}
