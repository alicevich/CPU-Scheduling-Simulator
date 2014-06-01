// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

// Priority: priority value, lower process id as tie breaker

#include "Node.h"
#ifndef PRIORITYQ_H
#define PRIORITYQ_H

class PriorityQ {
	private:
		Node* head;
		PriorityQ(const PriorityQ&); 
		PriorityQ& operator=(const PriorityQ&);
	public:
		PriorityQ();
		~PriorityQ();
		void add(int, int, int, int);
		void pop();
		bool isEmpty() const;
		int peekArrival() const;
		int peekID() const;
		int peekBurst() const;
};

#endif //PRIORITYQ_H
