
 
/*******************************************************************
* Recommended compiler options: *
* -v -O3 -march=native -funroll-loops *
* 250 million integers in the range [-2147483647,2147483647] 2.42s *
* 100 million integers in the range [-2147483647,2147483647] 0.98s *
*******************************************************************/
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>
//#include "project2.c"
 void sort(int *,int);
 
/*******************************************************************
* Flips all the bits if negative. *
* Flips only the sign bit if positive. *
* Alilows for both negative and positive values. *
* Returns one of the two bytes of the integer. *
*******************************************************************/
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

void generaterand(int a[],int n)
{
    int i;
   // int g = n;
    for(i = 0;i<n;i++)
    {
     //   a[i] = i;
    //    g--;
       a[i] = rand() % 1000000 - 1000000;    
    }
}
int main(void)
{
    int n = 1000000;
    int a[n];
    int i;

    srand(clock());
    generaterand(a,n);
    clock_t tic = clock();
    qsort(a, n, sizeof(int), cmpfunc);
    clock_t toc = clock();
    printf("Size : %d\n\n", n);
    for(i = 0;i<n;i++)
    {
        if(i != n-1)
            if(a[i] > a[i+1])
                printf("Problem");
    }
    printf("\nQsort:\n");
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    
    
    generaterand(a,n);
    tic = clock();
    sort(a,n);
    toc = clock();
    for(i = 0;i<n;i++)
    {
        if(i != n-1)
            if(a[i] > a[i+1])
                printf("Problem");
    }
    printf("\nMysort:\n" );
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);   
    return 0;
}
