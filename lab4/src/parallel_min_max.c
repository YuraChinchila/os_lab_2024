#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int pnum = -1;
pid_t* pids = NULL;

void alarm_handler(int) {
    for (int i = 0, status; i < pnum; ++i) {
        printf("Kill%d\n", pids[i]);
        kill(pids[i], SIGKILL);
    }
    exit(0);
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int timeout = -1;
    bool with_files = false;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
            {"timeout", required_argument, 0, 0},
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
                        pnum = atoi(optarg);
                        // your code here
                        // error handling
                        break;
                    case 3:
                        with_files = true;
                        break;
                    case 4:
                        timeout = atoi(optarg);
                        break;
                    defalut:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f':
                with_files = true;
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

    if (seed == -1 || array_size == -1 || pnum == -1 || timeout == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
                     argv[0]);
        return 1;
    }

    int* array = (int*)malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    int p[2];
    FILE* pf = NULL;
    if (with_files) {
        pf = fopen("pipe.txt", "w");
    } else {
        if (pipe(p) == -1) {
            return 1;
        }
    }

    pids = (pid_t*)malloc(pnum * sizeof(pid_t));

    for (int pi = 0; pi < pnum; pi++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            // successful fork
            active_child_processes += 1;
            if (child_pid == 0) {
                // child process
                // parallel somehow
                struct MinMax min_max;
                min_max.min = INT_MAX;
                min_max.max = INT_MIN;
                for (int i = pi; i < array_size; i += pnum) {
                    if (array[i] < min_max.min) {
                        min_max.min = array[i];
                    }
                    if (array[i] > min_max.max) {
                        min_max.max = array[i];
                    }        
                }
                printf("Finished execution [%d]\n", getpid());
                fflush(stdout);
                if (with_files) {
                    // use files here
                    fwrite(&min_max.min, sizeof(int), 1, pf);
                    fwrite(&min_max.max, sizeof(int), 1, pf);
                } else {
                    // use pipe here
                    write(p[1], &min_max.min, sizeof(int));
                    write(p[1], &min_max.max, sizeof(int));
                }
                return 0;
            } else {
                pids[pi] = child_pid;
            }
        } else {
            printf("Fork failed!\n");
            return 1;
        }
    }

    signal(SIGALRM, alarm_handler);
    alarm(timeout);

    printf("Parent [%d]\n", getpid());
    for (int i = 0; i < pnum; ++i) {
        printf("Child: [%d]\n", pids[i]);
    }
    fflush(stdout);
    
    while (wait(NULL) != -1 || errno != ECHILD);
    
    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;
    pf = fopen("pipe.txt", "r");
    for (int i = 0; i < pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;

        if (with_files) {
            // read from files
            fread(&min, 1, sizeof(int), pf);
            fread(&max, 1, sizeof(int), pf);
        } else {
            // read from pipes
            read(p[0], &min, sizeof(int));
            read(p[0], &max, sizeof(int));
        }

        if (min < min_max.min) min_max.min = min;
        if (max > min_max.max) min_max.max = max;
    }
    close(p[0]);
    if (pf != NULL) {
        fclose(pf);
    }
    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    free(pids);
    return 0;
}
