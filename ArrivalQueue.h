// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

//Priority: arrival time, lower process id as tie breaker


#ifndef ARRIVALQUEUE_H
#define ARRIVALQUEUE_H
#include "Queue.h"

class ArrivalQueue : public Queue {
	private:
		ArrivalQueue(const ArrivalQueue&); 
		ArrivalQueue& operator=(const ArrivalQueue&); 
	public:
		ArrivalQueue();
		~ArrivalQueue();
		void add(int, int, int, int);
};

#endif // ARRIVALQUEUE_H
