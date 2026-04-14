/*
 * customer.c
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"

/**
 * This is what the thread will call.
 * Do not touch.
 */
void *customer(void *args) {
	unsigned int custID = *((unsigned int *)args);
	custTravelToBar(custID);
	custArriveAtBar(custID);
	custPlaceOrder(custID);
	custBrowseArt(custID);
	custAtRegister(custID);
	custLeaveBar(custID);
	return NULL;
}

/**
 * Each customer takes a random amount of time between 20 ms and 5000 ms to travel to the bar.
 */
void custTravelToBar(unsigned int custID) {
	useconds_t cust_travel = ((rand() % (5000 - (20 + 1))) + 20) * 1000;
	usleep(cust_travel);
	printf("Cust %u\t\t\t\t\t\t\t\t\t\t\t|\n", custID);
}

/**
 * If there is already another customer in the bar the current customer has
 * to wait until bar is empty before entering.
 */
void custArriveAtBar(unsigned int custID) {
	sem_wait(rmink_mutex);
	printf("\t\tCust %u\t\t\t\t\t\t\t\t\t|\n", custID);
}

/**
 * The customer in the bar places an order
 */
void custPlaceOrder(unsigned int custID) {
	sem_post(order_placed);
	printf("\t\t\t\tCust %u\t\t\t\t\t\t\t|\n", custID);
}

/**
 * The customer in the bar can browse the wall art for a random amount of time between 3ms and 4000ms.
 */
void custBrowseArt(unsigned int custID) {
	useconds_t cust_browse = ((rand() % (4000 - (3 + 1))) + 3) * 1000;
	usleep(cust_browse);
	printf("\t\t\t\t\t\tCust %u\t\t\t\t\t|\n", custID);
}

/**
 * If their drink is not ready by the time they are done admiring the art they must wait
 * until the bartender has finished. When the bartender is finished, the customer pays.
 */
void custAtRegister(unsigned int custID) {
	sem_wait(drink_made);
	sem_post(cust_paid);
	printf("\t\t\t\t\t\t\t\tCust %u\t\t\t|\n", custID);
}

/**
 * The customer in the bar leaves the bar.
 */
void custLeaveBar(unsigned int custID) {
	sem_wait(pay_received);
	sem_post(rmink_mutex);
	printf("\t\t\t\t\t\t\t\t\t\tCust %u\t|\n", custID);
}