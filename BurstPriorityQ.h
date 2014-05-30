// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

//Priority: burst time, lower process id as tie breaker

#ifndef BURSTPRIORITYQ_H
#define BURSTPRIORITYQ_H

struct Node2 {
	int id;
	int arrival;
	int burst;
	int priority;
	Node2* next;
};

class BurstPriorityQ {
	private:
		Node2* head;
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
