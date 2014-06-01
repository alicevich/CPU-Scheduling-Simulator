// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

//Priority: burst time, lower process id as tie breaker

#ifndef BURSTPRIORITYQ_H
#define BURSTPRIORITYQ_H
#include "Queue.h"

class BurstPriorityQ : public Queue {
	private:
		BurstPriorityQ(const BurstPriorityQ&); 
		BurstPriorityQ& operator=(const BurstPriorityQ&); 
	public:
		BurstPriorityQ();
		~BurstPriorityQ();
		void add(int, int, int, int);
};

#endif //BURSTPRIORITYQ_H
