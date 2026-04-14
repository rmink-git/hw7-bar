/*
 * globals.h
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

// number of customer threads
extern int num_threads;
extern int thread_args[];

extern sem_t *rmink_mutex;
extern sem_t *order_placed;
extern sem_t *drink_made;
extern sem_t *cust_paid;
extern sem_t *pay_received;

#endif /* GLOBALS_H_ */
