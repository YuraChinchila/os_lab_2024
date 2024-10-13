#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>

#include <sys/time.h>

#include "utils.h"

struct SumArgs {
    int *array;
    int begin;
    int end;
};

int thread_index = 0;

long long Sum(const struct SumArgs *args) {
    long long sum = 0;
    // printf("%d %d;", args[thread_index].begin, args[thread_index].end);
    int begin = args[thread_index].begin;
    int end = args[thread_index].end;
    ++thread_index;
    for (int i = begin; i < end; ++i) {
        sum += args->array[i];
    }
    return sum;
}

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    pthread_exit((void *)Sum(sum_args));
    // return (void *)Sum(sum_args);
}

int main(int argc, char **argv) {
    /*
     *    TODO:
     *    threads_num by command line arguments
     *    array_size by command line arguments
     *	seed by command line arguments
     */

    int seed = -1;
    int array_size = -1;
    int threads_num = -1;

    while (1) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"threads_num", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        // srand(seed);
                        // your code here
                        // error handling
                        break;
                    case 1:
                        array_size = atoi(optarg);
                        // your code here
                        // error handling
                        break;
                    case 2:
                        threads_num = atoi(optarg);
                        // your code here
                        // error handling
                        break;
                    defalut:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || threads_num == -1) {
        printf("Usage: --threads_num num --seed num --array_size num\n");
        return 1;
    }

    pthread_t threads[threads_num];
    /*
        * TODO:
        * your code here
        * Generate array here
        */

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    struct SumArgs args[threads_num];
    int part_sz = (threads_num > 1) ? (array_size / threads_num) : (array_size);

    for (int i = 0; i < threads_num - 1; ++i) {
        args[i].array = array;
        args[i].begin = i * part_sz;
        args[i].end = args[i].begin + part_sz;
    }
    args[threads_num - 1].array = array;
    args[threads_num - 1].begin = (threads_num - 1) * part_sz;
    args[threads_num - 1].end = array_size;

    for (int i = 0; i < threads_num; i++) {
        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }

    long long total_sum = 0;
    for (int i = 0; i < threads_num; i++) {
        void* sum = 0;
        pthread_join(threads[i], &sum);
        total_sum += (long long)sum;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);
    // printf("\n");
    printf("Total: %lld\n", total_sum);
    printf("Elapsed time: %fms\n", elapsed_time);
    return 0;
}
