/*
    ECE 368 Project 3
    Teammates : Bishop Sarkar

    The program description is given in the README file.

*/

#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <stdlib.h> 
#include <math.h> 
/*
    Data Structure to hold 
    Socail NetworK Data
*/
typedef struct SocailNetwork{
    struct SocailNetwork * next;
    double userID;
    double age;
    double gender;
    double martial_status;
    double race;
    double birth;
    double language;
    double occ;
    double income;
}Sng;



/*
    Structure to store 
    nodes for the queries.
*/
typedef struct Node{
    struct Node * next;
    int node;
}nd;

/*
    Function to initialise the Socail
    Structure
*/
Sng * Node_construct(char ** separated)
{
    Sng * rel = malloc(sizeof(Sng));
    rel -> userID = atof(separated[0]);
    rel -> age = atof(separated[1]);
    rel -> gender = atof(separated[2]);
    rel -> martial_status = atof(separated[3]);
    rel -> race = atof(separated[4]);
    rel -> birth = atof(separated[5]);
    rel -> language = atof(separated[6]);
    rel -> occ = atof(separated[7]);
    rel -> income = atof(separated[8]);
    return rel;
}
/*
    Function to inialise the 
    node structure
*/
nd * construct(int node)
{
    nd * rel = malloc(sizeof(nd));
    rel -> node = node;
    return rel ;
}
/*
    Function to insert nodes in a 
    structure
*/
nd * insert(nd * head, int node)
{
    if (head == NULL )
    {
        return construct(node);
    }
    if(head -> node != node) 
        head -> next = insert ( head -> next , node ) ;
    return head;
}
/*
    Function to insert 2 nodes and their Lab,
    Ulab between them
*/
Sng * network_insert ( Sng * head , char ** separated )
{
    if ( head == NULL )
    {
        return Node_construct (separated) ;
    }
    head -> next = network_insert ( head -> next , separated ) ;
    return head ;
}
/*
    Function to explode Strings
    based on Delimiters
*/
char * * explode(const char * str, const char * delims, int * arrLen)
{
    int N = 0;
    int pos = 0;
    while(str[pos] != '\0')
    {
        if(strchr(delims,str[pos]) != NULL)
        {
            N++;
        }
        pos++;
    }
    char * * strArr = malloc((N+1) * sizeof(char *));
    pos = 0;
    int arrInd = 0; // this is the next position where we'll create a string
    int last = 0; // 1 + the last index we saw a delimiter. Init to 0.
    int diff = 0;
    while(str[pos] != '\0')
    {
        if(strchr(delims,str[pos]) != NULL)
        {
            diff = pos - last;
            if(diff != 0){
                strArr[arrInd] = malloc((diff) * sizeof(char *));
                memcpy(strArr[arrInd], &str[last] , (diff)*sizeof(char));
                strArr[arrInd][diff] = '\0';
            }
            else
            {
                strArr[arrInd] = malloc((1) * sizeof(char *));
                strArr[arrInd][diff] = '\0';
                
            }
            last = pos + 1;
            arrInd++;
        }
        pos++;
    }
    diff = strlen(str) - last;
    if(diff != 0)
    {
        strArr[arrInd] = malloc((diff) * sizeof(char *));
        memcpy(strArr[arrInd], &str[last] , (diff)*sizeof(char));
        strArr[arrInd][diff] = '\0';
    }   
    else
    {
        strArr[arrInd] = malloc((1) * sizeof(char *));
        strArr[arrInd][diff] = '\0';
    }
    
    *arrLen = N+1;
    
    return (char **)strArr;
}

/*
    Function to calculate Ulab between 2
    nodes
*/


/*
    Function to generate the matrix
    for Graph
*/
void graph_creation(int no_of_users, double matrix[no_of_users][no_of_users],double delta)
{
    int i, j;
    int comp1,comp2;
    for(i = 0;i<no_of_users;i++)
    {
        for(j=0;j<no_of_users;j++)
        {
            if(matrix[i][j] != -1)
            {
                comp1 = (int)(delta*100);
                comp2 = (int)(matrix[i][j]*100);
                if(comp1 < comp2)
                {
                   continue;
                }
                else
                {
                    matrix[i][j] = -1;
                }
            }
        }
    }
}
/*
    Function for Query2 
    calculation and printing
*/
void query_2(int no_of_users, double graph_c[no_of_users][no_of_users], double query1_node, double alpha)
{
    int no = 0;
    int node_1 = query1_node - 1;
    int i;
    for(i = 0;i < node_1;i++)
    {
        if(graph_c[i][node_1] != -1 && graph_c[i][node_1] < alpha)
        {
            no++;
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        if(graph_c[i][node_1] != -1 && graph_c[i][node_1] < alpha)
        {
            no++;
        }
    }
    printf("Source Node : %d\n",(int)query1_node);
    printf("No of Nodes with Distance less than alpha : %d\n\n",no);
}
/*
    Function for Query 1 calculation 
    and printing
*/
void query_1(int no_of_users, double graph_c[no_of_users][no_of_users], double query1_node)
{
    int i;
    int node_1 = query1_node - 1;
    int comp1,comp2;
    double Lmax = 1;
    for(i = 0;i < node_1;i++)
    {
        comp1 = (int)(graph_c[i][node_1]*100);
        comp2 = (int)(Lmax*100);
        if(comp1 < comp2 && graph_c[i][node_1] != -1)
        {
            Lmax = graph_c[i][node_1];
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        comp1 = (int)(graph_c[i][node_1]*100);
        comp2 = (int)(Lmax*100);
        if(comp1 < comp2 && graph_c[i][node_1] != -1)
        {
            Lmax = graph_c[node_1][i];
        }
    }
    printf("Shorted Path : %0.2f\n",Lmax);
    printf("Sorted Ids : ");
    for(i = 0;i < node_1;i++)
    {
        comp1 = (int)(graph_c[i][node_1]*100);
        comp2 = (int)(Lmax*100);
        if(comp1 == comp2)
        {
            printf("%d\t",i+1);
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        comp1 = (int)(graph_c[i][node_1]*100);
        comp2 = (int)(Lmax*100);
        if(comp1 == comp2)
        {
            printf("%d\t",i+1);
        }
    }
    printf("\n\n");

}
/*
    FUnction the immediatte neighbours of a node
*/
nd * findneighbours(nd *Node, int node,int no_of_users, double graph_c[no_of_users][no_of_users],int node_1)
{
    int i;
    for(i = 0;i < node;i++)
    {
        if(graph_c[i][node] != -1 && i != node_1)
        {
            Node = insert(Node,i+1);
        }
    }
    for(i = node + 1;i < no_of_users;i++)
    {
        if(graph_c[i][node] != -1 && i != node_1)
        {
            Node = insert(Node,i+1);
        }
    }
    return Node;
}

/*
    Sorting the linked of nodes
*/
nd *sort_list(nd *head) {

    nd *tmpPtr = head;
    nd *tmpNxt = head->next;

    int tmp;

    while(tmpNxt != NULL){
           while(tmpNxt != tmpPtr){
                    if(tmpNxt->node < tmpPtr->node){
                            tmp = tmpPtr->node;
                            tmpPtr->node = tmpNxt->node;
                            tmpNxt->node = tmp;
                    }
                    tmpPtr = tmpPtr->next;
            }
            tmpPtr = head;
            tmpNxt = tmpNxt->next;
    }
         return tmpPtr ; // Place holder
} 
/*
    Function to destory a structure
*/
void List_destroy ( nd * head )
{
    while ( head != NULL )
    {
        nd * p = head -> next ;
        free ( head ) ;
        head = p ;
    }
}
/*
    Function to calculate Query 4
    and print it
*/
void query_4(int no_of_users, double graph_c[no_of_users][no_of_users], double query1_node)
{
    int no = 0;
    int node_1 = query1_node - 1;
    int i;
    nd *Node = NULL;
    for(i = 0;i < node_1;i++)
    {
        if(graph_c[i][node_1] != -1)
        {
            no++;
            Node = insert(Node,i+1);
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        if(graph_c[i][node_1] != -1)
        {
            no++;
            Node = insert(Node,i+1);
        }
    }
    nd * temp = Node;
    int Arr[no];
    for(i = 0;i<no;i++)
    {
        Arr[i] = temp->node;
        temp = temp -> next;
    }
    while(no != 0)
    {
        Node = findneighbours(Node,Arr[no-1]-1,no_of_users,graph_c,node_1);
        no--;

    }
    sort_list(Node);
    no = 0;
    nd * tmp = Node;
    printf("Source Node : %d\n",(int)query1_node);
    printf("Sorted IDs of Second Level neighbours : ");
    while(tmp != NULL)
    {
        printf("%d\t",tmp->node);
        no++;
        tmp = tmp -> next;
    }
    printf("\n");
    printf("No of Second Level neighbours : %d\n\n",no);
}
/*
    Function to generate Query 3, calculate it 
    and print it
*/
void query_3(int no_of_users, double graph_c[no_of_users][no_of_users], double query1_node)
{
    int no = 0;
    int node_1 = query1_node - 1;
    int i;
    nd *Node = NULL;
    for(i = 0;i < node_1;i++)
    {
        if(graph_c[i][node_1] != -1)
        {
            no++;
            Node = insert(Node,i+1);
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        if(graph_c[i][node_1] != -1)
        {
            no++;
            Node = insert(Node,i+1);
        }
    }
    printf("Source Node : %d\n",(int)query1_node);
    printf("No of immediatte neighbours : %d\n",no);
    printf("Sorted IDs of neighbours : ");
    while(Node != NULL)
    {
        printf("%d\t",Node->node);
        Node = Node -> next;
    }
    printf("\n\n");
}
/*
    Function to generate Query 5, calculate it 
    and print it
*/
void query_5(int no_of_users, double graph_c[no_of_users][no_of_users])
{
    int i,j;
    double no = 0;
    for(i = 0; i<no_of_users;i++)
    {
        for (j = 0; j < no_of_users; j++)
        {
            if(graph_c[i][j] != -1)
            {
                no++;
            }
        }
    }
    printf("Average Degree of a node : %0.2f\n\n",no/no_of_users);
}
/*
    Function to find second level neighbours
*/
int findsecondlevel(int no_of_users, double graph_c[no_of_users][no_of_users], double query1_node)
{
    int no = 0;
    int node_1 = query1_node - 1;
    int i;
    nd *Node = NULL;
    for(i = 0;i < node_1;i++)
    {
        if(graph_c[i][node_1] != -1)
        {
            no++;
            Node = insert(Node,i+1);
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        if(graph_c[i][node_1] != -1)
        {
            no++;
            Node = insert(Node,i+1);
        }
    }
    nd * temp = Node;
    int Arr[no];
    for(i = 0;i<no;i++)
    {
        Arr[i] = temp->node;
        temp = temp -> next;
    }
    while(no != 0)
    {
        Node = findneighbours(Node,Arr[no-1]-1,no_of_users,graph_c,node_1);
        no--;

    }
    sort_list(Node);
    no = 0;
    nd * tmp = Node;
    while(tmp != NULL)
    {
        no++;
        tmp = tmp -> next;
    }
    return no;


}
/*
    Function to calculate Query 6
    and print it
*/
void query_6(int no_of_users, double graph_c[no_of_users][no_of_users])
{
    int i;
    double no = 0;
    for(i = 0; i<no_of_users;i++)
    {
        no += findsecondlevel(no_of_users,graph_c,i+1);
    }
    printf("Average degree of Second Level neighbours : %0.2f\n\n",no/no_of_users);
}

double distancecalc(Sng *sclnet, int node1 , int node2)
{
    Sng *temp = NULL;
    float distance;
    while(sclnet -> userID != node2)
    {
        if(sclnet -> userID == node1)
        {
            temp = sclnet;
        }
        sclnet = sclnet -> next;
    }
    distance = sqrt(pow((sclnet->age - temp->age),2) + pow((sclnet->gender - temp->gender),2) + pow((sclnet->martial_status - temp->martial_status),2) + pow((sclnet->race - temp->race),2) + pow((sclnet->birth - temp->birth),2) + pow((sclnet->language - temp->language),2) + pow((sclnet->occ - temp->occ),2) + pow((sclnet->income - temp->income),2));
    return distance;
}

void matrix_creation(int no_of_users,double matrix[no_of_users][no_of_users], Sng *sclnet)
{
    int i,j;
    float distance;
    printf("dfd\n");
    for(i = 0;i < no_of_users;i++)
    {
        for(j = 0;j < no_of_users; j++)
        {
            if( i == j)
            {
                matrix[i][j] = -1;
            }
            if(i<j)
            {
                distance = distancecalc(sclnet,i+1,j+1);
                matrix[i][j] = distance;
            }
            if(i>j)
            {
                matrix[i][j] = matrix[j][i];
            }
        }
    }
    float Lmax = 0;
    for(i = 0;i < no_of_users;i++)
    {
        for(j = 0;j < no_of_users; j++)
        {
            if(matrix[i][j] != -1 && matrix[i][j] > Lmax)
            {
                Lmax = matrix[i][j];
            }
        }
    }
    float Lab_doub;
    for(i = 0;i < no_of_users;i++)
    {
        for(j = 0;j < no_of_users; j++)
        {
            if(matrix[i][j] != -1)
            {
                Lab_doub = 1 - (matrix[i][j]/Lmax);
                Lab_doub = (int)(Lab_doub * 100);
                matrix[i][j] = Lab_doub / 100;
            }
        }
    }
}
int main(int argc, char ** argv) {    
	if(argc == 2){
		int a;
        int *p = &a;
		int i = 0;
        char s[10000];
		FILE *fptr = fopen(argv[1] ,"r");
        if(fptr == NULL)
        {
            exit(1);
        } 
        Sng *sclnet = NULL;
        int no_of_users = 0;
        double delta_1 = 0;
        double delta_2 = 0;
        double query1_node = 0;
        double alpha = 0;
		while( fgets(s, sizeof(s), fptr) != NULL){                         //scan entire file
            if(i == 0)
			{

				a = 5;
				char ** separated = explode(s,", \n",p);
                no_of_users = atof(separated[0]);
                delta_1 = atof(separated[1]);
                delta_2 = atof(separated[2]);
                query1_node = atof(separated[3]);
                alpha = atof(separated[4]);
			}
			else
			{
				a = 9;
				char ** separated = explode(s,", \n",p);
                sclnet = network_insert(sclnet,separated);
			}
            i += 1;
        }
        //double** matrix = (double**)malloc(no_of_users);
        double (*matrix)[no_of_users] = malloc(sizeof(double[no_of_users][no_of_users])); 
        //double (*gr_dense)[no_of_users] = malloc(sizeof(double[no_of_users][no_of_users]));
        //double (*gr_sparse)[no_of_users] = malloc(sizeof(double[no_of_users][no_of_users])); 
        
        //graph_creation(no_of_users,matrix,delta_2); 
        //double matrix[no_of_users][no_of_users];
        matrix_creation(no_of_users,matrix,sclnet);
        //printf("%f\n",delta_1 );
        graph_creation(no_of_users,matrix,delta_1);
        /*int j;
        printf("\n");
        for(i = 0;i<no_of_users;i++)
        {
            for(j=0;j<no_of_users;j++)
            {
                printf("%f ",matrix[i][j]);
            }
            printf("\n");
        }
           */ 
        printf("\n----------Dense Graph Data---------- \n\n" );    
        printf("Query 1 : \n\n");
        query_1(no_of_users,matrix,query1_node);
        printf("Query 2 : \n\n");
        query_2(no_of_users,matrix,query1_node,alpha);
        printf("Query 3 : \n\n");
        query_3(no_of_users,matrix,query1_node);
        printf("Query 4 : \n\n");
        query_4(no_of_users,matrix,query1_node);
        printf("Query 5 : \n\n");
        query_5(no_of_users,matrix);
        printf("Query 6 : \n\n");
        query_6(no_of_users,matrix);

        graph_creation(no_of_users,matrix,delta_2);
        printf("\n----------Sparse Graph Data---------- \n\n" );
        printf("Query 1 : \n\n");
        query_1(no_of_users,matrix,query1_node);
        printf("Query 2 : \n\n");
        query_2(no_of_users,matrix,query1_node,alpha);
        printf("Query 3 : \n\n");
        query_3(no_of_users,matrix,query1_node);
        printf("Query 4 : \n\n");
        query_4(no_of_users,matrix,query1_node);
        printf("Query 5 : \n\n");
        query_5(no_of_users,matrix);
        printf("Query 6 : \n\n");
        query_6(no_of_users,matrix);
        free(matrix);
        fclose(fptr);
	}
	else
	{
		printf("Wrong No. Of Arguements!!!\n");
	}
    return 0;
}
/*while(gr != NULL)
        {
            printf("%f\n",gr->Lab);
            gr = gr->next;

        }*/
        //int no;
        
/*int j;
for(i = 0;i<no_of_users;i++)
        {
            printf("%d ",i);
        }
        for(i = 1;i<no_of_users;i++)
        {
            printf("\n%d",i);
        }
        printf("\n");
        for(i = 0;i<no_of_users;i++)
        {
            for(j=0;j<no_of_users;j++)
            {
                printf("%f ",gr_sparse[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");
        for(i = 0;i<no_of_users;i++)
        {
            printf("%d ",i);
        }
        for(i = 1;i<no_of_users;i++)
        {
            printf("\n%d",i);
        }
        printf("\n");
        for(i = 0;i<no_of_users;i++)
        {
            for(j=0;j<no_of_users;j++)
            {
                printf("%f ",gr_dense[i][j]);
            }
            printf("\n");
        }*/
	
