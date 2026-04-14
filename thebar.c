/*
 * thebar.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"
#include <sys/wait.h>

void printBanner();
void init();
void cleanup();

int num_threads;
int thread_args[] = {};
sem_t *rmink_mutex;
sem_t *order_placed;
sem_t *drink_made;
sem_t *cust_paid;
sem_t *pay_received;

/**
 * Main function
 */
int main(int argc, char *argv[]) {
	if (argc != 2){
		printf("Usage: ./thebar <num-threads>\n");
		return 0;
	}
	else{
		num_threads = atoi(argv[1]);
	}

	printBanner();
	
	//initialize semaphores
	init();

	//spawn and exec all customer threads
	pthread_t *cust_threads = malloc(num_threads * sizeof(pthread_t));
	for (int i = 0; i < num_threads; i++){
		thread_args[0] = i;
        pthread_create(&cust_threads[i], NULL, customer, thread_args);
    }

	//spawn and exec bartender thread
	pthread_t *bar_thread = malloc(sizeof(pthread_t));
	pthread_create(bar_thread, NULL, bartender, NULL);

	//wait on all customer threads
	for(int i = 0; i < num_threads; i++){
        pthread_join(cust_threads[i], NULL);
    }

	//wait on bartender thread
	pthread_join(*bar_thread, NULL);

	//cleanup and destroy semaphores
	cleanup();

	//free malloced space and dangling pointers
	free(cust_threads);
	cust_threads = NULL;

	free(bar_thread);
	bar_thread = NULL;

	return 0;
}

/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner() {
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("-----------------------------------------------------------\n");
}

/**
 * Create and initialize semaphores
 */
void init() {
	rmink_mutex = sem_open("/rmink_mutex", O_CREAT, 0600, 1);
	order_placed = sem_open("/order_placed", O_CREAT, 0600, 0);
	drink_made = sem_open("/drink_made", O_CREAT, 0600, 0);
	cust_paid = sem_open("/cust_paid", O_CREAT, 0600, 0);
	pay_received = sem_open("/pay_received", O_CREAT, 0600, 0);
}

/**
 * Cleanup and destroy semaphores
 */
void cleanup() {
	sem_close(rmink_mutex);
	sem_unlink("/rmink_mutex");

	sem_close(order_placed);
	sem_unlink("/order_placed");

	sem_close(drink_made);
	sem_unlink("/drink_made");

	sem_close(cust_paid);
	sem_unlink("/cust_paid");

	sem_close(pay_received);
	sem_unlink("/pay_received");
}
