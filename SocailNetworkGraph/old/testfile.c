#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <stdlib.h> 
#include <math.h> 

int main(int argc, char ** argv) {
    FILE *f = fopen("test_me.txt", "w");
    if (f == NULL)
    {
        printf("Error creating write file!\n");
        exit(1);
    }
    int i;
    fprintf(f,"1000 .5 .7 1 1.5\n");
    for(i = 0;i<1000;i++)
    {
        fprintf(f,"%d ",i+1);
        fprintf(f,"%d ",rand() % 60 + 1);
        fprintf(f,"%d ",rand() % 5 + 1);
        fprintf(f,"%d ",rand() % 10 + 1);
        fprintf(f,"%d ",rand() % 2 + 1);
        fprintf(f,"%d ",rand() % 2 + 1);
        fprintf(f,"%d ",rand() % 2 + 1);
        fprintf(f,"%d ",rand() % 15 + 1);
        fprintf(f,"%d ",rand() % 10000 + 1);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}