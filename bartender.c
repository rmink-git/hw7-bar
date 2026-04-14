/*
 * bartender.c
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "bartender.h"

/**
 * Code for bartender thread.
 * Do not touch.
 */
void *bartender(void *args) {
	for (int i = 0; i < num_threads; i++) {
		waitForCustomer();
		makeDrink();
		receivePayment();
	}
	return NULL;
}

/**
 * Waits in a closet until a customer enters.
 */
void waitForCustomer() {
	sem_wait(order_placed);
	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
}

/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink() {
	useconds_t make_drink = ((rand() % (1000 - (5 + 1))) + 5) * 1000;
	usleep(make_drink);
	sem_post(drink_made);
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");
}

/**
 * Gets payment from the correct customer
 */
void receivePayment() {
	// at the register waiting for customer to pay
	sem_wait(cust_paid);
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");

	// got paid by the customer!
	sem_post(pay_received);
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");
}