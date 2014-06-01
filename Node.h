// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

// Node struct used across multiple classes. 

#ifndef NODE_H
#define NODE_H

struct Node {
	int id;
	int arrival;
	int burst;
	int priority;
	Node* next;
};

#endif //NODE_H
