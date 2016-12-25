#include <iostream>
#include <pthread.h>
#include <climits>
#include <semaphore.h>
#include "node.h"
#include "functions.h"
#define NUM_OF_TRD 4

using namespace std;

pthread_t thr[NUM_OF_TRD];
int index[NUM_OF_TRD];
int trd_rtn_value[NUM_OF_TRD];
node* current;
int cur_bound;
sem_t cnt, barrier;
int counter;

void* parallel_region(int* rank){
	node* ind_thread = current;
	bool parent_minmax = current->parent->min_or_max;
	while(current->parent != NULL){ // loop terminate when the root is reached
		for(int k = 0; k < *rank; k++){
			ind_thread = ind_thread->sibling;
			if(ind_thread == NULL) break;
		}
		if(parent_minmax) trd_rtn_value[(*rank)] = MinMax(ind_thread, cur_bound, INT_MAX);
		else trd_rtn_value[(*rank)] = MinMax(ind_thread, INT_MIN, cur_bound);
		
		//////////////////////////////////////barrier
		sem_wait(&cnt);
		if(counter == NUM_OF_THREAD - 1){
			counter = 0;
			sem_post(&cnt);
			for(int i = 0; i < NUM_OF_THREAD -1; i++)
				sem_post(&barrier);
		}
		else{
			counter++;
			sem_post(&cnt);
			sem_wait(&barrier);
		}
		//////////////////////////////////////barrier
		
		if(*rank == 0){
			int temp;
			if(!parent_minmax){ // true if parent is min node
				temp = cur_bound;
				for(int i = 0; i < NUM_OF_TRD; i++)
					if(trd_rtn_value[i] < temp) temp = trd_rtn_value[i];
				cur_bound = temp;
			}
			else{ // parent is max node
				temp = cur_bound;
				for(int i = 0; i < NUM_OF_TRD; i++)
					if(trd_rtn_value[i] > temp) temp = trd_rtn_value[i];
				cur_bound = temp;
			}
			current = current->parent;
		}
		
		//////////////////////////////////////barrier
		sem_wait(&cnt);
		if(counter == NUM_OF_THREAD - 1){
			counter = 0;
			sem_post(&cnt);
			for(int i = 0; i < NUM_OF_THREAD -1; i++)
				sem_post(&barrier);
		}
		else{
			counter++;
			sem_post(&cnt);
			sem_wait(&barrier);
		}
		//////////////////////////////////////barrier
		
	}
}

int paraAlphaBeta(const node* root){
	current = root;
	counter = 0;
	sem_init(&cnt, 0, 1);
	sem_init(&barrier, 0, 0);
	while(current->children != NULL)
		current = current->children;
	//current = current->parent;
	cur_bound = current->value;
	for(int i = 0; i < NUM_OF_TRD; i++){
		index[i] = i;
		pthread_create(&thr[i], NULL, &parallel_region, &index[i]);
	}
	for(int i = 0; i < NUM_OF_TRD; i++)
		pthread_join(thr[i], NULL);
	sem_destroy(&cnt);
	sem_destroy(&barrier);
	return cur_bound;
}