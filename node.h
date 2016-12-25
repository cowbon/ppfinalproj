#ifndef NODE_H
#define NODE_H

typedef struct node node;

struct node{
	int value;
	node* children;
	//int nchildren;
	node* parent;
	node* sibling;
	bool min_or_max; // true for max node, false for min node
};

#endif
