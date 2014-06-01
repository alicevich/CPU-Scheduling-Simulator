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

bool isValidScheduler(string scheduleType)
{
	return scheduleType.compare("sjf") == 0 
		|| scheduleType.compare("srtf") == 0 
		|| scheduleType.compare("np") == 0 
		|| scheduleType.compare("pp") == 0;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cerr << "Error: Please enter \"./run <file name> <SJF, SRTF, NP, PP>\"" << endl;
		return 0;
	} else {
		string filename = argv[1];
		string schedType = string(argv[2]);
		transform(schedType.begin(), schedType.end(), schedType.begin(), ::tolower);
		
		if(!isValidScheduler(schedType)) {
			cerr << "Error: Invalid scheduling algorithm. Please enter \"./run <file name> <SJF, SRTF, NP, PP>\"" << endl;
			return 0;
		}
		
		ifstream file;
		file.open(filename.c_str());
		if(!file.is_open()) {
			cerr << "Error: File does not exist" << endl;
			return 0;
		} else {
			// Read in all processes from file and store into ArrivalQueue.
			// ArrivalQueue sorted by order of arrival. 
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
			
			// At this point, ArrivalQueue holds process in order of arrival. 
			// Now test for Scheduler type and display CPU simulation run. 
			bool idle = true;
			bool outputIdle = false;
			int i = 0;
			int idleTime = 0;
			int totalTime = 0;
			queue<int> terminationQ;
			queue<int> turnAroundQ, waitingQ, burstQ, arrivalQ;
			
			// Display Shortest Job First simulation
			if(schedType == "sjf") {
				BurstPriorityQ burstPriorityQ; 
				
				while(!arrivalQueue.isEmpty() || !burstPriorityQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// burstPriorityQ(sorted in order of burst time)
					while(!arrivalQueue.isEmpty() && i >= arrivalQueue.peekArrival()) {
						int id, arrival, burst, priority;
						id = arrivalQueue.peekID();
						arrival = arrivalQueue.peekArrival();
						burst = arrivalQueue.peekBurst();
						priority = arrivalQueue.peekPriority();
						burstPriorityQ.add(id, arrival, burst, priority);
						arrivalQueue.pop();
					}
					
					// Process ready for execution
					if(!burstPriorityQ.isEmpty() && i >= burstPriorityQ.peekArrival()) {
						idle = false;
						outputIdle = false;
						int id = burstPriorityQ.peekID();
						cout << "Time " << i << " Process " << id << endl;
						i += burstPriorityQ.peekBurst();
						terminationQ.push(i);
						arrivalQ.push(burstPriorityQ.peekArrival());
						burstQ.push(burstPriorityQ.peekBurst());
						burstPriorityQ.pop();
					// CPU is idle
					} else {
						idle = true;
						if(!outputIdle) {
							outputIdle = true;
							cout << "Time " << i << " Idle" << endl;
						}
						idleTime++;
						i++;
					}
				}
				
			// Display Shortest Remaining Time First simulation
			} else if(schedType == "srtf") {
				BurstPriorityQ burstPriorityQ; 
				std::map<int, bool> map;
				std::map<int, int> burstMap;
				int remainingTime, currentID, currentArrival, totalTimeCount = 0;
				
				while(!arrivalQueue.isEmpty() || !burstPriorityQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// burstPriorityQ(sorted in order of burst time).
					while(!arrivalQueue.isEmpty() && i >= arrivalQueue.peekArrival()) {
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
									terminationQ.push(i+1);
									arrivalQ.push(currentArrival);
									burstQ.push(burstMap[currentID]);
								}
								
								currentID = id;
								currentArrival = arrival;
								remainingTime = burst;
								cout << "Time " << i << " Process " << currentID << endl;
								burstPriorityQ.pop();
							} else { 
								if(id < currentID) {
									if(remainingTime >= 1) {
										burstPriorityQ.add(currentID, currentArrival, remainingTime, 0);
									} else {
										map[currentID] = true;
									}
									
									if(map[currentID]) {
										terminationQ.push(i+1);
										arrivalQ.push(currentArrival);
										burstQ.push(burstMap[currentID]);
									}
									
									currentID = id;
									currentArrival = arrival;
									remainingTime = burst;
									cout << "Time " << i << " Process " << currentID << endl;
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
							terminationQ.push(i+1);
							arrivalQ.push(currentArrival);
							burstQ.push(burstMap[currentID]);
							if(burstPriorityQ.isEmpty()) {
								idle = true;
								outputIdle = true;
								cout << "Time " << i+1 << " Idle" << endl;
							}else {
								idle = false;
								outputIdle = false;
								currentID = burstPriorityQ.peekID();
								currentArrival = burstPriorityQ.peekArrival();
								remainingTime = burstPriorityQ.peekBurst();
								cout << "Time " << i+1 << " Process " << currentID << endl;
								burstPriorityQ.pop();
							}
						} else {
							remainingTime--;
						}
					// CPU is currently idle and a new process in burstPriorityQ is ready for execution
					} else if(!burstPriorityQ.isEmpty() && i >= burstPriorityQ.peekArrival()) {
						currentID = burstPriorityQ.peekID();
						currentArrival = burstPriorityQ.peekArrival();
						remainingTime = burstPriorityQ.peekBurst();
						idle = false;
						outputIdle = false;
						cout << "Time " << i << " Process " << currentID << endl;
						burstPriorityQ.pop();
						remainingTime--;
					// CPU is currently idle and will stay idle
					} else {
						idleTime++;
						if(!outputIdle) {
							cout << "Time " << i << " Idle" << endl;
							outputIdle = true;
						}
					}
					i++;
				}
				
				terminationQ.push(totalTimeCount + idleTime);
				arrivalQ.push(currentArrival);
				burstQ.push(burstMap[currentID]);
				
			// Display Nonpreemptive Priority simulation 
			} else if(schedType == "np") {
				PriorityQ priorityQ; 
				
				while(!arrivalQueue.isEmpty() || !priorityQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// priorityQ(sorted in order of burst time)
					while(!arrivalQueue.isEmpty() && i >= arrivalQueue.peekArrival()) {
						int id, arrival, burst, priority;
						id = arrivalQueue.peekID();
						arrival = arrivalQueue.peekArrival();
						burst = arrivalQueue.peekBurst();
						priority = arrivalQueue.peekPriority();
						priorityQ.add(id, arrival, burst, priority);
						arrivalQueue.pop();
					}
					
					// Process ready for execution
					if(!priorityQ.isEmpty() && i >= priorityQ.peekArrival()) {
						idle = false;
						outputIdle = false;
						int id = priorityQ.peekID();
						cout << "Time " << i << " Process " << id << endl;
						i += priorityQ.peekBurst();
						terminationQ.push(i);
						arrivalQ.push(priorityQ.peekArrival());
						burstQ.push(priorityQ.peekBurst());
						priorityQ.pop();
					// CPU is idle
					} else {
						idle = true;
						if(!outputIdle) {
							outputIdle = true;
							cout << "Time " << i << " Idle" << endl;
						}
						idleTime++;
						i++;
					}
				}
				
			// Display Preemptive Priority simulation
			} else { 
				PriorityQ priorityQ; 
				std::map<int, bool> map;
				std::map<int, int> priorityMap, burstMap;
				int remainingTime, currentID, currentArrival, totalTimeCount = 0;
				
				while(!arrivalQueue.isEmpty() || !priorityQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// priorityQ(sorted in order of priority value). 
					while(!arrivalQueue.isEmpty() && i >= arrivalQueue.peekArrival()) {
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
									terminationQ.push(i+1);
									arrivalQ.push(currentArrival);
									burstQ.push(burstMap[currentID]);
								}
								
								currentID = id;
								currentArrival = arrival;
								remainingTime = burst;
								cout << "Time " << i << " Process " << currentID << endl;
								priorityQ.pop();
							} else { 
								if(id < currentID) {
									if(remainingTime >= 1) {
										priorityQ.add(currentID, currentArrival, remainingTime, priorityMap[currentID]);
									} else {
										map[currentID] = true;
									}
									
									if(map[currentID]) {
										terminationQ.push(i+1);
										arrivalQ.push(currentArrival);
										burstQ.push(burstMap[currentID]);
									}
									
									currentID = id;
									currentArrival = arrival;
									remainingTime = burst;
									cout << "Time " << i << " Process " << currentID << endl;
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
							terminationQ.push(i+1);
							arrivalQ.push(currentArrival);
							burstQ.push(burstMap[currentID]);
							if(priorityQ.isEmpty()) {
								idle = true;
								outputIdle = true;
								cout << "Time " << i+1 << " Idle" << endl;
							}else {
								idle = false;
								outputIdle = false;
								currentID = priorityQ.peekID();
								currentArrival = priorityQ.peekArrival();
								remainingTime = priorityQ.peekBurst();
								cout << "Time " << i+1 << " Process " << currentID << endl;
								priorityQ.pop();
							}
						} else {
							remainingTime--;
						}
					// CPU is currently idle and a new process in priorityQ is ready for execution
					} else if(!priorityQ.isEmpty() && i >= priorityQ.peekArrival()) {
						currentID = priorityQ.peekID();
						currentArrival = priorityQ.peekArrival();
						remainingTime = priorityQ.peekBurst();
						idle = false;
						outputIdle = false;
						cout << "Time " << i << " Process " << currentID << endl;
						priorityQ.pop();
						remainingTime--;
					// CPU is currently idle and will stay idle
					} else {
						idleTime++;
						if(!outputIdle) {
							cout << "Time " << i << " Idle" << endl;
							outputIdle = true;
						}
					}
					i++;
				}
				terminationQ.push(totalTimeCount + idleTime);
				arrivalQ.push(currentArrival);
				burstQ.push(burstMap[currentID]);			
			}
			
			totalTime = terminationQ.back();
			
			// Display CPU utilization
			double x = (double(totalTime - idleTime) / totalTime) * 100;
			double ceilX = ceil(x);
			double mod = 1 - (ceilX - x);
			if(mod < .5) {
				cout << "CPU Utilization: " << floor(x) << "%" << endl;
			} else {
				cout << "CPU Utilization: " << ceilX << "%" << endl;
			}
			
			// Calculate turn around times
			while (!terminationQ.empty()) {
				int x = terminationQ.front() - arrivalQ.front();
				turnAroundQ.push(x);
				terminationQ.pop();
				arrivalQ.pop();
			}
			
			// Calculate waiting times
			while(!turnAroundQ.empty()) {
				int x = turnAroundQ.front() - burstQ.front();
				waitingQ.push(x);
				turnAroundQ.pop();
				burstQ.pop();
			}			
			
			// Display avg waiting time and worst-case waiting time
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
	}
	return 0;
}

