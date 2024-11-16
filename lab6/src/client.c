#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "mymath.h"

#define h_addr h_addr_list[0]

struct Server {
	char ip[255];
	int port;
};

bool ConvertStringToUI64(const char *str, uint64_t *val) {
	char *end = NULL;
	unsigned long long i = strtoull(str, &end, 10);
	if (errno == ERANGE) {
		fprintf(stderr, "Out of uint64_t range: %s\n", str);
		return false;
	}

	if (errno != 0)
		return false;

	*val = i;
	return true;
}

typedef struct ThreadServerArgs{
    uint64_t begin;
    uint64_t end;
    uint64_t mod;
    struct Server to_server;
} ThreadServerArgs;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
uint64_t answer = 1;

void thread_server(void* args) {
	ThreadServerArgs* server_args = (ThreadServerArgs*)(args);
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		fprintf(stderr, "Can not create server socket!");
		exit(1);
	}
	struct hostent *hostname = gethostbyname(server_args->to_server.ip);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(server_args->to_server.port);
	server.sin_addr.s_addr = *((uint64_t*)hostname->h_addr);

	if (connect(server_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
		fprintf(stderr, "Can not connect to server!");
		exit(1);
	}

	uint64_t begin = server_args->begin;
    uint64_t end = server_args->end;
    uint64_t mod = server_args->mod;

	char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

	if (send(server_fd, task, sizeof(task), 0) < 0) {
    	fprintf(stderr, "Send failed\n");
      	exit(1);
    }

    printf("%s:%d send: %lu %lu %lu\n", server_args->to_server.ip, server_args->to_server.port, begin, end, mod);

	char response[sizeof(uint64_t)];
    if (recv(server_fd, response, sizeof(response), 0) < 0) {
    	fprintf(stderr, "Recieve failed\n");
      	exit(1);
    }

    uint64_t tmp_answer = 0;
    memcpy(&tmp_answer, response, sizeof(uint64_t));
    printf("%s:%d answer: %lu\n", server_args->to_server.ip, server_args->to_server.port, tmp_answer);
    
    pthread_mutex_lock(&mutex);
	answer = MultModulo(answer, tmp_answer, mod);
    pthread_mutex_unlock(&mutex);
    
    close(server_fd);
}

int main(int argc, char **argv) {
	uint64_t k = -1;
	uint64_t mod = -1;
	char servers[255] = {'\0'}; // TODO: explain why 255

	while (true) {
		int current_optind = optind ? optind : 1;

		static struct option options[] = {
			{"k", required_argument, 0, 0},
			{"mod", required_argument, 0, 0},
			{"servers", required_argument, 0, 0},
			{0, 0, 0, 0}
		};

		int option_index = 0;
		int c = getopt_long(argc, argv, "", options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 0: {
			switch (option_index) {
			case 0:
				ConvertStringToUI64(optarg, &k);
				// TODO: your code here
				break;
			case 1:
				ConvertStringToUI64(optarg, &mod);
				// TODO: your code here
				break;
			case 2:
				// TODO: your code here
				memcpy(servers, optarg, strlen(optarg));
				break;
			default:
				printf("Index %d is out of options\n", option_index);
			}
		} break;

		case '?':
			printf("Arguments error\n");
			break;
		default:
			fprintf(stderr, "getopt returned character code 0%o?\n", c);
		}
	}

	if (k == -1 || mod == -1 || !strlen(servers)) {
		fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
						argv[0]);
		return 1;
	}

	// TODO: for one server here, rewrite with servers from file
	unsigned int servers_num = 3;
	struct Server* to = malloc(sizeof(struct Server) * servers_num);
	FILE* fptr = fopen("servers.txt", "r");
	for (int i = 0; i < servers_num; ++i) {
		fscanf(fptr, "%s %d", to[i].ip, &to[i].port);
		printf("Server %s:%d\n", to[i].ip, to[i].port);
	}
	fclose(fptr);
	// TODO: delete this and parallel work between servers
	pthread_t threads[servers_num];
	ThreadServerArgs* args = (ThreadServerArgs*)malloc(sizeof(ThreadServerArgs) * servers_num);
	for (int i = 0, start = 1, step = k / servers_num; i < servers_num; ++i, start += step) {
		args[i].begin = start;
		args[i].end = (i < servers_num - 1) ? start + step : k;
		args[i].mod = mod;
		args[i].to_server = to[i];

		if (pthread_create(&threads[i], NULL, (void *)thread_server, (void *)(args + i))) {
			fprintf(stderr, "pthread_create failed");
			return 1;
		}
	}
	for (int i = 0; i < servers_num; ++i) 
    	pthread_join(threads[i], NULL);
	// TODO: work continiously, rewrite to make parallel
	printf("answer: %llu\n", answer);
	free(to);

	return 0;
}
