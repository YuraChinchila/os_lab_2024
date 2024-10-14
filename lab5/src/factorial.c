#include <stdio.h>
#include <pthread.h>
#include <getopt.h>
#include <stdlib.h>

#include <sys/time.h>

typedef struct params {
    int start;
    int end;
    int mod;
} params;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int fact = 1;

void product(params param) {
    for (int i = param.start; i < param.end; ++i) {
        pthread_mutex_lock(&mutex);
        fact = (1ll * fact * i) % param.mod;
        pthread_mutex_unlock(&mutex);
    }
}

void* thread_func(void* param) {
    product(*(params*)(param));
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int k = -1;
    int pnum = -1;
    int mod = -1;

    while (1) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"k", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"mod", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        k = atoi(optarg);
                        break;
                    case 1:
                        pnum = atoi(optarg);
                        break;
                    case 2:
                        mod = atoi(optarg);
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

    if (k == -1 || pnum == -1 || mod == -1) {
        printf("Usage: --k num --pnum num --mod num\n");
        return 1;
    }

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    pthread_t threads[pnum];
    params params[pnum];

    for (int i = 0, start = 1, step = k / pnum; i < pnum; i++, start += step) {
        params[i].start = start;
        params[i].end = (i != pnum - 1) ? (start + step) : (k + 1);
        params[i].mod = mod;
        if (pthread_create(&threads[i], NULL, thread_func, (void *)&params[i])) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }

    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    printf("Total: %d\n", fact);
    printf("Elapsed time: %fms\n", elapsed_time);
    return 0;
}
