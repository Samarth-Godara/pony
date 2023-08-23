#include<stdio.h>
#include<malloc.h>
#include<time.h>

struct node
{
    struct node * child[3];
    int visit,level, id;
};

FILE *file;

typedef struct node g_node;

int node_limit, total_nodes, front=-1, rear=-1, node_id=0;

void get_node_number()
{
    printf("Enter numbers of nodes you want \nto generate the graph of (multiple of 2) : ");
    scanf("%d", &node_limit);
}

g_node * generate_node()
{
    int i;
    g_node *ptr = (g_node *)malloc(sizeof(g_node));

    for(i=0;i<3;i++)
        ptr->child[i]=NULL;

    ptr->id=node_id++;
    ptr->level=0;
    ptr->visit=0;

    return ptr;
}

void connect_nodes(g_node *a, g_node *b)
{
    a->child[a->visit]=b;
    b->child[b->visit]=a;
    a->visit++;
    b->visit++;
}

g_node *dequeue( g_node *queue[node_limit])
{
    if(front==-1) return NULL;
    g_node *x = queue[front];
    front++;
    return x;
}

void rand_enqueue(g_node *x, g_node *queue[node_limit])
{
    int r_no;
    g_node *temp;

    if(front==-1)
    {
        front=0;
        rear=0;
        queue[front]=x;
    }
    else
    {
        if((rear+2)!=node_limit)
        {
            rear++;
            queue[rear]=x;
            r_no = rand()%((rear-front)+1);
            temp = queue[front+r_no];
            queue[front+r_no] = queue[rear];
            queue[rear] = temp;
        }
    }
}


g_node* generate_graph(g_node *queue[node_limit])
{
    g_node* head, *a, *b, *c, *d;

    a=generate_node();
    b=generate_node();
    c=generate_node();
    d=generate_node();

    connect_nodes(a,b);
    connect_nodes(b,c);
    connect_nodes(b,d);

    total_nodes+=4;

    head=a;

    rand_enqueue(c,queue);
    rand_enqueue(d,queue);

    while(total_nodes<node_limit)
    {
        a=dequeue(queue);
        b=generate_node();
        c=generate_node();
        connect_nodes(a,b);
        connect_nodes(a,c);
        rand_enqueue(b,queue);
        rand_enqueue(c,queue);
        total_nodes+=2;
    }

    return head;
}

void assign_numbers(g_node *head)
{
    int i;

    if(head==NULL || head->visit==0) return;

    head->visit=0;
    head->id=node_id++;

    for(i=0;i<3;i++)
    {
        assign_numbers(head->child[i]);
    }
}

void connect_mat_node(int a, int b, short mat[node_limit][5])
{
    int i;

    for(i=0;i<mat[a][3];i++)
        if(mat[a][i]==b) return;

    mat[a][mat[a][3]]=b;
    mat[b][mat[b][3]]=a;
    mat[a][3]++;
    mat[b][3]++;
}

void create_matrix(g_node * head, short mat[node_limit][5])
{
    int i;

    if(head==NULL || head->visit==1) return;
    head->visit=1;

    for(i=0; i<3; i++)
    {
        if(head->child[i])
                connect_mat_node(head->id, head->child[i]->id, mat);
        create_matrix(head->child[i], mat);
    }
}

void initialize_mat(short mat[node_limit][5])
{
    int i;
    for(i=0;i<node_limit;i++)
    {
        mat[i][0]=-1;
        mat[i][1]=-1;
        mat[i][2]=-1;
        mat[i][3]=0;
        mat[i][4]=0;
    }
}

void print_matrix(short mat[node_limit][5])
{
    int i,j;
    printf("Printing matrix :\n");
    for(i=0;i<node_limit;i++)
    {
        printf("%d : ",i);
        for(j=0;j<5;j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

int reverse(int x)
{
    int a=0;
    while(x>0)
    {
        a=a*10;
        a=a+(x%10);
        x=x/10;
    }
    return a;
}

void write_numb(int x)
{
    int r=x, len=0;

    for(;r>0;r/=10) len++;

    if(x>=0)
    {
        x=reverse(x);
        do
        {
            r=x%10;
            x=x/10;
            fputc((char)('0'+r),file);
            len--;
        }while(len>0);
    }
    else
    {
        fputc('-',file);
        x=x*-1;
        x=reverse(x);
        do
        {
            r=x%10;
            x=x/10;
            fputc((char)('0'+r),file);
            len--;
        }while(len>0);
    }
}

void write_matrix_file(short mat[node_limit][5])
{
    int i,j;

    for(i=0;i<node_limit;i++)
    {
        mat[i][3]=0;
        mat[i][4]=0;
    }

    write_numb(node_limit);
    fputc('\n',file);

    fputc('{',file);
    for(i=0;i<node_limit;i++)
    {
        fputc('{',file);
        for(j=0;j<5;j++)
        {
            write_numb(mat[i][j]);
            if(j<=3)
                fputc(',',file);
        }
        fputc('}',file);
        if(i<(node_limit-1))
        {
            fputc(',',file);
            if(i%10==0)fputc('\n',file);
        }
    }
    fputc('}',file);
    fputc(';',file);

    fclose(file);
}

int main()
{
    srand(time(0));

    get_node_number();

    g_node *queue[node_limit];
    short graph_mat[node_limit][5];
    queue[0]=NULL;
    file = fopen("graph_matrix.dat", "w");


    initialize_mat(graph_mat);

    g_node *head = generate_graph(queue);

    node_id=0;

    assign_numbers(head);

    create_matrix(head, graph_mat);

    print_matrix(graph_mat);

    write_matrix_file(graph_mat);

    exit(0);
}
