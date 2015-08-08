#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>

int partition(int a[],int lower,int upper)
{
    int pivot,i,j,temp;
    pivot=a[lower];
    i=lower+1;
    j=upper;
    while(i<j)
    {
        while((i<upper)&&(a[i]<=pivot))
        i++;
        while((a[j]>pivot))
        j--;
        if(i<j)
        {
            temp=a[i];
            a[i]=a[j];
            a[j]=temp;
        }

    }//end while

    if(pivot>a[j])
    {
         temp=a[j];
         a[j]=a[lower];
         a[lower]=temp;
    }

    return(j);

}//end partition

void quick(int a[], int lower, int upper)
{
    while (lower < upper)
    {
        int loc = partition(a, lower, upper);
        if (loc - lower < upper - loc)
        { /* Lower part is shorter... */
            quick(a, lower, loc - 1); /* ...process it recursively... */
            lower = loc + 1; /* ...and process the upper part on the next iteration */
        }
        else
        { /* Upper part is shorter... */
            quick(a, loc + 1, upper); /* ...process it recursively... */
            upper = loc - 1; /* ...and process the lower part on the next iteration */
        }
    }
}

void insertion_sort(int *array, int offset, int end) {
	int x, y, temp;
	for (x=offset; x<end; ++x) {
		for (y=x; y>offset && array[y-1]>array[y]; y--) {
			temp = array[y];
			array[y] = array[y-1];
			array[y-1] = temp;
			}
	}
}

void radix_sort(int *array, int offset, int end, int shift) {
	int x, y, value, temp;
	int last[256] = { 0 }, pointer[256];
	for (x=offset; x<end; ++x) {
		++last[(array[x] >> shift) & 0xFF];
	}
	last[0] += offset;
	pointer[0] = offset;
	for (x=1; x<256; ++x) {
		pointer[x] = last[x-1];
		last[x] += last[x-1];
	}
	for (x=0; x<256; ++x) {
		while (pointer[x] != last[x]) {
			value = array[pointer[x]];
			y = (value >> shift) & 0xFF;
			while (x != y) {
				temp = array[pointer[y]];
				array[pointer[y]++] = value;
				value = temp;
				y = (value >> shift) & 0xFF;
			}
			array[pointer[x]++] = value;
		}
	}
	if (shift > 0) {
		shift -= 8;
		for (x=0; x<256; ++x) {
			temp = x > 0 ? pointer[x] - pointer[x-1] : pointer[0] - offset;
			if (temp > 64) {
				radix_sort(array, pointer[x] - temp, pointer[x], shift);
			} else if (temp > 1) {
				insertion_sort(array, pointer[x] - temp, pointer[x]);
			}
		}
	}
}

int intcmp(const void *aa, const void *bb)
{
	const int *a = (int *)aa, *b = (int *)bb;
	return (*a < *b) ? -1 : (*a > *b);
}

void modifed_sort_radix(int array[],int end)
{
	int i;
	int min = array[0];
	int forwrd = 0;
	int revers = 0;
	for(i = 0;i<end;i++)
    {
    	if(array[i] < min)
    	{
    		min = array[i];
    	}
    	if(i+1 != end)
    	{
    		if(array[i] > array[i+1])
    		{
    			revers++;
    		}
    		else
    		{
    			forwrd++;
    		}

    	}	
    }
 	i = 0;
 	if(revers == end-1)
 	{
 		int n = end - 1;
 		int temp;
 		while(i <= n)
 		{
 			temp = array[i];
 			array[i] = array[n];
 			array[n] = temp;
 			i++;
 			n--;
 		}

 		return;

 	}
 	if(forwrd == end-1)
 	{
 		return;
 	}
    if(min <= 0)
    {
	    min = min - 1;
	    for(i = 0;i<end;i++)
	    {
	    	array[i] = array[i] - min;
	    }
	}
    radix_sort(array, 0, end, 24);
	if(min <= 0)
	{
	    for(i = 0;i<end;i++)
	    {
	    	array[i] = array[i] + min;
	    }
	}
}

void sort(int list[], int len)
{
    if(len < 50)
    {
    	//modifed_sort_radix(list,len);
        insertion_sort(list,0,len);
    }
    else if(len >=50 && len<500)
    {
    	int i;
    	int forwrd = 0;
    	int revers = 0;
    	int end = len;
    	for(i = 0;i<end;i++)
	    {
	    	if(i+1 != end)
	    	{
	    		if(list[i] > list[i+1])
	    		{
	    			revers++;
	    		}
	    		else
	    		{
	    			forwrd++;
	    		}

	    	}	
	    }
	 	i = 0;
	 	if(revers == end-1)
	 	{
	 		int n = end - 1;
	 		int temp;
	 		while(i <= n)
	 		{
	 			temp = list[i];
	 			list[i] = list[n];
	 			list[n] = temp;
	 			i++;
	 			n--;
	 		}

	 		return;

	 	}
	 	if(forwrd == end-1)
	 	{
	 		return;
	 	}
        quick(list,0,len-1);
    }
    else
    {
    	modifed_sort_radix(list,len);
    }
}
