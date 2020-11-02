#include <stdio.h>
#include <stdlib.h>

// 栈的链接实现


// 定义节点
typedef struct node 
{
  int data;
  struct node *next;
}LkStk;



// 打印
void print(LkStk *LS)
{
    if (LS->next == NULL){
        printf("Empty!\n");
    }
    else {
        LkStk *p;
        p=LS;
        while (p->next != NULL){
            p=p->next;
            printf("%d\n", p->data);
        }

        printf("Bottom\n");
    }
}


// 入栈 x
void Push(LkStk *LS, int x){
    // 新建节点
    LkStk *p;
    p=(LkStk *)malloc(sizeof(LkStk));
    p->data=x;

    // 插入
    p->next=LS->next;
    LS->next=p;
}

// 出栈
void Pop(LkStk *LS){
  if (LS->next != NULL){
    LkStk *p;
    p = LS->next;  // 待删除节点

    // 删除并释放
    LS->next=p->next;
    free(p);
  }
}



void main(){
    // 初始化，创建一个空的头节点
    LkStk *LS;
    LS=(LkStk *)malloc(sizeof(LkStk));
    LS->next=NULL;  // next 指向栈顶, NULL 为栈底

    print(LS);

    Push(LS, 4);
    Push(LS, 3);
    Push(LS, 2);
    print(LS);

    Pop(LS);
    print(LS);
}