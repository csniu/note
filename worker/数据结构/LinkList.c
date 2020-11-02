#include <stdio.h>
#include <stdlib.h>

// 单链表


// 定义节点数据类型
typedef struct 
{ int num;
  char name[8];
  char sex[2];
  int age;
  int score;
} DataType;


// 定义单链表
typedef struct node
{ 
  // DataType data;
  int data; // 数据域
  struct node * next;  // 指针域
} Node, *LinkList;


// 初始化
LinkList InitiateLinkList()
{
    LinkList head;  // 头指针
    head=malloc(sizeof(LinkList)); // 头节点
    // head->data=88888;  // 头节点的数据域一般不存储数据
    head->next=NULL;
    return head;
}


// 逐个打印
void print(LinkList head) {
    LinkList p;
    p = head;
    while (p->next != NULL) {
        printf("->%d", p->data);
        p = p->next;
    };
    printf("\n");
}


// 求表长
int LengthLinkList(LinkList head) {
    LinkList p;
    p = head;
    int cnt = 0;

    while (p->next != NULL)
    {
        p=p->next;
        cnt++;
    }
    return cnt;
}


// 读取第 i 个元素
LinkList GetLinkList(LinkList head, int i) {
    LinkList p;
    p=head->next;  // 初始化工作节点并指向头节点

    int c = 1;
    while ((c<=i) && (p->next != NULL))
    {
        if (c == i) {
            return p;
        };
        
        c++;
        p=p->next;
    };
    return NULL;
}


// 定位数据 x 的位置
int LocateLinkList(LinkList head, int x) {
    LinkList p;
    p=head->next;

    int cnt = 1;
    while (p->next != NULL)
    {
        if (p->data == x) {
            return cnt;
        };

        cnt++;
        p=p->next;
    }
    return 0;
}


// 在 i 位置插入 x
void InsertLinkList(LinkList head, int x, int i) {
    LinkList p, q;
    
    // 插入位置的前一个节点
    if (i==1){
        p=head;
    }
    else {
        p=GetLinkList(head, i-1);
    }

    if (p == NULL) {
        printf("Index Error!\n");
        exit(1);
    }

    // 创建节点
    q=malloc(sizeof(LinkList));
    q->data = x;

    // 插入
    q->next = p->next;
    p->next = q;
}


// 删除第 i 个元素
void DeleteLinkList(LinkList head, int i) {
    LinkList p, q;

    // 前一个节点
    if (i==1){
        p=head;
    }
    else {
        p=GetLinkList(head, i-1);
    }

    // 待删除节点存在
    if ((p != NULL) && (p->next != NULL)) {
        // 删除
        q=p->next;  // 待删除节点
        p->next=q->next;
        // 释放空间
        free(q);
    }
    else {
        printf("Index Error!\n");
    }
}


void main() {
    LinkList head, p, q; 
    head = InitiateLinkList();  // 头指针
    printf("head -> %d\n", head);
    
    int i;
    p = head;  // 工作节点
    for (i = 1; i <= 5; i++) {
        // 创建新节点
        q = malloc(sizeof(LinkList));
        q->data = i*2;
        q->next = NULL;

        // 工作节点指向新节点
        p->next = q;
        // 更新工作节点
        p = q;
    }
    print(head);

    int length = LengthLinkList(head);
    printf("Length: %d\n", length);

    printf("3 index data: %d\n", GetLinkList(head, 3)->data);

    printf("data 6 index: %d\n", LocateLinkList(head, 6));

    InsertLinkList(head, 12, 1);
    printf("insert 12 at 1\n");
    print(head);

    DeleteLinkList(head, 3);
    printf("Delete index: 3\n");
    print(head);
}