// Thank you lali you are the best professor we could wish for //
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>

int *numbers;
int finish;

struct lalis_crew
{
    int left, right, finish;
};

void selectionSort(int arr[], int start, int end) {
    for (int i = start; i < end; i++) {
        int min_index = i;
        for (int j = i + 1; j <= end; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        // Swap the found minimum element with the element at index i
        int temp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = temp;
    }
}
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
  

void *thread_func(void *varg) 
{   
    struct lalis_crew *ptr = (struct lalis_crew *)varg;
    struct lalis_crew *ptrRight = NULL;
    struct lalis_crew *ptrLeft = NULL;

    int size = ptr->right - ptr->left;
    
    if (size <= 64)
    {
        //sort
        selectionSort(numbers, ptr->left, ptr->right);
    }
    else
    {
        pthread_t left, right;

        ptrLeft = (struct lalis_crew *) calloc(1, sizeof(struct lalis_crew));
        ptrLeft->left = ptr->left ;     // new left for thread
        ptrLeft->right = ptr->left + size / 2;
        ptrLeft->finish = 0;

        ptrRight = (struct lalis_crew *) calloc(1, sizeof(struct lalis_crew));
        ptrRight->left = ptr->left + size / 2 + 1;     // new left for thread  
        ptrRight->right = ptr->right;
        ptrRight->finish = 0;

        //sleep(5);
        pthread_create(&left, NULL, thread_func, (void *) ptrLeft);
        pthread_create(&right, NULL, thread_func, (void *) ptrRight);
        // printf("Thread created\n");

        
        while (ptrLeft->finish == 0 || ptrRight->finish == 0);

        merge(numbers, ptrLeft->left, ptrLeft->left + size / 2, ptrRight->right);     
    }
    
    ptr->finish = 1;
    free(ptrLeft);
    free(ptrRight);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    int i = 0;
    FILE *input;
    input = fopen(argv[1], "r");
    pthread_t id;
    int size = 0;
    finish = 0;
    struct lalis_crew* ptr = NULL;
    
    numbers = (int *) malloc(sizeof(int) * 1);

    if(argc < 2)
    {
        printf("No input file\n");
        
        return 1;
    }

    while (fscanf(input, "%d", &numbers[i]) == 1) 
    {
        // Process the read integer, for example, print it
        i++;
        numbers = (int *) realloc(numbers, sizeof(int) * (i+1));
    }
    size = i;
    
    ptr = (struct lalis_crew *) calloc(1, sizeof(struct lalis_crew));
    ptr->left = 0;
    ptr->right = size -1;
    ptr->finish = 0;

    pthread_create(&id, NULL, thread_func, (void*) ptr);

    while(ptr->finish == 0);

    printf("The sorted array is: \n");
    for (i = 0; i < size; i++) 
    {
        printf(" %d\n", numbers[i]);
    }
    printf("\n");
    
    free(ptr);    
    return 0;
}