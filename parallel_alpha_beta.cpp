#include <iostream>
#include <pthread.h>
#include <climits>
//#include <semaphore.h>
#include "node.h"
#include "functions.h"
#include "parallel_alpha_beta.h"
//#define NUM_OF_THREAD 4

using namespace std;
pthread_t thr[10];
int index[10];
int trd_rtn_value[30];
node* node_index[20];
bool done[20];
node* current;
int cur_bound;
//sem_t cnt, barrier;
pthread_barrier_t barr;
//pthread_mutex_t _mutex;
int num_of_thread, b_factor_global;
int counter;
//int counter0, counter1;

void* parallel_region(void* rank){
	node* ind_thread = current;
	int r = *((int*)rank);
	bool parent_minmax = current->parent->min_or_max;
	while(current->parent != NULL){ // loop terminate when the root is reached
		ind_thread = node_index[r];
		//counter1 = 0;
		parent_minmax = current->parent->min_or_max;
		if(ind_thread != NULL){
			if(parent_minmax) trd_rtn_value[r] = MinMax(ind_thread, INT_MIN+1, (-1)*cur_bound) * (-1);
			else trd_rtn_value[r] = MinMax(ind_thread, INT_MIN+1, cur_bound);
			done[r] = true;
		}
		for(int i = num_of_thread+r; i < b_factor_global-1; i+=num_of_thread){
			if(!done[i]){
				done[i] = true;
				if(parent_minmax) trd_rtn_value[i] = MinMax(ind_thread, INT_MIN+1, (-1)*cur_bound) * (-1);
				else trd_rtn_value[i] = MinMax(ind_thread, INT_MIN+1, cur_bound);
			}
		}
		
		//////////////////////////////////////barrier

		/*sem_wait(&cnt);
		if(counter == num_of_thread - 1){
			counter = 0;
			sem_post(&cnt);
			for(int i = 0; i < num_of_thread -1; i++)
				sem_post(&barrier);
		}
		else{
			counter++;
			sem_post(&cnt);
			sem_wait(&barrier);
		}*/
		pthread_barrier_wait(&barr);

		//////////////////////////////////////barrier
		//counter0 = 0;
		if(r == 0){
			int temp;
			if(!parent_minmax){ // true if parent is min node
				temp = cur_bound;
				for(int i = 0; i < num_of_thread; i++)
					if(trd_rtn_value[i] < temp) temp = trd_rtn_value[i];
				cur_bound = temp;
			}
			else{ // parent is max node
				temp = cur_bound;
				for(int i = 0; i < num_of_thread; i++)
					if(trd_rtn_value[i] > temp) temp = trd_rtn_value[i];
				cur_bound = temp;
			}
			current = current->parent;
			node* n = current;
			if(current->parent != NULL){
				for(int i = 0; i < b_factor_global-1; i++){
					node_index[i] = (n = n->sibling);
					done[i] = false;
				}
			}
		}
		//////////////////////////////////////barrier
		
		/*sem_wait(&cnt);
		if(counter == num_of_thread - 1){
			counter = 0;
			sem_post(&cnt);
			for(int i = 0; i < num_of_thread -1; i++)
				sem_post(&barrier);
		}
		else{
			counter++;
			sem_post(&cnt);
			sem_wait(&barrier);
		}*/
		pthread_barrier_wait(&barr);

		/////////////////////////////////////barrier

	}
	return NULL;
}

int paraAlphaBeta(node* root, int branch_factor){
	current = root;
	//sem_init(&cnt, 0, 1);
	//sem_init(&barrier, 0, 0);
	//pthread_mutex_init(&_mutex, NULL);
	
	while(current->children != NULL)
		current = current->children;
	//current = current->parent;
	cur_bound = current->value;
	b_factor_global = branch_factor;
	num_of_thread = 2;//branch_factor <= 9 ? branch_factor-1 : 8;
	counter = 0;
	pthread_barrier_init(&barr, NULL, num_of_thread);
	//counter0 = 0;
	//counter1 = 0;
	node* n = current;
	for(int i = 0; i < branch_factor-1; i++){
		node_index[i] = (n = n->sibling);
		done[i] = false;
	}
	for(int i = 0; i < num_of_thread; i++){
		index[i] = i;
		pthread_create(&thr[i], NULL, &parallel_region, (void*)&index[i]);
	}
	for(int i = 0; i < num_of_thread; i++)
		pthread_join(thr[i], NULL);
	//pthread_mutex_destroy(&_mutex);
	//sem_destroy(&cnt);
	//sem_destroy(&barrier);
	pthread_barrier_destroy(&barr);
	return cur_bound;
}
