#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_ORDERS_IN_KITCHEN 5
#define MAX_ORDERS_TO_SERVE 5
#define MAX_CUSTOMERS 10
#define MAX_TOTAL_ORDERS 20 // Example termination condition
//gcc -o main main.c -pthread
// Semaphore declarations
sem_t kitchen_mutex;
sem_t serving_mutex;
sem_t chef_semaphore;
sem_t customer_order_semaphore;

int orders_in_kitchen = 0;
int orders_to_serve = 0;
int orders_in_queue = 0;
int total_orders_served = 0;

// Function declarations
void *customer(void *arg);
void *chef(void *arg);
void *waiter(void *arg);
void place_order();
void prepare_order();
void serve_order();

int main() {
    // Initialize semaphores
    sem_init(&kitchen_mutex, 0, 1);
    sem_init(&serving_mutex, 0, 1);
    sem_init(&chef_semaphore, 0, 0);
    sem_init(&customer_order_semaphore, 0, MAX_CUSTOMERS);

    // Create threads for customers, chefs, and waiters
    pthread_t customer_threads[MAX_CUSTOMERS];
    pthread_t chef_thread, waiter_thread;

    // Create chef and waiter threads
    pthread_create(&chef_thread, NULL, chef, NULL);
    pthread_create(&waiter_thread, NULL, waiter, NULL);

    // Create customer threads
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_create(&customer_threads[i], NULL, customer, NULL);
    }

    // Wait for a specific number of orders to be served
    while (total_orders_served < MAX_TOTAL_ORDERS) {
        usleep(1000000); // Sleep for 1 second
    }

    // Terminate threads
    pthread_cancel(chef_thread);
    pthread_cancel(waiter_thread);
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_cancel(customer_threads[i]);
    }

    // Destroy semaphores
    sem_destroy(&kitchen_mutex);
    sem_destroy(&serving_mutex);
    sem_destroy(&chef_semaphore);
    sem_destroy(&customer_order_semaphore);

    return 0;
}

void *customer(void *arg) {
    while (1) {
        place_order();
        // Sleep to simulate time between orders
        usleep(rand() % 3000000);
    }
    return NULL;
}

void *chef(void *arg) {
    while (1) {
        prepare_order();
        sem_post(&chef_semaphore); // Signal waiter that order is prepared
        // Sleep to simulate time to prepare an order
        usleep(rand() % 2000000);
    }
    return NULL;
}

void *waiter(void *arg) {
    while (1) {
        sem_wait(&chef_semaphore); // Wait for order to be prepared
        serve_order();
        // Sleep to simulate time to serve an order
        usleep(rand() % 1000000);
    }
    return NULL;
}

void place_order() {
    sem_wait(&customer_order_semaphore);
    sem_wait(&kitchen_mutex);

    if (orders_in_kitchen < MAX_ORDERS_IN_KITCHEN) {
        orders_in_kitchen++;
        printf("Customer placed an order. Orders in kitchen: %d\n", orders_in_kitchen);
    } else {
        printf("Kitchen is full. Customer waits for space.\n");
    }

    sem_post(&kitchen_mutex);
}

void prepare_order() {
    sem_wait(&kitchen_mutex);

    if (orders_in_kitchen > 0) {
        orders_in_kitchen--;
        orders_in_queue++; // Increment orders in queue
        printf("Chef prepared an order. Orders in kitchen: %d\n", orders_in_kitchen);
    }

    sem_post(&kitchen_mutex);
}

void serve_order() {
    sem_wait(&serving_mutex);

    if (orders_in_queue > 0) {
        orders_in_queue--;
        total_orders_served++; // Increment total orders served
        printf("Waiter served an order. Orders in queue: %d\n", orders_in_queue);
    }

    sem_post(&serving_mutex);
    sem_post(&customer_order_semaphore); // Signal that customer can place another order
}
