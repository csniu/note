#include <stdio.h>
#include <stdlib.h>

// 顺序表

#define Maxsize 10

// 定义节点数据类型
typedef struct 
{ int num;
  char name[8];
  char sex[2];
  int age;
  int score;
} DataType;

// 使用数组定义顺序表
typedef struct 
{ 
  // DataType data [Maxsize];
  int data [Maxsize];
  int length;
} SeqList;

// 所有奇数之和
int sum(SeqList l)
{
    int i, sum;
    sum = 0;
    for (i = 0; i <= l.length-1; i++) {
        if (l.data[i] % 2 != 0) {
            sum += l.data[i];
        }
    }
    return sum;
}

// 打印顺序表的每个元素
void print(SeqList l) {
    int i;
    for (i = 0; i <= l.length-1; i++){
        printf("%3d,", l.data[i]);
    }
    printf("\n");
}


// 插入，将 x 插入到 i 位置
SeqList InsertSeqList(SeqList l, int x, int i)
{
  if (l.length == Maxsize) {
    printf("SeqList Full!");
    exit(1);
  };

  if (i<0 || i>l.length) {
    printf("Index Error!");
    exit(1);
  };

  int j;
  for (j=l.length; j>=i; j--) {
    l.data[j] = l.data[j-1];  // 依次先后挪一位
  }

  l.data[i] = x;  // 插入数据
  l.length++;
  return l;
}


// 删除位置 i 上的元素
SeqList DeleteSeqList(SeqList l, int i) {
  if (i<0 || i>l.length-1){
    printf("Index Error!");
    exit(1);
  }

  int j;
  for (j=i; j<=l.length-1; j++){
    l.data[j] = l.data[j+1];  // 依次前移一位
  }
  l.length--;
  return l;
}


// 定位元素 x 的位置
int LocateSeqList(SeqList l, int x) {
  int j;
  for (j=0; j<l.length; j++) {
    if (l.data[j] == x) {
      return j + 1;
    }
  }
  return 0;
}


void main() {
    SeqList L;
    int i, s;
    for (i = 0; i <= 5; i++)
        L.data[i] = i;
    L.length = 6;    

    printf("init SeqList->L\n");  // 必须使用双引号
    print(L);

    s = sum(L);
    printf("sum=%3d\n", s);

    L = InsertSeqList(L, 8, 0);
    print(L);

    L = DeleteSeqList(L, 2);
    print(L);

    int ll;
    ll = LocateSeqList(L, 2);
    printf("%3d\n", ll);
}
