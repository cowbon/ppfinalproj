#ifndef _PARALLEL_ALPHA_BETA
#define _PARALLEL_ALPHA_BETA
#include <iostream>
#include <pthread.h>
#include <climits>
#include <semaphore.h>
#include "node.h"
#include "functions.h"

void* parallel_region(void* rank);
int paraAlphaBeta(node* root, int branch_factor);
#endif
