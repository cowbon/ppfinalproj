#include <iostream>
#include <pthread.h>
#include <climits>
//#include <semaphore.h>
#include "node.h"
//#include "functions.h"
#include "parallel_alpha_beta.h"
//#define NUM_OF_THREAD 4

using namespace std;
pthread_t thr[10];
int index[10];
//int trd_rtn_value[30];
//node* node_index[20];
//bool done[20];
node* current;
int cur_bound;
//sem_t cnt, barrier;
pthread_barrier_t barr;
pthread_mutex_t update_bound;
int num_of_thread, b_factor_global;
int counter;
//int counter0, counter1;

void* parallel_region(void* rank){
	node* ind_thread;
	int r = *((int*)rank);
	bool parent_minmax;
	int aa = 0;
	while(current != NULL){ // loop terminate when the root is reached
		
		//ind_thread = node_index[r];
		//counter1 = 0;
		parent_minmax = current->min_or_max;
		ind_thread = (current->children)+(r+1);
		if(ind_thread != NULL){
			int a;
			if(parent_minmax){ // max node
				/*trd_rtn_value[r]*/ a = MinMax(ind_thread, INT_MIN+1, (-1)*cur_bound) * (-1);
				pthread_mutex_lock(&update_bound);
				if(a > cur_bound) cur_bound = a;
				pthread_mutex_unlock(&update_bound);
			}
			else{
				/*trd_rtn_value[r]*/ a = MinMax(ind_thread, INT_MIN+1, cur_bound);
				pthread_mutex_lock(&update_bound);
				if(a < cur_bound) cur_bound = a;
				pthread_mutex_unlock(&update_bound);
			}
			//done[r] = true;
		}
		//cout << "Entered here? " << r << " " << aa << endl;
		for(int i = num_of_thread+r+1; i < b_factor_global; i+=num_of_thread){
			int a;
			//if(!done[i]){
				//done[i] = true;
				ind_thread = (current->children)+i;
				if(parent_minmax){
					/*trd_rtn_value[i]*/ a = MinMax(ind_thread, INT_MIN+1, (-1)*cur_bound) * (-1);
					pthread_mutex_lock(&update_bound);
					if(a > cur_bound) cur_bound = a;
					pthread_mutex_unlock(&update_bound);
				}
				else{
					/*trd_rtn_value[i]*/ a = MinMax(ind_thread, INT_MIN+1, cur_bound);
					pthread_mutex_lock(&update_bound);
					if(a < cur_bound) cur_bound = a;
					pthread_mutex_unlock(&update_bound);
				}
			//}
		}
		
		//////////////////////////////////////barrier
		
		pthread_barrier_wait(&barr);

		//////////////////////////////////////barrier
		if(r == 0){
			//int temp;
			/*if(!parent_minmax){ // true if parent is min node
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
			}*/
			current = current->parent;
			/*node* n = current;
			if(current->parent != NULL){
				for(int i = 0; i < b_factor_global-1; i++){
					node_index[i] = (n = n->sibling);
					done[i] = false;
				}
			}*/
		}
		//////////////////////////////////////barrier
		//cout << "Entered here! " << r << " " << aa << endl;
		aa+=1;
		pthread_barrier_wait(&barr);

		/////////////////////////////////////barrier
	}
	return NULL;
}

int paraAlphaBeta(node* root, int branch_factor){
	current = root;
	
	while(current->children != NULL)
		current = current->children;
	//current = current->parent;
	cur_bound = current->value;
	current = current->parent;
	b_factor_global = branch_factor;
	num_of_thread = branch_factor <= 5 ? branch_factor-1 : 4;
	counter = 0;
	pthread_barrier_init(&barr, NULL, num_of_thread);
	pthread_mutex_init(&update_bound, NULL);
	//counter0 = 0;
	//counter1 = 0;
	//node* n = current;
	/*for(int i = 0; i < branch_factor-1; i++){
		node_index[i] = (n = n->sibling);
		done[i] = false;
	}*/
	for(int i = 0; i < num_of_thread; i++){
		index[i] = i;
		pthread_create(&thr[i], NULL, &parallel_region, (void*)&index[i]);
	}
	for(int i = 0; i < num_of_thread; i++)
		pthread_join(thr[i], NULL);
	pthread_barrier_destroy(&barr);
	pthread_mutex_destroy(&update_bound);
	return cur_bound;
}
