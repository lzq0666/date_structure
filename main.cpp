#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

// �������������ṹ
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// ջ�ṹ�����ڷǵݹ������
typedef struct Stack {
    TreeNode** data;
    int top;
    int capacity;
} Stack;

// ���нṹ�����ڲ�α�����
typedef struct Queue {
    TreeNode** data;
    int front;
    int rear;
    int capacity;
} Queue;

//��������

// ����������
TreeNode* createNode(int val);

// ջ��������
Stack* createStack(int capacity);
void push(Stack* stack, TreeNode* node);
TreeNode* pop(Stack* stack);
bool isStackEmpty(Stack* stack);
void freeStack(Stack* stack);

// ���в�������
Queue* createQueue(int capacity);
void enqueue(Queue* queue, TreeNode* node);
TreeNode* dequeue(Queue* queue);
bool isQueueEmpty(Queue* queue);
void freeQueue(Queue* queue);

// ������������������
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

// ��������
void clearInputBuffer();

// ȫ�ָ����ָ��
TreeNode* root = NULL;

// �����½��
TreeNode* createNode(int val) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("�ڴ����ʧ��!\n");
        exit(1);
    }
    newNode->data = val;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


// ջ��������
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


// ���в�������
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

/*��1��*/

// �����½��
TreeNode* insert(TreeNode* node, int val) {
    if (node == NULL) {
        return createNode(val);
    }

    if (val < node->data) {
        node->left = insert(node->left, val);
    } else if (val > node->data) {
        node->right = insert(node->right, val);
    }
    // ��� val == node->data���򲻲����ظ�ֵ
    return node;
}

/*��2��*/

// ǰ��������ݹ飩
void preorderRecursive(TreeNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorderRecursive(node->left);
        preorderRecursive(node->right);
    }
}

// ����������ݹ飩
void inorderRecursive(TreeNode* node) {
    if (node != NULL) {
        inorderRecursive(node->left);
        printf("%d ", node->data);
        inorderRecursive(node->right);
    }
}

// ����������ݹ飩
void postorderRecursive(TreeNode* node) {
    if (node != NULL) {
        postorderRecursive(node->left);
        postorderRecursive(node->right);
        printf("%d ", node->data);
    }
}

/*��3��*/

// ǰ��������ǵݹ飩
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

// ����������ǵݹ飩
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

// ����������ǵݹ飩
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

/*��4��*/

// ��α���
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

/*��5��*/

// ���Ҹ����ؼ���
int search(TreeNode* node, int key) {
    if (node == NULL) {
        return 0; // ʧ��
    }

    if (key == node->data) {
        return 1; // �ɹ�
    } else if (key < node->data) {
        return search(node->left, key);
    } else {
        return search(node->right, key);
    }
}

/*��6��*/

// ������������������
void swapLeftRight(TreeNode* node) {
    if (node != NULL) {
        // ������������
        TreeNode* temp = node->left;
        node->left = node->right;
        node->right = temp;

        // �ݹ齻��
        swapLeftRight(node->left);
        swapLeftRight(node->right);
    }
}

/*��7��*/

// ������������
int getDepth(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    int leftDepth = getDepth(node->left);
    int rightDepth = getDepth(node->right);

    return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
}

/*��8��*/

// Ҷ�ӽ����
int getLeafCount(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->left == NULL && node->right == NULL) {
        return 1;
    }

    return getLeafCount(node->left) + getLeafCount(node->right);
}

// ������Сֵ���
TreeNode* findMin(TreeNode* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

/*��9��*/

// ɾ��ĳ���
TreeNode* deleteNode(TreeNode* node, int val) {
    if (node == NULL) {
        return node;
    }

    if (val < node->data) {
        node->left = deleteNode(node->left, val);
    } else if (val > node->data) {
        node->right = deleteNode(node->right, val);
    } else {
        // �ҵ�Ҫɾ���Ľ��
        if (node->left == NULL) {
            TreeNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            TreeNode* temp = node->left;
            free(node);
            return temp;
        }

        // �������ӽ������
        TreeNode* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteNode(node->right, temp->data);
    }
    return node;
}

// ������뻺����
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int choice, value;
    while (true) {
        system("cls"); // ����
        printf("��ӭʹ�ö�����������ʾ����!\n");
        printf("\n========== ���������������˵� ==========\n");
        printf("1. �����½��\n");
        printf("2. ǰ�����(�ݹ�)\n");
        printf("3. �������(�ݹ�)\n");
        printf("4. �������(�ݹ�)\n");
        printf("5. ǰ�����(�ǵݹ�)\n");
        printf("6. �������(�ǵݹ�)\n");
        printf("7. �������(�ǵݹ�)\n");
        printf("8. ��α���\n");
        printf("9. ���ҹؼ���\n");
        printf("10. ������������\n");
        printf("11. ���������\n");
        printf("12. ��Ҷ�ӽ����\n");
        printf("13. ɾ�����\n");
        printf("0. �˳�����\n");
        printf("=======================================\n");
        printf("��ѡ�����: ");
        if (scanf("%d", &choice) != 1) {
            printf("�����������������!\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                printf("������Ҫ�����ֵ: ");
                if (scanf("%d", &value) == 1) {
                    root = insert(root, value);
                    printf("�ɹ�������: %d\n", value);
                } else {
                    printf("�������!\n");
                    clearInputBuffer();
                }
                break;

            case 2:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("ǰ�����(�ݹ�): ");
                    preorderRecursive(root);
                    printf("\n");
                }
                break;

            case 3:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("�������(�ݹ�): ");
                    inorderRecursive(root);
                    printf("\n");
                }
                break;

            case 4:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("�������(�ݹ�): ");
                    postorderRecursive(root);
                    printf("\n");
                }
                break;

            case 5:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("ǰ�����(�ǵݹ�): ");
                    preorderNonRecursive(root);
                    printf("\n");
                }
                break;

            case 6:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("�������(�ǵݹ�): ");
                    inorderNonRecursive(root);
                    printf("\n");
                }
                break;

            case 7:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("�������(�ǵݹ�): ");
                    postorderNonRecursive(root);
                    printf("\n");
                }
                break;

            case 8:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("��α���: ");
                    levelOrder(root);
                    printf("\n");
                }
                break;

            case 9:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("������Ҫ���ҵĹؼ���: ");
                    if (scanf("%d", &value) == 1) {
                        if (search(root, value)) {
                            printf("���ҳɹ�: �ҵ��ؼ��� %d\n", value);
                        } else {
                            printf("����ʧ��: δ�ҵ��ؼ��� %d\n", value);
                        }
                    } else {
                        printf("�������!\n");
                        clearInputBuffer();
                    }
                }
                break;

            case 10:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    swapLeftRight(root);
                    printf("�ѽ������н�����������\n");
                }
                break;

            case 11:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    int depth = getDepth(root);
                    printf("�����������: %d\n", depth);
                }
                break;

            case 12:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    int count = getLeafCount(root);
                    printf("Ҷ�ӽ����: %d\n", count);
                }
                break;

            case 13:
                if (root == NULL) {
                    printf("��Ϊ��!\n");
                } else {
                    printf("������Ҫɾ����ֵ: ");
                    if (scanf("%d", &value) == 1) {
                        root = deleteNode(root, value);
                        printf("�ɹ�ɾ�����: %d\n", value);
                    } else {
                        printf("�������!\n");
                        clearInputBuffer();
                    }
                }
                break;

            case 0:
                printf("��лʹ��! �����˳���\n");
                return 0;

            default:
                printf("��Чѡ������������!\n");
                break;
        }

        printf("\n���س�������...");
        clearInputBuffer();
        getchar();
    }

    return 0;
}
