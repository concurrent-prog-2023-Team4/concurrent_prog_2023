#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>

// Merges two subarrays of arr[]
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    // Create temp arrays 
    int L[n1], R[n2]; 
  
    // Copy data to temp arrays 
    // L[] and R[] 
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
  
    // Merge the temp arrays back 
    // into arr[l..r] 
    // Initial index of first subarray 
    i = 0; 
  
    // Initial index of second subarray 
    j = 0; 
  
    // Initial index of merged subarray 
    k = l; 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    // Copy the remaining elements 
    // of L[], if there are any 
    while (i < n1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    // Copy the remaining elements of 
    // R[], if there are any 
    while (j < n2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
// l is for left index and r is 
// right index of the sub-array 
// of arr to be sorted 
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) { 
        // Same as (l+r)/2, but avoids 
        // overflow for large l and h 
        int m = l + (r - l) / 2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m + 1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

void *thread_func(void *varg) 
{   
    int i;
    int size = 0;
    int *array = (int *)varg;

    for (i = 0; (array+i) != NULL; i++ )
    {
        size++ ;  
    }
    printf("size is %d\n", size);
    // int size = sizeof(array) / sizeof(array[0]);
    // array1 = array/2

    // array2 = array/2

    // pthread_create(array1)
    // pthread_create(array2)
    // // if(size < 64)
    // // {
    // //     sort
    // // }
    // //else
    
    // //{
    //     // open 2 threads
    //     // merge
        
    // //}
    // //return
    // pthread_create();
}

int main(int argc, char *argv[]) 
{
    int i = 0, j;
    FILE *input;
    input = fopen(argv[1], "r");
    int *number = NULL;    // dynamic array for input //
    pthread_t id;

    number = (int *) malloc(sizeof(int) * 1);

    while (fscanf(input, "%d", &number[i]) == 1) 
    {
        // Process the read integer, for example, print it
        //printf("Read: %d\n", number[i]);
        i++;
        number = (int *) realloc(number, sizeof(int) * (i+1));
    }

    for(j = 0; j < i; j++)
    {
        printf("Result is %d\n", number[j]);
    }

    pthread_create(&id, NULL, thread_func, (void *) number);

    free(number);    

    return 0;
}