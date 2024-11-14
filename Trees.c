#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#define COUNT 10

int max(int a, int b)
{
    return (a > b) ? a : b;
}

typedef struct Node
{
    int data;
    int height;
    struct Node *left, *right;
} Node;

// Function Declarations
void print2D(Node *root);
void print2DUtil(Node *root, int space);

// Queue for Level Order Traversal
typedef struct QueueNode
{
    Node *treeNode;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front, *rear;
} Queue;

// Function Prototypes
Node *newNode(int data);
int getHeight(Node *node);

// BST Functions
Node *insertBST(Node *root, int j);
Node *createBST(int arr[], int i, int n);
Node *minValueNodeBST(Node *node);
Node *deleteBST(Node *root, int key);

// AVL Functions
Node *rightRotate(Node *prt);
Node *leftRotate(Node *prt);
Node *retrace(Node *node);
Node *insertAVL(Node *root, int j);
Node *minValueNodeAVL(Node *node);
Node *deleteAVL(Node *root, int key);
Node *createAVL(int arr[], int i, int n);

// Traversal Functions
void morrisInorder(Node *root);
void morrisPreorder(Node *root);
void printLevelOrder(Node *root);

// Tree Reconstruction Functions
Node *buildTreeInPost(int in[], int post[], int inStart, int inEnd, int *postIndex);
Node *buildTreeInPre(int in[], int pre[], int inStart, int inEnd, int *preIndex);

// Queue Functions
Queue *createQueue();
void enqueue(Queue *q, Node *node);
Node *dequeue(Queue *q);
int isEmptyQueue(Queue *q);

// Print Functions
void print2DUtil(Node *root, int space);
void print2D(Node *root);

// Main Function
int main()
{
    // Sample arrays
    int arr[] = {2, 1, 3, 4, 6, 8, 5, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    // ==================== BST Operations ====================
    printf("=== Creating and Printing BST ===\n");
    Node *bstRoot = createBST(arr, 0, n);
    print2D(bstRoot);
    printf("\nLevel Order Traversal of BST: ");
    printLevelOrder(bstRoot);
    printf("\nInorder Traversal of BST: ");
    morrisInorder(bstRoot);
    printf("\nPreorder Traversal of BST: ");
    morrisPreorder(bstRoot);
    printf("\n");

    // Delete a node from BST
    printf("\n=== Deleting 4 from BST ===\n");
    bstRoot = deleteBST(bstRoot, 4);
    print2D(bstRoot);
    printf("\nLevel Order Traversal of BST after deletion: ");
    printLevelOrder(bstRoot);
    printf("\nInorder Traversal of BST after deletion: ");
    morrisInorder(bstRoot);
    printf("\nPreorder Traversal of BST after deletion: ");
    morrisPreorder(bstRoot);
    printf("\n");

    // ==================== AVL Operations ====================
    printf("\n=== Creating and Printing AVL Tree ===\n");
    Node *avlRoot = createAVL(arr, 0, n);
    print2D(avlRoot);
    printf("\nLevel Order Traversal of AVL: ");
    printLevelOrder(avlRoot);
    printf("\nInorder Traversal of AVL: ");
    morrisInorder(avlRoot);
    printf("\nPreorder Traversal of AVL: ");
    morrisPreorder(avlRoot);
    printf("\n");

    // Delete a node from AVL
    printf("\n=== Deleting 4 from AVL ===\n");
    avlRoot = deleteAVL(avlRoot, 4);
    print2D(avlRoot);
    printf("\nLevel Order Traversal of AVL after deletion: ");
    printLevelOrder(avlRoot);
    printf("\nInorder Traversal of AVL after deletion: ");
    morrisInorder(avlRoot);
    printf("\nPreorder Traversal of AVL after deletion: ");
    morrisPreorder(avlRoot);
    printf("\n");

    // ==================== Tree Reconstruction ====================
    // Example for creating tree from Inorder and Postorder
    int inorder1[] = {9, 3, 15, 20, 7};
    int postorder1[] = {9, 15, 7, 20, 3};
    int n1 = sizeof(inorder1) / sizeof(inorder1[0]);

    printf("\n=== Creating Tree from Inorder and Postorder Traversals ===\n");
    int postIndex = n1 - 1;
    Node *treeInPost = buildTreeInPost(inorder1, postorder1, 0, n1 - 1, &postIndex);
    print2D(treeInPost);
    printf("\nLevel Order Traversal of Tree (Inorder & Postorder): ");
    printLevelOrder(treeInPost);
    printf("\nInorder Traversal of Tree (Inorder & Postorder): ");
    morrisInorder(treeInPost);
    printf("\nPreorder Traversal of Tree (Inorder & Postorder): ");
    morrisPreorder(treeInPost);
    printf("\n");

    // Example for creating tree from Inorder and Preorder
    int inorder2[] = {4, 2, 5, 1, 6, 3, 7};
    int preorder2[] = {1, 2, 4, 5, 3, 6, 7};
    int n2 = sizeof(inorder2) / sizeof(inorder2[0]);

    printf("\n=== Creating Tree from Inorder and Preorder Traversals ===\n");
    int preIndex = 0;
    Node *treeInPre = buildTreeInPre(inorder2, preorder2, 0, n2 - 1, &preIndex);
    print2D(treeInPre);
    printf("\nLevel Order Traversal of Tree (Inorder & Preorder): ");
    printLevelOrder(treeInPre);
    printf("\nInorder Traversal of Tree (Inorder & Preorder): ");
    morrisInorder(treeInPre);
    printf("\nPreorder Traversal of Tree (Inorder & Preorder): ");
    morrisPreorder(treeInPre);
    printf("\n");

    return 0;
}

// ==================== Node Creation ====================
Node *newNode(int data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;
    node->height = 0;
    return node;
}

// Get height of node
int getHeight(Node *node)
{
    if (node == NULL)
        return -1;
    else
        return node->height;
}

// ==================== BST Functions ====================

// Insert into BST
Node *insertBST(Node *root, int j)
{
    if (root == NULL)
        return newNode(j);
    if (j <= root->data)
        root->left = insertBST(root->left, j);
    else
        root->right = insertBST(root->right, j);

    return root;
}

// Create BST from array
Node *createBST(int arr[], int i, int n)
{
    if (n == 0)
        return NULL;

    Node *root = newNode(arr[0]);

    for (int i = 1; i < n; i++)
        insertBST(root, arr[i]);

    return root;
}

// Find the minimum value node in BST
Node *minValueNodeBST(Node *node)
{
    Node *current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Delete a node from BST
Node *deleteBST(Node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = deleteBST(root->left, key);
    else if (key > root->data)
        root->right = deleteBST(root->right, key);
    else
    {
        // Node with only one child or no child
        if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor
        Node *temp = minValueNodeBST(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data);
    }
    return root;
}

// ==================== AVL Functions ====================

// Right rotate
Node *rightRotate(Node *prt)
{
    Node *chd = prt->left;
    prt->left = chd->right;
    chd->right = prt;

    // Update heights: first the original root, then the new root
    prt->height = 1 + max(getHeight(prt->left), getHeight(prt->right));
    chd->height = 1 + max(getHeight(chd->left), getHeight(chd->right));

    return chd; // Return the new root after rotation
}

// Left rotate
Node *leftRotate(Node *prt)
{
    Node *chd = prt->right;
    prt->right = chd->left;
    chd->left = prt;

    // Update heights: first the original root, then the new root
    prt->height = 1 + max(getHeight(prt->left), getHeight(prt->right));
    chd->height = 1 + max(getHeight(chd->left), getHeight(chd->right));

    return chd; // Return the new root after rotation
}

// Rebalance the AVL tree
Node *retrace(Node *node)
{
    if (node == NULL)
        return node;

    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getHeight(node->left) - getHeight(node->right);

    // Left Left Case
    if (balance > 1 && getHeight(node->left->left) >= getHeight(node->left->right))
        return rightRotate(node);

    // Left Right Case
    if (balance > 1 && getHeight(node->left->left) < getHeight(node->left->right))
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && getHeight(node->right->right) >= getHeight(node->right->left))
        return leftRotate(node);

    // Right Left Case
    if (balance < -1 && getHeight(node->right->right) < getHeight(node->right->left))
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Insert into AVL
Node *insertAVL(Node *root, int j)
{
    if (root == NULL)
        return newNode(j);
    if (j <= root->data)
        root->left = insertAVL(root->left, j);
    else
        root->right = insertAVL(root->right, j);

    return retrace(root);
}

// Find the minimum value node in AVL (same as BST)
Node *minValueNodeAVL(Node *node)
{
    Node *current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Delete a node from AVL
Node *deleteAVL(Node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = deleteAVL(root->left, key);
    else if (key > root->data)
        root->right = deleteAVL(root->right, key);
    else
    {
        // Node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL))
        {
            Node *temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else               // One child case
                *root = *temp; // Copy the contents of the non-empty child

            free(temp);
        }
        else
        {
            // Node with two children: Get the inorder successor
            Node *temp = minValueNodeAVL(root->right);
            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // Rebalance the node
    return retrace(root);
}

// Create AVL tree from array
Node *createAVL(int arr[], int i, int n)
{
    if (n == 0)
        return NULL;

    Node *root = NULL;
    for (int i = 0; i < n; i++)
        root = insertAVL(root, arr[i]);

    return root;
}

// ==================== Traversal Functions ====================

// Morris Inorder Traversal
void morrisInorder(Node *root)
{
    Node *current = root;
    while (current != NULL)
    {
        if (current->left == NULL)
        {
            printf("%d ", current->data);
            current = current->right;
        }
        else
        {
            // Find the inorder predecessor of current
            Node *pre = current->left;
            while (pre->right != NULL && pre->right != current)
                pre = pre->right;

            // Make current as right child of its inorder predecessor
            if (pre->right == NULL)
            {
                pre->right = current;
                current = current->left;
            }
            else
            {
                pre->right = NULL;
                printf("%d ", current->data);
                current = current->right;
            }
        }
    }
}

// Morris Preorder Traversal
void morrisPreorder(Node *root)
{
    Node *current = root;
    while (current != NULL)
    {
        if (current->left == NULL)
        {
            printf("%d ", current->data);
            current = current->right;
        }
        else
        {
            // Find the inorder predecessor of current
            Node *pre = current->left;
            while (pre->right != NULL && pre->right != current)
                pre = pre->right;

            // Make current as right child of its inorder predecessor
            if (pre->right == NULL)
            {
                printf("%d ", current->data); // Visit before making the link
                pre->right = current;
                current = current->left;
            }
            else
            {
                pre->right = NULL;
                current = current->right;
            }
        }
    }
}

// ==================== Level Order Functions ====================

// Initialize a new queue
Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Enqueue a tree node
void enqueue(Queue *q, Node *node)
{
    QueueNode *temp = (QueueNode *)malloc(sizeof(QueueNode));
    temp->treeNode = node;
    temp->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

// Dequeue a tree node
Node *dequeue(Queue *q)
{
    if (q->front == NULL)
        return NULL;
    QueueNode *temp = q->front;
    Node *node = temp->treeNode;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return node;
}

// Check if queue is empty
int isEmptyQueue(Queue *q)
{
    return q->front == NULL;
}

// Print level order traversal
void printLevelOrder(Node *root)
{
    if (root == NULL)
        return;

    Queue *q = createQueue();
    enqueue(q, root);

    while (!isEmptyQueue(q))
    {
        Node *current = dequeue(q);
        printf("%d ", current->data);

        if (current->left != NULL)
            enqueue(q, current->left);
        if (current->right != NULL)
            enqueue(q, current->right);
    }

    // Free the queue
    free(q);
}

// ==================== Tree Reconstruction Functions ====================

// Utility function to find index of a value in inorder array
int searchInorder(int in[], int start, int end, int value)
{
    for (int i = start; i <= end; i++)
        if (in[i] == value)
            return i;
    return -1;
}

// Build tree from Inorder and Postorder traversals
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

// Build tree from Inorder and Preorder traversals
Node *buildTreeInPre(int in[], int pre[], int inStart, int inEnd, int *preIndex)
{
    if (inStart > inEnd)
        return NULL;

    // Pick current node from Preorder traversal
    // and increment preIndex
    Node *node = newNode(pre[*preIndex]);
    (*preIndex)++;

    // If this node has no children, return it
    if (inStart == inEnd)
        return node;

    // Else find the index of this node in Inorder traversal
    int inIndex = searchInorder(in, inStart, inEnd, node->data);

    // Using index in Inorder traversal, construct left and right subtrees
    node->left = buildTreeInPre(in, pre, inStart, inIndex - 1, preIndex);
    node->right = buildTreeInPre(in, pre, inIndex + 1, inEnd, preIndex);

    return node;
}

// ==================== Print Functions ====================

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(Node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(Node *root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}
