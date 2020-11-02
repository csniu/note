#include <stdio.h>
#include <stdlib.h>

// 队列的链表实现

typedef struct LinkQueueNode 
{
    int data;
    struct LinkQueuenode *next;
} LKQueNode;

typedef struct LkQueue 
{
    LKQueNode *front, *rear;  // 首尾指针
} LkQue;



// 打印
void print(LkQue *LQ){
    if (LQ->front == LQ->rear){
        printf("Empty!\n");
    }
    else{
        LKQueNode *p;
        p=LQ->front;
        printf("front");
        while (p->next != NULL)
        {
            printf("->%d", p->data);
        }
        printf("->rear\n");        
    }
}



// 入队
void Push(LkQue *LQ, int x){
    // 新建节点
    LKQueNode *p;
    p=(LKQueNode *)malloc(sizeof(LKQueNode));
    p->data=x;
    p->next=NULL;

    // 插入
    (LQ->rear)->next=p;

    // 更新尾指针
    LQ->rear=p;
}




void main(){
    // 初始化
    LkQue *LQ;
    LKQueNode *LQN;
    LQN=(LKQueNode *)malloc(sizeof(LKQueNode));
    LQ->front=LQN;
    LQ->rear=LQN;
    (LQ->front)->next = NULL;

    print(LQ);


}