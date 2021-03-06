// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

#include <iostream>
#include "BurstPriorityQ.h"
#include "Queue.h"
using namespace std;

BurstPriorityQ::BurstPriorityQ() {
	head = 0;
}

BurstPriorityQ::~BurstPriorityQ() {
	Node* temp = head;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
}

void BurstPriorityQ::add(int id, int arrival, int burst, int priority) {
	Node* insert = new Node;
	insert->id = id;
	insert->arrival = arrival;
	insert->burst = burst;
	insert->priority = priority;
	insert->next = 0;
	
	// Empty list
	if(head == 0) {
		head = insert;
	} else { 
		// Add to front of list
		if(insert->burst < head->burst) {
			insert->next = head;
			head = insert;
		// Add to front middle or back of list
		} else  { 
			Node* front = 0;
			Node* back = head;
			while(back->next && back->next->burst <= insert->burst) {
				front = back;
				back = back->next;
			}
			if(back->burst > insert->burst) {
				if(front == 0) {
					insert->next = back;
					head = insert;
				}
			} else if(back->burst < insert->burst) {
				insert->next = back->next;
				back->next = insert;
			} else { 
				if(back->id < insert->id) {
					insert->next = back->next;
					back->next = insert;
				} else {
					insert->next = back;
					if(front == 0) {
						head = insert;
					} else {
						front->next = insert;
					}
					
				}
			}
		} 
	}
}


