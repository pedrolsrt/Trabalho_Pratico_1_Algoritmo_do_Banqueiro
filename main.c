#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3
#define ITERATIONS 10

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_mutex_t mutex;

void print_vector(const int vec[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d", vec[i]);
        if (i < NUMBER_OF_RESOURCES - 1) printf(" ");
    }
}

void print_state() {
    printf("\n========== ESTADO DO SISTEMA ==========\n");
    printf("Available: ");
    print_vector(available);
    printf("\n");

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("Cliente %d | Allocation: ", i);
        print_vector(allocation[i]);
        printf(" | Maximum: ");
        print_vector(maximum[i]);
        printf(" | Need: ");
        print_vector(need[i]);
        printf("\n");
    }
    printf("======================================\n");
}

int is_safe() {
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS] = {0};

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];

    int progress;
    do {
        progress = 0;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                int possible = 1;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        possible = 0;
                        break;
                    }
                }
                if (possible) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                        work[j] += allocation[i][j];
                    finish[i] = 1;
                    progress = 1;
                }
            }
        }
    } while (progress);

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        if (!finish[i]) return 0;

    return 1;
}

int request_resources(int customer_num, int request[]) {
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i] || request[i] > available[i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    if (!is_safe()) {
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

int release_resources(int customer_num, int release[]) {
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (release[i] > allocation[customer_num][i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        allocation[customer_num][i] -= release[i];
        available[i] += release[i];
        need[customer_num][i] += release[i];
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

void *customer(void *param) {
    int id = *((int *)param);

    for (int iter = 0; iter < ITERATIONS; iter++) {
        sleep(1);

        int request[NUMBER_OF_RESOURCES] = {0};

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            pthread_mutex_lock(&mutex);
            int current_need = need[id][i];
            pthread_mutex_unlock(&mutex);

            if (current_need > 0)
                request[i] = rand() % (current_need + 1);
        }

        printf("\nCliente %d solicitando: ", id);
        print_vector(request);

        if (request_resources(id, request) == 0)
            printf(" -> aprovado\n");
        else
            printf(" -> negado\n");

        pthread_mutex_lock(&mutex);
        print_state();
        pthread_mutex_unlock(&mutex);

        sleep(1);

        int release[NUMBER_OF_RESOURCES] = {0};

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            pthread_mutex_lock(&mutex);
            int current_allocation = allocation[id][i];
            pthread_mutex_unlock(&mutex);

            if (current_allocation > 0)
                release[i] = rand() % (current_allocation + 1);
        }

        printf("\nCliente %d liberando: ", id);
        print_vector(release);

        if (release_resources(id, release) == 0)
            printf(" -> liberado\n");
        else
            printf(" -> erro\n");

        pthread_mutex_lock(&mutex);
        print_state();
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void init() {
    srand(time(NULL));

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != NUMBER_OF_RESOURCES + 1) {
        printf("Uso: %s <R1> <R2> <R3>\n", argv[0]);
        return 1;
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = atoi(argv[i + 1]);

    pthread_mutex_init(&mutex, NULL);

    init();
    print_state();

    pthread_t threads[NUMBER_OF_CUSTOMERS];
    int ids[NUMBER_OF_CUSTOMERS];

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, customer, &ids[i]);
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}