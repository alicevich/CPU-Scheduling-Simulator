// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

#include <iostream>
#include "ArrivalQueue.h"
#include "Queue.h"
using namespace std;

ArrivalQueue::ArrivalQueue() {
	head = 0;
}

ArrivalQueue::~ArrivalQueue() {
	Node* temp = head;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
}

void ArrivalQueue::add(int id, int arrival, int burst, int priority) {
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
		if(insert->arrival < head->arrival) {
			insert->next = head;
			head = insert;
		// Add to middle or back of list
		} else  { 
			Node* front = 0;
			Node* back = head;
			while(back->next && back->next->arrival <= insert->arrival) {
				front = back;
				back = back->next;
			}
			if(back->arrival > insert->arrival) {
				if(front == 0) {
					insert->next = back;
					head = insert;
				}
			} else if(back->arrival < insert->arrival) {
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

