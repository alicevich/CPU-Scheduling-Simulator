// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulation

// This program displays a simulation run of one of the CPU Scheduler algorithms
// based on user's input for command line arguments: 
// SJF  - Shortest Job First 
// SRTF - Shortest Remaining Time First
// NP   - Non-Preemptive Priority
// PP   - Preemptive Priority

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <queue>
#include "ArrivalQueue.h"
#include "BurstPriorityQ.h"
#include "PriorityQ.h"
using namespace std;

bool isValidScheduleType(string);
ArrivalQueue initializeArrivalQueue(ifstream&);
void runShortestJobFirst(ArrivalQueue&);
void runShortestRemainingTimeFirst(ArrivalQueue&);
void runNonPreemptivePriority(ArrivalQueue&);
void runPreemptivePriority(ArrivalQueue&);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cerr << "Error: Please enter \"./run <file name> <SJF, SRTF, NP, PP>\"" << endl;
		return 0;
	} else {
		string filename = argv[1];
		string scheduleType = string(argv[2]);
		transform(scheduleType.begin(), scheduleType.end(), scheduleType.begin(), ::tolower);
		
		if(!isValidScheduleType(scheduleType)) {
			cerr << "Error: Invalid scheduling algorithm. Please enter \"./run <file name> <SJF, SRTF, NP, PP>\"" << endl;
			return 0;
		}
		
		ifstream file;
		file.open(filename.c_str());
		if(!file.is_open()) {
			cerr << "Error: File does not exist" << endl;
			return 0;
		} 

		ArrivalQueue arrivalQueue = initializeArrivalQueue(file);
		
		if(scheduleType == "sjf") {
			runShortestJobFirst(arrivalQueue);
		} else if(scheduleType == "srtf") {
			runShortestRemainingTimeFirst(arrivalQueue);
		} else if(scheduleType == "np") {
			runNonPreemptivePriority(arrivalQueue);
		} else { 
			runPreemptivePriority(arrivalQueue);	
		}
	}
	return 0;
}

bool isValidScheduleType(string scheduleType)
{
	return scheduleType.compare("sjf") == 0 
		|| scheduleType.compare("srtf") == 0 
		|| scheduleType.compare("np") == 0 
		|| scheduleType.compare("pp") == 0;
}

void displayResults(int idleTime, queue<int> terminationQ, queue<int> turnAroundQ, 
	queue<int> waitingQ, queue<int> burstQ, queue<int> arrivalQ)
{
	// Compute and display CPU utilization
	int totalTime = terminationQ.back();
	double percentage = (double(totalTime - idleTime) / totalTime) * 100;
	double roundUpPercentage = ceil(percentage);
	double delta = 1 - (roundUpPercentage - percentage);
	percentage = (delta < .5) ? floor(percentage) : roundUpPercentage;
	
	cout << "CPU Utilization: " << percentage << "%" << endl;

	// get turn around times
	while (!terminationQ.empty()) {
		int delta = terminationQ.front() - arrivalQ.front();
		turnAroundQ.push(delta);
		terminationQ.pop();
		arrivalQ.pop();
	}
	
	// get waiting times
	while(!turnAroundQ.empty()) {
		int delta = turnAroundQ.front() - burstQ.front();
		waitingQ.push(delta);
		turnAroundQ.pop();
		burstQ.pop();
	}			
	
	// get avg waiting time and worst-case waiting time
	int count = 0;
	int worstCaseTime = 0;
	int sum = 0;
	while(!waitingQ.empty()) {
		worstCaseTime = max(worstCaseTime, waitingQ.front());
		count++;
		sum += waitingQ.front();
		waitingQ.pop();
	}
	
	double avgWaitingTime = double(sum) / count;
	printf("Average waiting time: %.2f\n", avgWaitingTime);
	cout << "Worst-case waiting time: " << worstCaseTime << endl;
}

ArrivalQueue initializeArrivalQueue(ifstream& file)
{
	string token;
	ArrivalQueue arrivalQueue;
	while(!file.eof()) {
		file >> token;	
		// Check again for eof, so won't read last input token twice
		if(!file.eof()) {
			string id_str, arrival_str, burst_str, priority_str;
		
			id_str = token;
			file >> token;
			arrival_str = token;
			file >> token;
			burst_str = token;
			file >> token;
			priority_str = token;
			
			istringstream id_ss(id_str);
			istringstream arrival_ss(arrival_str);
			istringstream burst_ss(burst_str);
			istringstream priority_ss(priority_str);
			
			int id, arrival, burst, priority;
			
			id_ss >> id;
			arrival_ss >> arrival;
			burst_ss >> burst;
			priority_ss >> priority;
			
			arrivalQueue.add(id, arrival, burst, priority);
		}
	}
	return arrivalQueue;
}

void runShortestJobFirst(ArrivalQueue& arrivalQueue)
{		
	bool idle = true;
	bool outputIdle = false;
	int time = 0, idleTime = 0, totalTime = 0;
	queue<int> terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ;
	BurstPriorityQ burstPriorityQ; 
			
	while(!arrivalQueue.isEmpty() || !burstPriorityQ.isEmpty()) {
		// If a new process arrives at time time, add to 
		// burstPriorityQ(sorted in order of burst time)
		while(!arrivalQueue.isEmpty() && time >= arrivalQueue.peekArrival()) {
			int id, arrival, burst, priority;
			id = arrivalQueue.peekID();
			arrival = arrivalQueue.peekArrival();
			burst = arrivalQueue.peekBurst();
			priority = arrivalQueue.peekPriority();
			burstPriorityQ.add(id, arrival, burst, priority);
			arrivalQueue.pop();
		}
		
		// Process ready for execution
		if(!burstPriorityQ.isEmpty() && time >= burstPriorityQ.peekArrival()) {
			idle = false;
			outputIdle = false;
			int id = burstPriorityQ.peekID();
			cout << "Time " << time << " Process " << id << endl;
			time += burstPriorityQ.peekBurst();
			terminationQ.push(time);
			arrivalQ.push(burstPriorityQ.peekArrival());
			burstQ.push(burstPriorityQ.peekBurst());
			burstPriorityQ.pop();
		// CPU is idle
		} else {
			idle = true;
			if(!outputIdle) {
				outputIdle = true;
				cout << "Time " << time << " Idle" << endl;
			}
			idleTime++;
			time++;
		}
	}
	displayResults(idleTime, terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ);
}

void runShortestRemainingTimeFirst(ArrivalQueue& arrivalQueue)
{
	bool idle = true;
	bool outputIdle = false;
	int time = 0, idleTime = 0, totalTime = 0;
	queue<int> terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ;
	BurstPriorityQ burstPriorityQ; 
	std::map<int, bool> map;
	std::map<int, int> burstMap;
	int remainingTime, currentID, currentArrival, totalTimeCount = 0;
	
	while(!arrivalQueue.isEmpty() || !burstPriorityQ.isEmpty()) {
		// If a new process arrives at time time, add to 
		// burstPriorityQ(sorted in order of burst time).
		while(!arrivalQueue.isEmpty() && time >= arrivalQueue.peekArrival()) {
			int id, arrival, burst, priority;
			id = arrivalQueue.peekID();
			arrival = arrivalQueue.peekArrival();
			burst = arrivalQueue.peekBurst();
			priority = arrivalQueue.peekPriority();
			map[id] = false;
			burstMap[id] = burst;
			totalTimeCount += burst; 
			burstPriorityQ.add(id, arrival, burst, priority);
			
			// Compare new arrival's burst time to current running 
			// process's remaining burst time. Run the process with
			// lower burst time.
			if(!idle) {
				if(burst > remainingTime) {
				} else if( burst < remainingTime) {
					if(remainingTime >= 1) {
						burstPriorityQ.add(currentID, currentArrival, remainingTime, 0);
					} else {
						map[currentID] = true;
					}
					
					if(map[currentID]) {
						terminationQ.push(time+1);
						arrivalQ.push(currentArrival);
						burstQ.push(burstMap[currentID]);
					}
					
					currentID = id;
					currentArrival = arrival;
					remainingTime = burst;
					cout << "Time " << time << " Process " << currentID << endl;
					burstPriorityQ.pop();
				} else { 
					if(id < currentID) {
						if(remainingTime >= 1) {
							burstPriorityQ.add(currentID, currentArrival, remainingTime, 0);
						} else {
							map[currentID] = true;
						}
						
						if(map[currentID]) {
							terminationQ.push(time+1);
							arrivalQ.push(currentArrival);
							burstQ.push(burstMap[currentID]);
						}
						
						currentID = id;
						currentArrival = arrival;
						remainingTime = burst;
						cout << "Time " << time << " Process " << currentID << endl;
						burstPriorityQ.pop();
					} 
				}
			} 
			arrivalQueue.pop();
		}
		
		// CPU currently running a process
		if(!idle) {
			if(remainingTime == 1) {
				map[currentID] = true;
				terminationQ.push(time+1);
				arrivalQ.push(currentArrival);
				burstQ.push(burstMap[currentID]);
				if(burstPriorityQ.isEmpty()) {
					idle = true;
					outputIdle = true;
					cout << "Time " << time+1 << " Idle" << endl;
				}else {
					idle = false;
					outputIdle = false;
					currentID = burstPriorityQ.peekID();
					currentArrival = burstPriorityQ.peekArrival();
					remainingTime = burstPriorityQ.peekBurst();
					cout << "Time " << time+1 << " Process " << currentID << endl;
					burstPriorityQ.pop();
				}
			} else {
				remainingTime--;
			}
		// CPU is currently idle and a new process in burstPriorityQ is ready for execution
		} else if(!burstPriorityQ.isEmpty() && time >= burstPriorityQ.peekArrival()) {
			currentID = burstPriorityQ.peekID();
			currentArrival = burstPriorityQ.peekArrival();
			remainingTime = burstPriorityQ.peekBurst();
			idle = false;
			outputIdle = false;
			cout << "Time " << time << " Process " << currentID << endl;
			burstPriorityQ.pop();
			remainingTime--;
		// CPU is currently idle and will stay idle
		} else {
			idleTime++;
			if(!outputIdle) {
				cout << "Time " << time << " Idle" << endl;
				outputIdle = true;
			}
		}
		time++;
	}
	
	terminationQ.push(totalTimeCount + idleTime);
	arrivalQ.push(currentArrival);
	burstQ.push(burstMap[currentID]);
	
	displayResults(idleTime, terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ);
}

void runNonPreemptivePriority(ArrivalQueue& arrivalQueue)
{
	bool idle = true;
	bool outputIdle = false;
	int time = 0, idleTime = 0, totalTime = 0;
	queue<int> terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ;
	PriorityQ priorityQ; 
	
	while(!arrivalQueue.isEmpty() || !priorityQ.isEmpty()) {
		// If a new process arrives at time time, add to 
		// priorityQ(sorted in order of burst time)
		while(!arrivalQueue.isEmpty() && time >= arrivalQueue.peekArrival()) {
			int id, arrival, burst, priority;
			id = arrivalQueue.peekID();
			arrival = arrivalQueue.peekArrival();
			burst = arrivalQueue.peekBurst();
			priority = arrivalQueue.peekPriority();
			priorityQ.add(id, arrival, burst, priority);
			arrivalQueue.pop();
		}
		
		// Process ready for execution
		if(!priorityQ.isEmpty() && time >= priorityQ.peekArrival()) {
			idle = false;
			outputIdle = false;
			int id = priorityQ.peekID();
			cout << "Time " << time << " Process " << id << endl;
			time += priorityQ.peekBurst();
			terminationQ.push(time);
			arrivalQ.push(priorityQ.peekArrival());
			burstQ.push(priorityQ.peekBurst());
			priorityQ.pop();
		// CPU is idle
		} else {
			idle = true;
			if(!outputIdle) {
				outputIdle = true;
				cout << "Time " << time << " Idle" << endl;
			}
			idleTime++;
			time++;
		}
	}
	displayResults(idleTime, terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ);
}

void runPreemptivePriority(ArrivalQueue& arrivalQueue)
{
	bool idle = true;
	bool outputIdle = false;
	int time = 0, idleTime = 0, totalTime = 0;
	queue<int> terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ;
	PriorityQ priorityQ; 
	std::map<int, bool> map;
	std::map<int, int> priorityMap, burstMap;
	int remainingTime, currentID, currentArrival, totalTimeCount = 0;
	
	while(!arrivalQueue.isEmpty() || !priorityQ.isEmpty()) {
		// If a new process arrives at time time, add to 
		// priorityQ(sorted in order of priority value). 
		while(!arrivalQueue.isEmpty() && time >= arrivalQueue.peekArrival()) {
			int id, arrival, burst, priority;
			id = arrivalQueue.peekID();
			arrival = arrivalQueue.peekArrival();
			burst = arrivalQueue.peekBurst();
			priority = arrivalQueue.peekPriority();
			map[id] = false;
			burstMap[id] = burst;
			priorityMap[id] = priority;
			totalTimeCount += burst; 
			priorityQ.add(id, arrival, burst, priority);
			
			// Compare new arrival's priority value to current running 
			// process's priority value. Run the process with
			// lower priority value.
			if(!idle) {
				if(priority > priorityMap[currentID]) {
				} else if( priority < priorityMap[currentID]) {
					if(remainingTime >= 1) {
						priorityQ.add(currentID, currentArrival, remainingTime, priorityMap[currentID]);
					} else {
						map[currentID] = true;
					}
					
					if(map[currentID]) {
						terminationQ.push(time+1);
						arrivalQ.push(currentArrival);
						burstQ.push(burstMap[currentID]);
					}
					
					currentID = id;
					currentArrival = arrival;
					remainingTime = burst;
					cout << "Time " << time << " Process " << currentID << endl;
					priorityQ.pop();
				} else { 
					if(id < currentID) {
						if(remainingTime >= 1) {
							priorityQ.add(currentID, currentArrival, remainingTime, priorityMap[currentID]);
						} else {
							map[currentID] = true;
						}
						
						if(map[currentID]) {
							terminationQ.push(time+1);
							arrivalQ.push(currentArrival);
							burstQ.push(burstMap[currentID]);
						}
						
						currentID = id;
						currentArrival = arrival;
						remainingTime = burst;
						cout << "Time " << time << " Process " << currentID << endl;
						priorityQ.pop();
					} 
				}
			} 
			arrivalQueue.pop();
		}
		
		// CPU currently running a process
		if(!idle) {
			if(remainingTime == 1) {
				map[currentID] = true;
				terminationQ.push(time+1);
				arrivalQ.push(currentArrival);
				burstQ.push(burstMap[currentID]);
				if(priorityQ.isEmpty()) {
					idle = true;
					outputIdle = true;
					cout << "Time " << time+1 << " Idle" << endl;
				}else {
					idle = false;
					outputIdle = false;
					currentID = priorityQ.peekID();
					currentArrival = priorityQ.peekArrival();
					remainingTime = priorityQ.peekBurst();
					cout << "Time " << time+1 << " Process " << currentID << endl;
					priorityQ.pop();
				}
			} else {
				remainingTime--;
			}
		// CPU is currently idle and a new process in priorityQ is ready for execution
		} else if(!priorityQ.isEmpty() && time >= priorityQ.peekArrival()) {
			currentID = priorityQ.peekID();
			currentArrival = priorityQ.peekArrival();
			remainingTime = priorityQ.peekBurst();
			idle = false;
			outputIdle = false;
			cout << "Time " << time << " Process " << currentID << endl;
			priorityQ.pop();
			remainingTime--;
		// CPU is currently idle and will stay idle
		} else {
			idleTime++;
			if(!outputIdle) {
				cout << "Time " << time << " Idle" << endl;
				outputIdle = true;
			}
		}
		time++;
	}
	terminationQ.push(totalTimeCount + idleTime);
	arrivalQ.push(currentArrival);
	burstQ.push(burstMap[currentID]);
	
	displayResults(idleTime, terminationQ, turnAroundQ, waitingQ, burstQ, arrivalQ);
}


