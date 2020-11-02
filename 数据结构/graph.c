#include <stdio.h>

#define maxsize 10

typedef struct graph {
    int a[maxsize][maxsize];
    int vexnum;
    int edgnum;
} graph_adj;


// 所有入度为m的节点数
int in_degree(graph_adj g, int m)
{
    int count, count_num, i, j;

    count_num = 0;
    for (i = 1; i <= 5; i++) {
        count = 0;
        for (j = 1; j <= 5; j++) {
            if (g.a[j][i] == 1)
                count++;
        }
        if (count == m)
            count_num++;
    }
    return count_num;

}


void main()
{    int c;
    int i,j;
    graph_adj g;
    int b[6][6]= { {0}, {0,0,1,1,0,0}, {0,0,0,0,0,1}, {0,0,0,0,0,1}, {0,0,1,1,0,1}, {0,1,0,0,0,0}};
    g.vexnum=5;g.edgnum=8;
    for(i=1;i<=5;i++)
    for(j=1;j<=5;j++)
    g.a[i][j]=b[i][j];
    c=in_degree(g,2);
    printf("in_degree=2  is %d\n",c);
}