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
#define infinity 2000

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

void dij(int n,int v,int cost[n][n],int dist[])
{
    int i,u,count,w,flag[n],min;
    for(i=1;i<=n;i++)
    {
        flag[i]=0;
        dist[i]=cost[v][i];
    }
    count=2;
    while(count<=n)
    {
        min=99;
        for(w=1;w<=n;w++)
        {
            if(dist[w]<min && !flag[w])
            {
                min=dist[w];
                u=w;
            }
            flag[u]=1;
            count++;
            for(w=1;w<=n;w++)
                if((dist[u]+cost[u][w]<dist[w]) && !flag[w])
                    dist[w]=dist[u]+cost[u][w];
        }
    }
}
char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}


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

/*path function */
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
    //int preD[no_of_users];
    int distance[no_of_users];
    int matrix[no_of_users][no_of_users];
    int visited[no_of_users];
    int nextNode;
    int i,j;
    for(i = 0;i < no_of_users;i++)
    {
        visited[i] = 0;
        //preD[i] = 0;
        for (j = 0; j < no_of_users; j++)
        {
            if(graph_c[i][j] == -1)
                matrix[i][j] = 999;
            else
                matrix[i][j] = (int)(100*graph_c[i][j]);
        }
    }
    int node_1;
    node_1 = query1_node-1;
    for(i = 0; i<no_of_users;i++)
    {
        distance[i] = matrix[node_1][i];
    }
    visited[node_1] = 1;
    distance[node_1] = 0;
    int counter;
    int min = 999;
    for(counter = 0; counter < no_of_users; counter++){
            
            min = 999;
            
            for( i = 0; i < no_of_users; i++){
                
                if(min > distance[i] && visited[i]!=1){
                    
                    min = distance[i];
                    nextNode = i;
                    
                }
                
            }
            
            visited[nextNode] = 1;
            
            for(i = 0; i < no_of_users; i++){
                
                if(visited[i]!=1){
                    
                    if(min+matrix[nextNode][i] < distance[i]){
                        
                        distance[i] = min+matrix[nextNode][i];
                        //preD[i] = nextNode;
                        
                    }
                    
                }
                
            }    
        }
        for(i = 0;i<no_of_users;i++)
        {
            if(i == node_1)
                continue;

            if(distance[i] < (int)(alpha * 100))
            {
                no++;
            }
        }
    printf("%d\n",no);
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
    nd *Node = NULL;
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
    printf("%0.2f,",Lmax);
    for(i = 0;i < node_1;i++)
    {
        comp1 = (int)(graph_c[i][node_1]*100);
        comp2 = (int)(Lmax*100);
        if(comp1 == comp2)
        {

            Node = insert(Node,i+1);
        }
    }
    for(i = node_1 + 1;i < no_of_users;i++)
    {
        comp1 = (int)(graph_c[i][node_1]*100);
        comp2 = (int)(Lmax*100);
        if(comp1 == comp2)
        {
            Node = insert(Node,i+1);
        }
    }
    while(Node != NULL)
    {
        printf("%d",Node->node );
        Node = Node -> next;
        if(Node != NULL)
        {
            printf(",");
        }
    }
    printf("\n");

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
    while(tmp != NULL)
    {
        no++;
        tmp = tmp -> next;
    }
    if(no != 0 )
        printf("%d,",no);
    else
        printf("%d",no);
    while(Node != NULL)
    {
        printf("%d",Node->node);
        Node = Node -> next;
        if(Node != NULL)
        {
            printf(",");
        }
    }
    printf("\n");
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
    if(no != 0)
        printf("%d,",no);
    else
        printf("%d",no);  
    while(Node != NULL)
    {
        printf("%d",Node->node);
        Node = Node -> next;
        if(Node != NULL)
            printf(",");
    }
    printf("\n");
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
    printf("%0.2f\n",no/no_of_users);
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
    int i,j,k;
    int hop2[no_of_users][no_of_users];
    double no = 0;
    for(i = 0;i<no_of_users;i++)
    {
        for(j=0;j<no_of_users;j++)
        {
            for(k=0;k<no_of_users;k++)
            {
                if(i != j && i != k && graph_c[i][j] != -1 && graph_c[j][k] != -1 )
                {
                    hop2[i][k] = 1;
                }
            }
        }
    }
    for(i = 0;i<no_of_users;i++)
    {
        for(j=0;j<no_of_users;j++)
        {
            if(hop2[i][j] == 1)
            {
                no++;
            }            
        }
    }
    printf("%0.2f\n",no/no_of_users);
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
        double (*matrix)[no_of_users] = malloc(sizeof(double[no_of_users][no_of_users])); 
        matrix_creation(no_of_users,matrix,sclnet);
        graph_creation(no_of_users,matrix,delta_1);
        //query_2_test(no_of_users,matrix,query1_node);
        query_1(no_of_users,matrix,query1_node);
        query_2(no_of_users,matrix,query1_node,alpha);
        query_3(no_of_users,matrix,query1_node);
        query_4(no_of_users,matrix,query1_node);
        query_5(no_of_users,matrix);
        query_6(no_of_users,matrix);
        printf("\n");
        graph_creation(no_of_users,matrix,delta_2);
        query_1(no_of_users,matrix,query1_node);
        query_2(no_of_users,matrix,query1_node,alpha);
        query_3(no_of_users,matrix,query1_node);
        query_4(no_of_users,matrix,query1_node);
        query_5(no_of_users,matrix);
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
