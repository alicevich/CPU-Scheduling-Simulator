// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulator

// Abstract Queue class, child classes are to implement add(int, int, int, int)

#ifndef NODE_H
#define NODE_H

struct Node {
	int id;
	int arrival;
	int burst;
	int priority;
	Node* next;
};

class Queue {
	protected:
		Node* head;
	public:
		virtual void add(int, int, int, int) = 0;
		bool isEmpty() const { return head == 0; };
		int peekArrival() const { return head->arrival; };
		int peekID() const { return head->id; };
		int peekBurst() const { return head->burst; };
		int peekPriority() const { return head->priority; };
		void pop() {
			if(head->next == 0) {
				delete head;
				head = 0;
			} else {
				Node* temp = head->next;
				delete head;
				head = temp;
			}
		};
};

#endif //NODE_H

