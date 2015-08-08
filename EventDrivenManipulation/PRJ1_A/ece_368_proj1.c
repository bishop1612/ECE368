/*
    ECE 368 Project 1 Part 1
    Teammates : Bishop Sarkar and Kenji Inoue

    The program description is given in the README file.

*/

#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <stdlib.h> 
#include <math.h> 

/*
    This is the structure that stores the arrival, departure, waiting and service time of each event 
*/
typedef struct FutureEventList{
    struct FutureEventList * next;
    int eventTime;
    double waiting;
    double depTime;
    double servTime;
    int priority;
    int depArr;
}Fel;

/*
    FUNCTION DEFINETIONS
*/
int genArrTime(double );
double genServTime(double );
Fel * fel_insert(Fel * , Fel * );
Fel * mode2_insert(Fel * , int , double , double );
Fel * Node_construct(int , double , double );
Fel * create_eventArr(double , double , double , int );
Fel * pop(Fel * );
Fel * cal_depar_wait(Fel * );
void mode1(int , char ** );
void mode2(Fel *);
char * * explode(const char *, const char *, int *);
void printandwrite(double , double , double , double );

/*
    Function to explode the strings in the text file in MODE 2
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
    Function to calculate depature and waiting times of each event
*/

Fel * cal_depar_wait(Fel * FEL_List)
{
    FEL_List -> depTime = FEL_List -> eventTime + FEL_List -> servTime;
    FEL_List -> waiting = 0;
    double dep = FEL_List -> depTime;
    FEL_List = FEL_List -> next;
    while(FEL_List != NULL)
    {
        if(dep > FEL_List -> eventTime)
        {
            FEL_List -> waiting = (dep - FEL_List -> eventTime);
        }
        FEL_List -> depTime = FEL_List -> servTime + FEL_List -> eventTime + FEL_List -> waiting;
        dep = FEL_List -> depTime;
        FEL_List = FEL_List -> next;
    }
    return FEL_List;
}
/*
    Function to pop the queue
*/
Fel * pop(Fel * list) {
    Fel * tmp = NULL;
    if(list->next != NULL) {
        tmp = list->next;
    }
    free(list);
    return(tmp);
}
/*
    Function to generate arrival times
*/
int genArrTime(double avArr){
    double x = (double) rand()/RAND_MAX;
    double r = 1/avArr * log(1-x);
    double f = avArr * exp(avArr * -1 * r);
    return(ceil(f));
}

/*
    Function to generate several times
*/
double genServTime(double avServ){
    double x = (double) rand()/RAND_MAX;
    double r = 1/avServ * log(1-x);
    double f = avServ * exp(avServ * -1 * r);
    return(f);
}

/*
    Function to assign arrival times and service times
*/

Fel * create_eventArr(double time, double avArr, double avServ, int depArr){
    Fel * ret = malloc(sizeof(Fel));
    ret->eventTime = time + genArrTime(avArr);
    ret->servTime = genServTime(avServ);
    ret->depTime = 0;
    ret->waiting = 0;
    ret->depArr = depArr;
    ret->next = NULL;
    return(ret);
}

/*
    Function to sort the queue based on arrival times
*/

Fel * fel_insert(Fel * node, Fel * list){
    if(list == NULL) {
        return(node);
    }
    Fel * temp = list;
    Fel * prev = NULL;
    int done = 0;
    while(done == 0){
        if(temp->eventTime < node->eventTime){
            if(temp->next != NULL) {
                prev = temp;
                temp = temp->next;
            }
            else{
                temp->next = node;
                done = 1;
            }
        }
        if(temp->eventTime == node->eventTime)
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
        if(temp->eventTime > node->eventTime)
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

/*
    Node to insert elements in the queue for Mode 2
*/
Fel * Node_construct(int prior, double arr, double serv)
{
    Fel * rel = malloc(sizeof(Fel));
    rel -> priority = prior;
    rel -> eventTime = arr;
    rel -> servTime = serv;
    rel -> waiting = 0;
    rel -> depTime = 0;
    return rel;
}

/*
    Queue to insert elements in Queue    
*/
Fel * mode2_insert ( Fel * head , int prior, double arr, double serv )
{
    if ( head == NULL )
    {
    return Node_construct ( prior,arr, serv) ;
    }
    head -> next = mode2_insert ( head -> next , prior,arr, serv ) ;
    return head ;
}


/*
    Printing all the waiting times of 0 and 1, average queue length, CPU utilization
*/
void printandwrite(double waiting_0, double waiting_1, double queue_length, double util)
{
    printf("\nECE 368 PROJECT 1 PART A OUTPUT (ALSO RIDIRECTED TO proj-a_output)\n");
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");

    printf("Average Waiting Time for 0 tasks : %f\n", waiting_0);
    printf("Average Waiting Time for 1 tasks : %f\n", waiting_1);
    printf("Average Queue Length : %f\n", queue_length);
    printf("Average Utilization of CPU : %f\n\n",util);

    FILE *f = fopen("proj-a_output", "w");
    if (f == NULL)
    {
        printf("Error creating write file!\n");
        exit(1);
    }
    fprintf(f,"Average Waiting Time for 0 tasks : %f\n", waiting_0);
    fprintf(f,"Average Waiting Time for 1 tasks : %f\n", waiting_1);
    fprintf(f,"Average Queue Length : %f\n", queue_length);
    fprintf(f,"Average Utilization of CPU : %f\n",util);
    fclose(f);
    
}

/*
    Function to do al functions for Mode 1
*/
void mode1(int argc, char ** argv){
    double avArr0 = atof(argv[1]);
    double avArr1 = atof(argv[2]);
    double avServ = atof(argv[3]);
    double maxCust = atof(argv[4]);//inputted number of elements.

    
    int clock = 0;

    Fel * FEL_List = NULL;
    Fel * queue_wait = NULL;
    Fel * queue_length_cal = NULL;
    Fel * queue4 = NULL;
    
    int numCust = 0;
    //1 represents arrival
    //0 represents departure
    clock = 0;
    while(numCust < (maxCust)) {         //puts arrival time of all priority 0 items in FEL
        //Clock represents the current time
        Fel * newEvent = create_eventArr(clock, avArr0, avServ, 1); //Creates a new arrival time stamp
        newEvent->priority = 0;
        clock = newEvent->eventTime; //Updates the time stamp
        FEL_List = fel_insert(newEvent, FEL_List); //Inserts the imp. stuff into FEL
        numCust++;
    }
    clock = 0;
    numCust = 0;
    while(numCust < (maxCust)) {         //puts arrival time of all priority 1 items in FEL
        Fel * newEvent = create_eventArr(clock, avArr1, avServ, 1);
        newEvent->priority = 1;
        clock = newEvent->eventTime;
        FEL_List = fel_insert(newEvent, FEL_List);
        numCust++;
    }
    queue_wait = FEL_List;
    queue_length_cal = FEL_List;
    FEL_List = cal_depar_wait(FEL_List);
    
    double waiting_0 = 0;
    double waiting_1 = 0;
    double service = 0;
    double depar;

    while(queue_wait != NULL)
    {   
        if(queue_wait -> priority == 0)
        {
            waiting_0 = waiting_0 + queue_wait -> waiting;
        }
        else
        {
            waiting_1 = waiting_1 + queue_wait -> waiting;
        }
        service = service + queue_wait -> servTime;
        depar = queue_wait -> depTime;
        queue_wait = queue_wait -> next;
    }
    waiting_0 = waiting_0/maxCust;
    waiting_1 = waiting_1/maxCust;
    int cur_time;
    double queue_length = 0;
    double avg = 0;
    while(queue_length_cal != NULL)
    {
        cur_time = 0;
        queue4 = queue_length_cal;
        while(cur_time <= queue_length_cal -> servTime)
        {
            if(queue4 -> next != NULL)
                queue4 = queue4 -> next;
            else
                break;
            if(queue4 -> eventTime <= queue_length_cal -> depTime)
            {
                queue_length++;
            }
            avg++;
            cur_time++;
        }
        queue_length_cal = pop(queue_length_cal);
        if(queue_length_cal != NULL)
            queue_length_cal = queue_length_cal -> next;
    }
    queue_length = queue_length/avg;
    double util = service/depar;
    printandwrite( waiting_0,  waiting_1,  queue_length,  util);
}

/*
    Function to do al functions for Mode 2
*/
void mode2(Fel * FEL_List)
{
    Fel * queue_wait = NULL; 
    Fel * queue_length_cal = NULL; 
    Fel * queue4 = NULL;

    queue_wait = FEL_List;
    queue_length_cal = FEL_List;
    FEL_List = cal_depar_wait(FEL_List);
    double waiting_0 = 0;
    double waiting_1 = 0;
    int wait_0 = 0;
    int wait_1 = 0;
    double service = 0;
    double depar;
    
    while(queue_wait != NULL)
    {
        if(queue_wait -> priority == 0)
        {
            waiting_0 = waiting_0 + queue_wait -> waiting;
            wait_0++;
        }
        else
        {
            waiting_1 = waiting_1 + queue_wait -> waiting;
            wait_1++;
        }
        service = service + queue_wait -> servTime;
        depar = queue_wait -> depTime;
        queue_wait = queue_wait -> next;
    }
    if(wait_0 == 0)
        waiting_0 = 0;
    else
        waiting_0 = waiting_0/wait_0;
    
    if(wait_1 == 0)
        waiting_1 = 0;
    else
        waiting_1 = waiting_1/wait_1;

    int cur_time;
    double queue_length = 0;
    double avg = 0;
    while(queue_length_cal != NULL)
    {
        if(wait_0 + wait_1 == 1)
        {
            queue_length = 0;
            avg = 1;
            break;
        }
        cur_time = 0;
        queue4 = queue_length_cal;
        while(cur_time <= queue_length_cal -> servTime)
        {
            if(queue4 -> next != NULL)
                queue4 = queue4 -> next;
            else
                break;
            if(queue4 -> eventTime <= queue_length_cal -> depTime)
            {
                queue_length++;
            }
            avg++;
            cur_time++;
        }
        queue_length_cal = pop(queue_length_cal);
        if(queue_length_cal != NULL)
            queue_length_cal = queue_length_cal -> next;
    }

    queue_length = queue_length/avg;
    double util = service/depar;
    printandwrite( waiting_0,  waiting_1,  queue_length,  util);
}


int main(int argc, char ** argv) {
    if(argc == 5) {
        mode1(argc, argv);
    }
    else if(argc == 2) {
        //mode 2
        int prior;
        double arr;
        double serv;
        int a = 3;
        int * p = &a;

       // char * s = malloc(sizeof(char) * 5);       //char buffer for one line
        FILE *fptr = fopen(argv[1] ,"r");           //opening the file
        if(fptr == NULL)
        {
            printf("Error !! Reading File\n");
            return 0;
        }
        Fel * FEL_List = NULL;                   //making the future event list
         char s[1000];
        while( fgets(s, sizeof(s), fptr) != NULL){                         //scan entire file
           
        	char ** separated = explode(s, " ", p);
       		arr = atof(separated[0]);
        	 prior = atof(separated[1]);
        	serv = atof(separated[2]);
	//printf("%s",s);
	//break;
		//if(s == "\n")
			//printf("1");
         	FEL_List = mode2_insert(FEL_List,prior,arr,serv);//puts event into the FEL
        }
	
      	 mode2(FEL_List);
       // free(s);
        fclose(fptr);
    }
    else {
        printf("Error! : Wrong input!! Too few characters\n");
    }

    return 0;
}


/*
        TEST CODE : IGNORE!!!! 
printf("priority%d\n", queue_wait -> priority);
printf("Waiting : %f\n", queue_wait -> waiting);
printf("arrival : %d\n", queue_wait -> eventTime);
printf("service : %f\n", queue_wait -> servTime);
printf("departure : %f\n", queue_wait -> depTime);

*/









