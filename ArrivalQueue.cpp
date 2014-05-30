// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

#include <iostream>
#include "ArrivalQueue.h"
using namespace std;

ArrivalQueue::ArrivalQueue() {
	head = 0;
	size = 0;
}

ArrivalQueue::~ArrivalQueue() {
	Node1* temp = head;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
}

void ArrivalQueue::add(int id, int arrival, int burst, int priority) {
	size++;
	Node1* insert = new Node1;
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
			Node1* front = 0;
			Node1* back = head;
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

void ArrivalQueue::pop() {
	size--;
	if(head->next == 0) {
		delete head;
		head = 0;
	} else {
		Node1* temp = head->next;
		delete head;
		head = temp;
	}
}

bool ArrivalQueue::isEmpty() const {
	return head == 0;
}

int ArrivalQueue::peekArrival() const {
	return head->arrival;
}

int ArrivalQueue::peekID() const {
	return head->id;
}

int ArrivalQueue::peekBurst() const {
	return head->burst;
}

int ArrivalQueue::peekPriority() const {
	return head->priority;
}

int ArrivalQueue::getSize() const {
	return size;
}
