#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

// 二叉排序树结点结构
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 栈结构（用于非递归遍历）
typedef struct Stack {
    TreeNode** data;
    int top;
    int capacity;
} Stack;

// 队列结构（用于层次遍历）
typedef struct Queue {
    TreeNode** data;
    int front;
    int rear;
    int capacity;
} Queue;

//函数声明

// 结点操作函数
TreeNode* createNode(int val);

// 栈操作函数
Stack* createStack(int capacity);
void push(Stack* stack, TreeNode* node);
TreeNode* pop(Stack* stack);
bool isStackEmpty(Stack* stack);
void freeStack(Stack* stack);

// 队列操作函数
Queue* createQueue(int capacity);
void enqueue(Queue* queue, TreeNode* node);
TreeNode* dequeue(Queue* queue);
bool isQueueEmpty(Queue* queue);
void freeQueue(Queue* queue);

// 二叉排序树操作函数
TreeNode* insert(TreeNode* node, int val);
void preorderRecursive(TreeNode* node);
void inorderRecursive(TreeNode* node);
void postorderRecursive(TreeNode* node);
void preorderNonRecursive(TreeNode* root);
void inorderNonRecursive(TreeNode* root);
void postorderNonRecursive(TreeNode* root);
void levelOrder(TreeNode* root);
int search(TreeNode* node, int key);
void swapLeftRight(TreeNode* node);
int getDepth(TreeNode* node);
int getLeafCount(TreeNode* node);
TreeNode* findMin(TreeNode* node);
TreeNode* deleteNode(TreeNode* node, int val);

// 辅助函数
void clearInputBuffer();

// 全局根结点指针
TreeNode* root = NULL;

// 创建新结点
TreeNode* createNode(int val) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("内存分配失败!\n");
        exit(1);
    }
    newNode->data = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


// 栈操作函数
Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->data = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void push(Stack* stack, TreeNode* node) {
    if (stack->top < stack->capacity - 1) {
        stack->data[++stack->top] = node;
    }
}

TreeNode* pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top--];
    }
    return NULL;
}

bool isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}


// 队列操作函数
Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->data = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
    queue->front = 0;
    queue->rear = -1;
    queue->capacity = capacity;
    return queue;
}

void enqueue(Queue* queue, TreeNode* node) {
    if (queue->rear < queue->capacity - 1) {
        queue->data[++queue->rear] = node;
    }
}

TreeNode* dequeue(Queue* queue) {
    if (queue->front <= queue->rear) {
        return queue->data[queue->front++];
    }
    return NULL;
}

bool isQueueEmpty(Queue* queue) {
    return queue->front > queue->rear;
}

void freeQueue(Queue* queue) {
    free(queue->data);
    free(queue);
}

/*（1）*/

// 插入新结点
TreeNode* insert(TreeNode* node, int val) {
    if (node == NULL) {
        return createNode(val);
    }

    if (val < node->data) {
        node->left = insert(node->left, val);
    } else if (val > node->data) {
        node->right = insert(node->right, val);
    }
    // 如果 val == node->data，则不插入重复值
    return node;
}

/*（2）*/

// 前序遍历（递归）
void preorderRecursive(TreeNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorderRecursive(node->left);
        preorderRecursive(node->right);
    }
}

// 中序遍历（递归）
void inorderRecursive(TreeNode* node) {
    if (node != NULL) {
        inorderRecursive(node->left);
        printf("%d ", node->data);
        inorderRecursive(node->right);
    }
}

// 后序遍历（递归）
void postorderRecursive(TreeNode* node) {
    if (node != NULL) {
        postorderRecursive(node->left);
        postorderRecursive(node->right);
        printf("%d ", node->data);
    }
}

/*（3）*/

// 前序遍历（非递归）
void preorderNonRecursive(TreeNode* root) {
    if (root == NULL) return;

    Stack* stack = createStack(1000);
    push(stack, root);

    while (!isStackEmpty(stack)) {
        TreeNode* node = pop(stack);
        printf("%d ", node->data);

        if (node->right) push(stack, node->right);
        if (node->left) push(stack, node->left);
    }

    freeStack(stack);
}

// 中序遍历（非递归）
void inorderNonRecursive(TreeNode* root) {
    Stack* stack = createStack(1000);
    TreeNode* current = root;

    while (current != NULL || !isStackEmpty(stack)) {
        while (current != NULL) {
            push(stack, current);
            current = current->left;
        }

        current = pop(stack);
        printf("%d ", current->data);
        current = current->right;
    }

    freeStack(stack);
}

// 后序遍历（非递归）
void postorderNonRecursive(TreeNode* root) {
    if (root == NULL) return;

    Stack* stack1 = createStack(1000);
    Stack* stack2 = createStack(1000);
    push(stack1, root);

    while (!isStackEmpty(stack1)) {
        TreeNode* node = pop(stack1);
        push(stack2, node);

        if (node->left) push(stack1, node->left);
        if (node->right) push(stack1, node->right);
    }

    while (!isStackEmpty(stack2)) {
        TreeNode* node = pop(stack2);
        printf("%d ", node->data);
    }

    freeStack(stack1);
    freeStack(stack2);
}

/*（4）*/

// 层次遍历
void levelOrder(TreeNode* root) {
    if (root == NULL) return;

    Queue* queue = createQueue(1000);
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        TreeNode* node = dequeue(queue);
        printf("%d ", node->data);

        if (node->left) enqueue(queue, node->left);
        if (node->right) enqueue(queue, node->right);
    }

    freeQueue(queue);
}

/*（5）*/

// 查找给定关键字
int search(TreeNode* node, int key) {
    if (node == NULL) {
        return 0; // 失败
    }

    if (key == node->data) {
        return 1; // 成功
    } else if (key < node->data) {
        return search(node->left, key);
    } else {
        return search(node->right, key);
    }
}

/*（6）*/

// 交换各结点的左右子树
void swapLeftRight(TreeNode* node) {
    if (node != NULL) {
        // 交换左右子树
        TreeNode* temp = node->left;
        node->left = node->right;
        node->right = temp;

        // 递归交换
        swapLeftRight(node->left);
        swapLeftRight(node->right);
    }
}

/*（7）*/

// 求二叉树的深度
int getDepth(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int leftDepth = getDepth(node->left);
    int rightDepth = getDepth(node->right);

    return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
}

/*（8）*/

// 叶子结点数
int getLeafCount(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->left == NULL && node->right == NULL) {
        return 1;
    }

    return getLeafCount(node->left) + getLeafCount(node->right);
}

// 查找最小值结点
TreeNode* findMin(TreeNode* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

/*（9）*/

// 删除某结点
TreeNode* deleteNode(TreeNode* node, int val) {
    if (node == NULL) {
        return node;
    }

    if (val < node->data) {
        node->left = deleteNode(node->left, val);
    } else if (val > node->data) {
        node->right = deleteNode(node->right, val);
    } else {
        // 找到要删除的结点
        if (node->left == NULL) {
            TreeNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            TreeNode* temp = node->left;
            free(node);
            return temp;
        }

        // 有两个子结点的情况
        TreeNode* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteNode(node->right, temp->data);
    }
    return node;
}

// 清空输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int choice, value;
    while (true) {
        system("cls"); // 清屏
        printf("欢迎使用二叉排序树演示程序!\n");
        printf("\n========== 二叉排序树操作菜单 ==========\n");
        printf("1. 插入新结点\n");
        printf("2. 前序遍历(递归)\n");
        printf("3. 中序遍历(递归)\n");
        printf("4. 后序遍历(递归)\n");
        printf("5. 前序遍历(非递归)\n");
        printf("6. 中序遍历(非递归)\n");
        printf("7. 后序遍历(非递归)\n");
        printf("8. 层次遍历\n");
        printf("9. 查找关键字\n");
        printf("10. 交换左右子树\n");
        printf("11. 求树的深度\n");
        printf("12. 求叶子结点数\n");
        printf("13. 删除结点\n");
        printf("0. 退出程序\n");
        printf("=======================================\n");
        printf("请选择操作: ");
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字!\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                printf("请输入要插入的值: ");
                if (scanf("%d", &value) == 1) {
                    root = insert(root, value);
                    printf("成功插入结点: %d\n", value);
                } else {
                    printf("输入错误!\n");
                    clearInputBuffer();
                }
                break;

            case 2:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("前序遍历(递归): ");
                    preorderRecursive(root);
                    printf("\n");
                }
                break;

            case 3:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("中序遍历(递归): ");
                    inorderRecursive(root);
                    printf("\n");
                }
                break;

            case 4:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("后序遍历(递归): ");
                    postorderRecursive(root);
                    printf("\n");
                }
                break;

            case 5:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("前序遍历(非递归): ");
                    preorderNonRecursive(root);
                    printf("\n");
                }
                break;

            case 6:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("中序遍历(非递归): ");
                    inorderNonRecursive(root);
                    printf("\n");
                }
                break;

            case 7:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("后序遍历(非递归): ");
                    postorderNonRecursive(root);
                    printf("\n");
                }
                break;

            case 8:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("层次遍历: ");
                    levelOrder(root);
                    printf("\n");
                }
                break;

            case 9:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("请输入要查找的关键字: ");
                    if (scanf("%d", &value) == 1) {
                        if (search(root, value)) {
                            printf("查找成功: 找到关键字 %d\n", value);
                        } else {
                            printf("查找失败: 未找到关键字 %d\n", value);
                        }
                    } else {
                        printf("输入错误!\n");
                        clearInputBuffer();
                    }
                }
                break;

            case 10:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    swapLeftRight(root);
                    printf("已交换所有结点的左右子树\n");
                }
                break;

            case 11:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    int depth = getDepth(root);
                    printf("二叉树的深度: %d\n", depth);
                }
                break;

            case 12:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    int count = getLeafCount(root);
                    printf("叶子结点数: %d\n", count);
                }
                break;

            case 13:
                if (root == NULL) {
                    printf("树为空!\n");
                } else {
                    printf("请输入要删除的值: ");
                    if (scanf("%d", &value) == 1) {
                        root = deleteNode(root, value);
                        printf("成功删除结点: %d\n", value);
                    } else {
                        printf("输入错误!\n");
                        clearInputBuffer();
                    }
                }
                break;

            case 0:
                printf("感谢使用! 程序退出。\n");
                return 0;

            default:
                printf("无效选择，请重新输入!\n");
                break;
        }

        printf("\n按回车键继续...");
        clearInputBuffer();
        getchar();
    }

    return 0;
}
