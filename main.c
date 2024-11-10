#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables
int *array;
int *sorted_array;
int n;

// Structure for sorting range
typedef struct
{
    int start, end;
} SortRange;

// Comparison function for sorting
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Thread function for sorting a sublist
void *sort_sublist(void *arg)
{
    SortRange *range = (SortRange *)arg;
    qsort(&array[range->start], range->end - range->start + 1, sizeof(int), compare);
    return NULL;
}

// Merging function
void *merge(void *arg)
{
    int mid = n / 2;
    int i = 0, j = mid, k = 0;

    // Merge two halves
    while (i < mid && j < n)
    {
        if (array[i] < array[j])
        {
            sorted_array[k++] = array[i++];
        }
        else
        {
            sorted_array[k++] = array[j++];
        }
    }
    while (i < mid)
        sorted_array[k++] = array[i++];
    while (j < n)
        sorted_array[k++] = array[j++];

    return NULL;
}

int main()
{
    // Initialize array and size
    n = 10;
    int sample[] = {12, 4, 24, 7, 3, 11, 43, 34, 22, 10};
    array = sample;
    sorted_array = (int *)malloc(n * sizeof(int));

    // Print original array
    printf("Original array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Define ranges for sorting threads
    SortRange range1 = {0, n / 2 - 1}; // First half
    SortRange range2 = {n / 2, n - 1}; // Second half

    // Create sorting threads
    pthread_t sort_thread1, sort_thread2;
    pthread_create(&sort_thread1, NULL, sort_sublist, &range1);
    pthread_create(&sort_thread2, NULL, sort_sublist, &range2);

    // Wait for sorting threads to finish
    pthread_join(sort_thread1, NULL);
    pthread_join(sort_thread2, NULL);

    // Create and wait for merging thread
    pthread_t merge_thread;
    pthread_create(&merge_thread, NULL, merge, NULL);
    pthread_join(merge_thread, NULL);

    // Output sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    // Clean up
    free(sorted_array);
    return 0;
}
