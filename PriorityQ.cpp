// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator
// 4-18-13
// PriorityQ.cpp

#include <iostream>
#include "PriorityQ.h"
using namespace std;

PriorityQ::PriorityQ() {
	head = 0;
}

PriorityQ::~PriorityQ() {
	Node3* temp = head;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
}

void PriorityQ::add(int a, int b, int c, int d) {
	Node3* insert = new Node3;
	insert->id = a;
	insert->arrival = b;
	insert->burst = c;
	insert->priority = d;
	insert->next = 0;
	
	// Empty list
	if(head == 0) {
		head = insert;
	} else { 
		// Add to front of list
		if(insert->priority < head->priority) {
			insert->next = head;
			head = insert;
		//Add to middle or back of list
		} else  { 
			Node3* front = 0;
			Node3* back = head;
			while(back->next && back->next->priority <= insert->priority) {
				front = back;
				back = back->next;
			}
			if(back->priority > insert->priority) {
				if(front == 0) {
					insert->next = back;
					head = insert;
				}
			} else if(back->priority < insert->priority) {
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

void PriorityQ::pop() {
	if(head->next == 0) {
		delete head;
		head = 0;
	} else {
		Node3* temp = head->next;
		delete head;
		head = temp;
	}
}

bool PriorityQ::isEmpty() const {
	return head == 0;
}

int PriorityQ::peekArrival() const {
	return head->arrival;
}

int PriorityQ::peekID() const {
	return head->id;
}

int PriorityQ::peekBurst() const {
	return head->burst;
}
