// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

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
