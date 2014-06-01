// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

// Priority: priority value, lower process id as tie breaker

#ifndef PRIORITYQ_H
#define PRIORITYQ_H
#include "Queue.h"

class PriorityQ : public Queue {
	private:
		PriorityQ(const PriorityQ&); 
		PriorityQ& operator=(const PriorityQ&);
	public:
		PriorityQ();
		~PriorityQ();
		void add(int, int, int, int);
};

#endif //PRIORITYQ_H
