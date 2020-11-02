#include <stdio.h>

// 栈的顺序实现

#define Maxsize 10


// 定义
typedef struct {
    int data [Maxsize];  // 使用数组存放栈的数据元素,最多放 Maxsize-1 个元素
    int top;  // 栈顶的位置
} SeqStk;


// 初始化
void InitStack(SeqStk *stk){
    stk->top=0;
}


// 打印
void print(SeqStk *stk){
    if (stk->top == 0){  // 为空
        printf("Empty!\n");
    }
    else{
        int i=stk->top;
        for (i;i>0;i--){
            printf("%d\n", stk->data[i]);
        }
        printf("bottom\n");
    }
}


// 进栈 x
int Push(SeqStk *stk, int x){
    if (stk->top==Maxsize-1){
        printf("Full!\n");
        return 0;
    }
    else{
        stk->top++;  // 栈顶上移
        stk->data[stk->top]=x;
        printf("Push: %d\n", x);
        return 1;
    } 
}


// 出栈
int Pop(SeqStk *stk){
    if (stk->top==0){
        printf("Empty!\n");
        return 0;
    }
    else{
        printf("Pop: %d\n", stk->data[stk->top]);
        stk->top--;  // 栈顶下移
        return 1;
    }
}


void main(){
    SeqStk stk;   
    InitStack(&stk);

    print(&stk);
    Push(&stk, 1);
    Push(&stk, 2);
    Push(&stk, 3);
    Push(&stk, 4);
    Push(&stk, 5);
    Push(&stk, 6);
    Push(&stk, 7);
    Push(&stk, 8);
    Push(&stk, 9);
    Push(&stk, 10);
    Push(&stk, 11);
    print(&stk);

    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    print(&stk);

    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    Pop(&stk);
    print(&stk);
}