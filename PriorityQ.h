// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator
// 4-18-13
// PriorityQ.h

// Priority: priority value, lower process id as tie breaker

#ifndef PRIORITYQ_H
#define PRIORITYQ_H

struct Node3{
	int id;
	int arrival;
	int burst;
	int priority;
	Node3* next;
};

class PriorityQ {
	private:
		Node3* head;
		PriorityQ(const PriorityQ&); //suppress copy constructor
		PriorityQ& operator=(const PriorityQ&); //suppress overloading assignment operator
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
