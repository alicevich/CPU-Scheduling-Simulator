// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

//Priority: arrival time, lower process id as tie breaker

#ifndef ARRIVALQUEUE_H
#define ARRIVALQUEUE_H

struct Node1 {
	int id;
	int arrival;
	int burst;
	int priority;
	Node1* next;
};

class ArrivalQueue {
	private:
		Node1* head;
		int size;
		ArrivalQueue(const ArrivalQueue&); 
		ArrivalQueue& operator=(const ArrivalQueue&); 
	public:
		ArrivalQueue();
		~ArrivalQueue();
		void add(int, int, int, int);
		void pop();
		bool isEmpty() const;
		int peekArrival() const;
		int peekID() const;
		int peekBurst() const;
		int peekPriority() const;
		int getSize() const;
};

#endif // ARRIVALQUEUE_H
