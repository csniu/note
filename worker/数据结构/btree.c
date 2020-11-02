#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char data;
    struct node *lchild, *rchild;
} b_tree;



int n = 0;
int hight(b_tree *t)
/*求二叉树高度  */
{
    int h1, h2;
    if (t == NULL)
        return 0;
    else {
        h1 = hight(t->lchild);
        h2 = hight(t->rchild);
        return 1 + (h1 > h2 ? h1 : h2);
    }

}



void postorder(b_tree *t)
/*后序遍历*/

{
    if (t) {
        postorder(t->lchild);
        postorder(t->rchild);
        printf("%c", t->data);
    }
}


void main() {
    b_tree *t, *p, *q, *r;
    t = (b_tree *) malloc(sizeof(b_tree));
    p = (b_tree *) malloc(sizeof(b_tree));
    q = (b_tree *) malloc(sizeof(b_tree));
    r = (b_tree *) malloc(sizeof(b_tree));
    t->data = 'a';
    p->data = 'b';
    q->data = 'c';
    r->data = 'd';

    t->lchild = p;
    t->rchild = q;
    p->lchild = r;
    p->rchild = NULL;
    q->rchild = NULL;
    q->lchild = NULL;
    r->rchild = NULL;
    r->lchild = NULL;

    printf("后序遍历");
    postorder(t);
    printf("\n");

    n = hight(t);
    printf("n=%3d\n", n);
}