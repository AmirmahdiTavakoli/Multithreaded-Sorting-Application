#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int original_list[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int sorted_list[SIZE];

typedef struct {
    int* array;
    int start;
    int end;
} sort_params;

void bubble_sort(int* array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void* sort_thread(void* arg) {
    sort_params* params = (sort_params*)arg;
    bubble_sort(params->array + params->start, params->end - params->start);
    return NULL;
}

void* merge_thread(void* arg) {
    sort_params* params = (sort_params*)arg;
    int half = SIZE / 2;
    int i = 0, j = half, k = 0;
    
    while (i < half && j < SIZE) {
        if (params->array[i] < params->array[j]) {
            sorted_list[k++] = params->array[i++];
        } else {
            sorted_list[k++] = params->array[j++];
        }
    }
    
    while (i < half) {
        sorted_list[k++] = params->array[i++];
    }
    
    while (j < SIZE) {
        sorted_list[k++] = params->array[j++];
    }
    
    return NULL;
}

int main() {
    pthread_t threads[3];
    sort_params params[2];
    int half = SIZE / 2;
    
    printf("Original list: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", original_list[i]);
    }
    printf("\n");
    
    // First sorting thread (first half)
    params[0].array = original_list;
    params[0].start = 0;
    params[0].end = half;
    pthread_create(&threads[0], NULL, sort_thread, &params[0]);
    
    // Second sorting thread (second half)
    params[1].array = original_list;
    params[1].start = half;
    params[1].end = SIZE;
    pthread_create(&threads[1], NULL, sort_thread, &params[1]);
    
    // Wait for sorting threads to finish
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    
    printf("After sorting threads:\n");
    printf("First half: ");
    for (int i = 0; i < half; i++) {
        printf("%d ", original_list[i]);
    }
    printf("\nSecond half: ");
    for (int i = half; i < SIZE; i++) {
        printf("%d ", original_list[i]);
    }
    printf("\n");
    
    // Merge thread
    sort_params merge_param;
    merge_param.array = original_list;
    pthread_create(&threads[2], NULL, merge_thread, &merge_param);
    pthread_join(threads[2], NULL);
    
    printf("Sorted list: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_list[i]);
    }
    printf("\n");
    
    return 0;
}