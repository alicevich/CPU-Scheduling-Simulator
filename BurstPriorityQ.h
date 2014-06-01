// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

//Priority: burst time, lower process id as tie breaker

#include "Node.h"
#ifndef BURSTPRIORITYQ_H
#define BURSTPRIORITYQ_H

class BurstPriorityQ {
	private:
		Node* head;
		BurstPriorityQ(const BurstPriorityQ&); 
		BurstPriorityQ& operator=(const BurstPriorityQ&); 
	public:
		BurstPriorityQ();
		~BurstPriorityQ();
		void add(int, int, int, int);
		void pop();
		bool isEmpty() const;
		int peekArrival() const;
		int peekID() const;
		int peekBurst() const;
};

#endif //BURSTPRIORITYQ_H
