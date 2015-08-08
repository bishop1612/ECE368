/*
    ECE 368 Project 1 Part B
    Teammates : Bishop Sarkar and Kenji Inoue

    The program description is given in the README file.

*/

#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <stdlib.h> 
#include <math.h> 

typedef struct FutureEventList{
    struct FutureEventList * next;
    int arrival;
    int *servTime;
    int priority;
    int no_of_tasks;
}Fel;

typedef struct servqueue{
    int serv;
    struct servqueue *next;
}srv;

char ** explode(const char *, const char *, int *);
Fel * mode2_insert(Fel * , char **);
Fel * Node_mode2_construct(char **);
Fel * Fel_pop(Fel *,int,int,int);
srv * srv_construct(int);
srv * srvpush(srv *, int);
//srv * pop(srv *);
srv * sortList(srv *);
float servtime_avg(Fel *);
float servtime_min(Fel *);
float servtime_max(Fel *);
float servtime_total(Fel *);
void mode2(Fel *);
void printandwrite(double , double , double , double, double);

/*
    This is the structure that stores the arrival, departure, waiting and service time of each event 
*/

void printandwrite(double waiting_0, double waiting_1, double queue_length, double util,double load_factor)
{
    printf("\nECE 368 PROJECT 1 PART B OUTPUT (ALSO RIDIRECTED TO proj1_output)\n");
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");

    printf("Average Waiting Time for 0 tasks : %f\n", waiting_0);
    printf("Average Waiting Time for 1 tasks : %f\n", waiting_1);
    printf("Average Queue Length : %f\n", queue_length);
    printf("Average Utilization of CPU : %f\n",util);
    printf("Load factor : %f\n\n",load_factor);

    FILE *f = fopen("proj1_output", "w");
    if (f == NULL)
    {
        printf("Error creating write file!\n");
        exit(1);
    }
    fprintf(f,"Average Waiting Time for 0 tasks : %f\n", waiting_0);
    fprintf(f,"Average Waiting Time for 1 tasks : %f\n", waiting_1);
    fprintf(f,"Average Queue Length : %f\n", queue_length);
    fprintf(f,"Average Utilization of CPU : %f\n",util);
    fprintf(f,"Load factor : %f\n\n",load_factor);
    fclose(f);
    
}

srv * check(srv *serv)
{
    srv * tmp;
    tmp = serv;
    while(tmp != NULL)
    {
        tmp -> serv--;
        tmp = tmp -> next;
    }
    return serv;
}

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

Fel * Fel_pop(Fel * head,int prior,int arr,int tasks)
{
    //printf("HELLO\n");
    if(head == NULL)
    {
        return NULL;
    }
    if(head -> arrival == arr && head -> priority == prior && head -> no_of_tasks == tasks)
    {
        Fel * p = head -> next;
        free(head -> servTime);
        free(head);
        return p;
    }
    head -> next = Fel_pop(head -> next,prior,arr,tasks);
    return head;
}

int delete(srv * t)
{
    int process = 0;
    if(t != NULL)
    {
         while(t -> serv == 0)
        {
            //tmp = pop(tmp);
            process++;
            t = t -> next;
            if(t == NULL)
            {
                return process;
            }
        }
    } 
    return process;
}

srv * sortList(srv * list) 
{

    if(list == NULL || list->next == NULL)
        return list; // the list is sorted.

    //replace largest node with the first : 

    //1- find largest node : 
    srv *curr, *largest,*largestPrev,*prev;
    curr = list;
    largest = list;
    prev = list;
    largestPrev = list;
    while(curr != NULL) {
        if(curr->serv < largest->serv) {
            largestPrev = prev;
            largest = curr;
        }
        prev = curr;
        curr = curr->next;

    }
    //largest node is in largest. 

    //2- switching firt node and largest node : 
    srv * tmp;
    if(largest != list)
    {
        largestPrev->next = list;
        tmp = list->next;
        list->next = largest->next;
        largest->next = tmp;
    }
    largest->next = sortList(largest->next);
    return largest;
}

Fel * Queue_push(Fel *head,Fel *prev)
{
    if ( head == NULL )
    {
        head = malloc(sizeof(Fel));
        head -> arrival = prev -> arrival;
        head -> priority = prev -> priority;
        head -> no_of_tasks = prev -> no_of_tasks;
        int i = 0;
        head -> servTime = malloc(sizeof(head -> servTime) *  head -> no_of_tasks);
        for(i = 0; i < prev -> no_of_tasks; i++)
        {
            head -> servTime[i] = prev -> servTime[i];
        }
        head -> next = NULL;
        return head;
    }
    head -> next = Queue_push( head -> next , prev ) ;
    return head ;
}

Fel * sortfel(Fel * list) 
{

    if(list == NULL || list->next == NULL)
        return list; // the list is sorted.

    //replace largest node with the first : 

    //1- find largest node : 
    Fel *curr, *largest,*largestPrev,*prev;
    curr = list;
    largest = list;
    prev = list;
    largestPrev = list;
    while(curr != NULL) {
        if(curr->arrival < largest->arrival) {
            largestPrev = prev;
            largest = curr;
        }
        if(curr->arrival == largest->arrival && curr -> priority == 0)
        {
            largestPrev = prev;
            largest = curr;   
        }
        prev = curr;
        curr = curr->next;

    }
    //largest node is in largest. 

    //2- switching firt node and largest node : 
    Fel * tmp;
    if(largest != list)
    {
        largestPrev->next = list;
        tmp = list->next;
        list->next = largest->next;
        largest->next = tmp;
    }
    largest->next = sortfel(largest->next);
    return largest;
}

srv * srv_construct(int srvtime)
{
    srv * rel = malloc(sizeof(srv));
    rel -> serv = srvtime;
    rel -> next = NULL;
    return rel;
}

srv * srvpush(srv * head, int srvtime)
{
    if ( head == NULL )
    {
        return srv_construct (srvtime) ;
    }
    head -> next = srvpush ( head -> next , srvtime ) ;
    return head ;
}

srv * pop(srv * head,int srv_time) {
    if(head == NULL)
    {
        return NULL;
    }
    if(head -> serv == srv_time)
    {
        srv * p = head -> next;
        free(head);
        return p;
    }
    head -> next = pop(head -> next,srv_time);
    return head;
}


Fel * Node_mode2_construct(char ** separated)
{
    Fel * rel = malloc(sizeof(Fel));
    rel -> arrival = atof(separated[0]);
    rel -> priority = atof(separated[1]);
    rel -> no_of_tasks = atof(separated[2]);
    int n = 3;
    int i = 0;
    rel -> servTime = malloc(sizeof(rel -> servTime) *  rel -> no_of_tasks);
    for(i = 0; i < rel -> no_of_tasks; i++)
    {
        rel -> servTime[i] = atof(separated[n]);
        n++;
    }
    rel -> next = NULL;
    return rel;
}

/*
    Queue to insert elements in Queue    
*/
Fel * mode2_insert ( Fel * head , char ** separated )
{
    if ( head == NULL )
    {
        return Node_mode2_construct (separated) ;
    }
    head -> next = mode2_insert ( head -> next , separated ) ;
    return head ;
}

float servtime_total(Fel * fel)
{
    int total = 0;
    //int tasks = 0;
    int i = 0;
    while(fel != NULL)
    {
        for(i = 0;i<fel->no_of_tasks;i++)
        {
            total += fel->servTime[i];
        } 
        fel = fel->next; 
    }
    return total;
}


Fel * fel_insert(Fel * node, Fel * list){
    if(list == NULL) {
        return(node);
    }
    Fel * temp = list;
    Fel * prev = NULL;
    int done = 0;
    while(done == 0){
        if(temp->arrival < node->arrival){
            if(temp->next != NULL) {
                prev = temp;
                temp = temp->next;
            }
            else{
                temp->next = node;
                done = 1;
            }
        }
        if(temp->arrival == node->arrival)
        {
            if(temp->priority < node->priority)
            {
                Fel * temp2 = temp -> next;
                temp -> next = node;
                node -> next = temp2;
                done = 1;
            }
            else 
            {
                if(prev == NULL) {
                    node->next = temp;
                    list = node;
                    done = 1;
                }
                else
                {
                    prev->next = node;
                    node->next = temp;
                    done = 1;
                }
            }
        }
        if(temp->arrival > node->arrival)
        {
          if(prev == NULL) {
                node->next = temp;
                list = node;
                done = 1;
            }
            else
            {
                prev->next = node;
                node->next = temp;
                done = 1;
            }  
        }
    }
    return(list);
}

int genArrTime(double avArr){
    double x =  (double) rand()/RAND_MAX;
    double r = -((1/avArr) * (log(1-x)));
    return(ceil(r));
}

/*
    Function to generate several times
*/
int genServTime(double avServ){
    double x = (double) rand()/RAND_MAX;
    double r = -(1/avServ * log(1-x));
    return(ceil(r));
}

Fel * create_eventArr(int time, double avArr, double avServ){
    Fel * ret = malloc(sizeof(Fel));
    ret->arrival = time + genArrTime(avArr);
    ret->no_of_tasks = rand() % 32 + 1;
    int i = 0;
    ret -> servTime = malloc(sizeof(ret -> servTime) *  ret -> no_of_tasks);
    for(i = 0;i<ret->no_of_tasks;i++)
    {
        ret->servTime[i] = genServTime(avServ);
    }
    ret->next = NULL;
    return(ret);
}

Fel* mode1(int argc, char ** argv){
    double avArr0 = atof(argv[1]);
    double avArr1 = atof(argv[2]);
    double avServ = atof(argv[3]);
    double maxCust = atof(argv[4]);//inputted number of elements.

    
    int clock = 0;

    Fel * FEL_List = NULL;
    
    int numCust = 0;
    clock = 0;
    while(numCust < (maxCust)) {         //puts arrival time of all priority 0 items in FEL
        //Clock represents the current time
        Fel * newEvent = create_eventArr(clock, avArr0, avServ); //Creates a new arrival time stamp
        newEvent->priority = 0;
        clock = newEvent->arrival; //Updates the time stamp
        FEL_List = fel_insert(newEvent, FEL_List); //Inserts the imp. stuff into FEL
        numCust++;
    }
    numCust = 0;
    clock = 0;
    while(numCust < (maxCust)) {         //puts arrival time of all priority 0 items in FEL
        //Clock represents the current time
        Fel * newEvent = create_eventArr(clock, avArr1, avServ); //Creates a new arrival time stamp
        newEvent->priority = 1;
        clock = newEvent->arrival; //Updates the time stamp
        FEL_List = fel_insert(newEvent, FEL_List); //Inserts the imp. stuff into FEL
        numCust++;
    }
    return FEL_List;
}

float load_factor_cal(Fel *fel)
{
    float min,max,load,total;
    int i = 0;
    load = 0;
    while(fel != NULL)
    {
        max = fel -> servTime[0];
        min = fel -> servTime[0];
        total = 0;
        i = 0;
        while(fel -> no_of_tasks != i)
        {
            if(fel->servTime[i] < min)
            {
                min = fel->servTime[i];
            }
            if(fel->servTime[i] > max)
            {
                max = fel->servTime[i];
            }
            total += fel -> servTime[i];
            i++;
        }
        load += (max - min)/(total/fel->no_of_tasks);
        fel = fel -> next;
    }
    return load;
}

void mode2(Fel * fel)
{
    int processors = 0;
    srv *tmp = NULL;
    Fel *temp_arr = NULL;
    temp_arr = fel;
    Fel *queue_0 = NULL;
    Fel *queue_1 = NULL;
    int wait_0 = 0;
    int wait_1 = 0;
    while(temp_arr != NULL)
    {
        if(temp_arr -> priority == 0)
        {
            wait_0++;
        }
        else
        {
            wait_1++;
        }
        temp_arr = temp_arr -> next;
    }
    int queue_length = 0;
    int wait_0_time = 0;
    int wait_1_time = 0;
    float serv_total = servtime_total(fel);
    float load_factor = load_factor_cal(fel);
    load_factor = load_factor/(wait_1+wait_0);
    int i = 0;
    processors = 0;
    int del;
    int cur_clock = fel -> arrival;
    int avg = 0;
    Fel *queue_0_tmp;
    Fel *queue_1_tmp;
    while(fel != NULL || queue_0 != NULL || queue_1 != NULL || tmp != NULL)
    {
        while(fel != NULL && cur_clock == fel -> arrival)
        {
            if(fel -> priority == 0)
            {
                queue_0 = Queue_push(queue_0,fel);
            }
            else
            {
                queue_1 = Queue_push(queue_1,fel);
            }
            fel = Fel_pop(fel,fel -> priority, fel -> arrival, fel -> no_of_tasks);
        }
        if(queue_0 != NULL)
        {
            queue_0_tmp = queue_0;
            while(queue_0_tmp != NULL)
            {
                avg++;
                if((queue_0_tmp->no_of_tasks + processors) <= 64)
                {
                    wait_0_time += (cur_clock - queue_0_tmp -> arrival);
                    for(i = 0;i < queue_0_tmp->no_of_tasks;i++)
                    {
                        tmp = srvpush(tmp,queue_0_tmp->servTime[i]);
                    }
                    tmp = sortList(tmp);
                    processors += queue_0_tmp -> no_of_tasks;
                    queue_0 = Fel_pop(queue_0,queue_0_tmp -> priority,queue_0_tmp -> arrival,queue_0_tmp -> no_of_tasks);
                    queue_0_tmp = queue_0;
                    if(queue_0_tmp == NULL)
                    {
                        break;
                    }
                }
                else
                {
                    queue_length++;
                }
                queue_0_tmp = queue_0_tmp -> next;
            }
        }
        if(queue_1 != NULL)
        {
            queue_1_tmp = queue_1;
            while(queue_1_tmp != NULL)
            {
                avg++;
                if((queue_1_tmp->no_of_tasks + processors) <= 64)
                {
                    wait_1_time += (cur_clock - queue_1_tmp -> arrival);
                    for(i = 0;i < queue_1_tmp->no_of_tasks;i++)
                    {
                        tmp = srvpush(tmp,queue_1_tmp->servTime[i]);
                    }
                    tmp = sortList(tmp);
                    processors += queue_1_tmp -> no_of_tasks;
                    queue_1 = Fel_pop(queue_1,queue_1_tmp -> priority,queue_1_tmp -> arrival,queue_1_tmp -> no_of_tasks);
                    queue_1_tmp = queue_1;
                    if(queue_1_tmp == NULL)
                    {
                        break;
                    }
                }
                else
                {
                    queue_length++;
                }
                queue_1_tmp = queue_1_tmp -> next;
            }
        }
        tmp = check(tmp);
        del = delete(tmp);
        while(del != 0)
        {
            del--;
            processors -= 1;
            tmp = pop(tmp,0);
        }  
        cur_clock++;    
    }

    
    float avg_wait_0 = (wait_0_time/(float)wait_0);
    float avg_wait_1 = (wait_1_time/(float)wait_1);
    float avg_queue_length = queue_length/(float)(avg);
    float avg_util = (((float)serv_total)/(cur_clock))/64.0;
    printandwrite(avg_wait_0,avg_wait_1,avg_queue_length,avg_util,load_factor);
    
}

int main(int argc, char ** argv) {
    if(argc == 5) {
        Fel * FEL_list = mode1(argc, argv); 
        mode2(FEL_list);
    }
    else if(argc == 2) {
        int a = 3;
        int * p = &a;

       //char buffer for one line
        FILE *fptr = fopen(argv[1] ,"r");           //opening the file
        if(fptr == NULL)
        {
            printf("Error !! Reading File\n");
            return 0;
        }
        Fel * FEL_List = NULL;               //making the future event list
        char s[1000];
        while( fgets(s, sizeof(s), fptr) != NULL){                         //scan entire file
            char ** separated = explode(s," ",p);
            FEL_List = mode2_insert(FEL_List,separated);//puts event into the FEL
        }
        mode2(FEL_List);
        fclose(fptr);
    }
    else {
        printf("Error! : Wrong input!! Too few characters\n");
    }

    return 0;
}


/*
    Function to assign arrival times and service times
*/


