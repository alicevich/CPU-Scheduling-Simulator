// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

#include <iostream>
#include "BurstPriorityQ.h"
using namespace std;

BurstPriorityQ::BurstPriorityQ() {
	head = 0;
}

BurstPriorityQ::~BurstPriorityQ() {
	Node2* temp = head;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
}

void BurstPriorityQ::add(int id, int arrival, int burst, int priority) {
	Node2* insert = new Node2;
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
			Node2* front = 0;
			Node2* back = head;
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

void BurstPriorityQ::pop() {
	if(head->next == 0) {
		delete head;
		head = 0;
	} else {
		Node2* temp = head->next;
		delete head;
		head = temp;
	}
}

bool BurstPriorityQ::isEmpty() const {
	return head == 0;
}

int BurstPriorityQ::peekArrival() const {
	return head->arrival;
}

int BurstPriorityQ::peekID() const {
	return head->id;
}

int BurstPriorityQ::peekBurst() const {
	return head->burst;
}
