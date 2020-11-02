#include <stdio.h>

// 循环队列的顺序实现

#define Maxsize 5

typedef struct cycqueue
{
    int data[Maxsize];
    int front, rear; // 队首指针，队尾指针, 尾入首出
}CycQue;


// 打印
void print(CycQue *CQ){
    if (CQ->rear == CQ->front){
        printf("Empty!\n");
    }
    else{
        printf("rear");
        int f=CQ->front, r=CQ->rear;
        if (r>f){
            for (r; r>f; r--){
                printf("->%d", CQ->data[r]);
            };
            printf("->front\n");
        }
        else{
            for (r;r>=0;r--){
                printf("->%d", CQ->data[r]);
            };
            int i=Maxsize-1;
            for (i;i>f;i--){
                printf("->%d", CQ->data[i]);
            };
            printf("->front\n");
        }
    }
}


// 入队
void EnQueue(CycQue *CQ, int x){
    if ((CQ->rear + 1) % Maxsize == CQ->front){  // 队满
        printf("Full!\n");
    }
    else{
        CQ->rear = (CQ->rear + 1) % Maxsize;  // 队尾后移
        CQ->data[CQ->rear] = x; // 入队
    }
}


// 出队
void OutQueue(CycQue *CQ){
    if (CQ->front == CQ->rear){
        printf("Empty!\n");
    }
    else{
        CQ->front = (CQ->front + 1) % Maxsize;
    }
}


void main(){
    // 初始化
    CycQue CQ;
    CQ.front=0;
    CQ.rear=0;

    print(&CQ);
    EnQueue(&CQ, 1);
    EnQueue(&CQ, 2);
    EnQueue(&CQ, 3);
    EnQueue(&CQ, 4);

    OutQueue(&CQ);
    print(&CQ);
    EnQueue(&CQ, 5);
    print(&CQ);
    OutQueue(&CQ);
    OutQueue(&CQ);
    print(&CQ);
}
