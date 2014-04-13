// Alice Vichitthavong
// CPSC 341
// HW1: CPU Scheduler Simulation
// 4/18/13

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

int main(int argc, char* argv[]) {
	// Check for valid arguments count
	if(argc < 3) {
		cerr << "Error: Too few arguments" << endl;
		return 0;
	} else {
		std::string filename = argv[1];
		std::string schedType = string(argv[2]);
		std::transform(schedType.begin(), schedType.end(), schedType.begin(), ::tolower);
		
		// Check for valid scheduler algorithm
		if(schedType.compare("sjf") != 0 && schedType.compare("srtf") != 0 && 
				schedType.compare("np") != 0 && schedType.compare("pp") != 0) {
			cerr << "Error: Invalid argument " << argv[2] << endl;
			return 0;
		}
		
		// Check for valid file name
		ifstream infile;
		infile.open(filename.c_str());
		if(!infile.is_open()) {
			cerr << "Error: Invalid file name" << endl;
			return 0;
		} else {
			string line;
			// Read in all processes from file and store into ArrivalQueue.
			// ArrivalQueue sorted by order of arrival. 
			ArrivalQueue q;
			while(!infile.eof()) {
				infile >> line;
				// Check again for eof, so won't read last input line twice
				if(!infile.eof()) {
					string id_str = "";
					string arrival_str = "";
					string burst_str = "";
					string priority_str = "";
					
					id_str = line;
					infile >> line;
					arrival_str = line;
					infile >> line;
					burst_str = line;
					infile >> line;
					priority_str = line;
					
					std::istringstream id_ss(id_str);
					std::istringstream arrival_ss(arrival_str);
					std::istringstream burst_ss(burst_str);
					std::istringstream priority_ss(priority_str);
					
					int id, arrival, burst, priority;
					
					id_ss >> id;
					arrival_ss >> arrival;
					burst_ss >> burst;
					priority_ss >> priority;
					
					q.add(id,arrival,burst,priority);
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
			queue<int> turnAroundQ;
			queue<int> waitingQ;
			queue<int> burstQ;
			queue<int> arrivalQ;
			
			// Display Shortest Job First simulation if "sjf" passed in
			if(schedType == "sjf") {
				BurstPriorityQ RQ; 
				
				while(!q.isEmpty() || !RQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// RQ(sorted in order of burst time)
					while(!q.isEmpty() && i >= q.peekArrival()) {
						int id, arrival, burst, priority;
						id = q.peekID();
						arrival = q.peekArrival();
						burst = q.peekBurst();
						priority = q.peekPriority();
						RQ.add(id, arrival, burst, priority);
						q.pop();
					}
					
					// Process ready for execution
					if(!RQ.isEmpty() && i >= RQ.peekArrival()) {
						idle = false;
						outputIdle = false;
						int id = RQ.peekID();
						cout << "Time " << i << " Process " << id << endl;
						i += RQ.peekBurst();
						terminationQ.push(i);
						arrivalQ.push(RQ.peekArrival());
						burstQ.push(RQ.peekBurst());
						RQ.pop();
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
				BurstPriorityQ RQ; 
				std::map<int, bool> map;
				std::map<int, int> burstMap;
				int remainingTime;
				int currentID;
				int currentArrival;
				int totalTimeCount = 0;
				
				while(!q.isEmpty() || !RQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// RQ(sorted in order of burst time).
					while(!q.isEmpty() && i >= q.peekArrival()) {
						int id, arrival, burst, priority;
						id = q.peekID();
						arrival = q.peekArrival();
						burst = q.peekBurst();
						priority = q.peekPriority();
						map[id] = false;
						burstMap[id] = burst;
						totalTimeCount += burst; 
						RQ.add(id, arrival, burst, priority);
						
						// Compare new arrival's burst time to current running 
						// process's remaining burst time. Run the process with
						// lower burst time.
						if(!idle) {
							if(burst > remainingTime) {
							} else if( burst < remainingTime) {
								if(remainingTime >= 1) {
									RQ.add(currentID, currentArrival, remainingTime, 0);
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
								RQ.pop();
							} else { 
								if(id < currentID) {
									if(remainingTime >= 1) {
										RQ.add(currentID, currentArrival, remainingTime, 0);
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
									RQ.pop();
								} 
							}
						} 
						q.pop();
					}
					
					// CPU currently running a process
					if(!idle) {
						if(remainingTime == 1) {
							map[currentID] = true;
							terminationQ.push(i+1);
							arrivalQ.push(currentArrival);
							burstQ.push(burstMap[currentID]);
							if(RQ.isEmpty()) {
								idle = true;
								outputIdle = true;
								cout << "Time " << i+1 << " Idle" << endl;
							}else {
								idle = false;
								outputIdle = false;
								currentID = RQ.peekID();
								currentArrival = RQ.peekArrival();
								remainingTime = RQ.peekBurst();
								cout << "Time " << i+1 << " Process " << currentID << endl;
								RQ.pop();
							}
						} else {
							remainingTime--;
						}
					// CPU is currently idle and a new process in RQ is ready for execution
					} else if(!RQ.isEmpty() && i >= RQ.peekArrival()) {
						currentID = RQ.peekID();
						currentArrival = RQ.peekArrival();
						remainingTime = RQ.peekBurst();
						idle = false;
						outputIdle = false;
						cout << "Time " << i << " Process " << currentID << endl;
						RQ.pop();
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
				PriorityQ RQ; 
				
				while(!q.isEmpty() || !RQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// RQ(sorted in order of burst time)
					while(!q.isEmpty() && i >= q.peekArrival()) {
						int id, arrival, burst, priority;
						id = q.peekID();
						arrival = q.peekArrival();
						burst = q.peekBurst();
						priority = q.peekPriority();
						RQ.add(id, arrival, burst, priority);
						q.pop();
					}
					
					// Process ready for execution
					if(!RQ.isEmpty() && i >= RQ.peekArrival()) {
						idle = false;
						outputIdle = false;
						int id = RQ.peekID();
						cout << "Time " << i << " Process " << id << endl;
						i += RQ.peekBurst();
						terminationQ.push(i);
						arrivalQ.push(RQ.peekArrival());
						burstQ.push(RQ.peekBurst());
						RQ.pop();
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
				PriorityQ RQ; 
				std::map<int, bool> map;
				std::map<int, int> priorityMap;
				std::map<int, int> burstMap;
				int remainingTime;
				int currentID;
				int currentArrival;
				int totalTimeCount = 0;
				
				while(!q.isEmpty() || !RQ.isEmpty()) {
					// If a new process arrives at time i, add to 
					// RQ(sorted in order of priority value). 
					while(!q.isEmpty() && i >= q.peekArrival()) {
						int id, arrival, burst, priority;
						id = q.peekID();
						arrival = q.peekArrival();
						burst = q.peekBurst();
						priority = q.peekPriority();
						map[id] = false;
						burstMap[id] = burst;
						priorityMap[id] = priority;
						totalTimeCount += burst; 
						RQ.add(id, arrival, burst, priority);
						
						// Compare new arrival's priority value to current running 
						// process's priority value. Run the process with
						// lower priority value.
						if(!idle) {
							if(priority > priorityMap[currentID]) {
							} else if( priority < priorityMap[currentID]) {
								if(remainingTime >= 1) {
									RQ.add(currentID, currentArrival, remainingTime, priorityMap[currentID]);
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
								RQ.pop();
							} else { 
								if(id < currentID) {
									if(remainingTime >= 1) {
										RQ.add(currentID, currentArrival, remainingTime, priorityMap[currentID]);
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
									RQ.pop();
								} 
							}
						} 
						q.pop();
					}
					
					// CPU currently running a process
					if(!idle) {
						if(remainingTime == 1) {
							map[currentID] = true;
							terminationQ.push(i+1);
							arrivalQ.push(currentArrival);
							burstQ.push(burstMap[currentID]);
							if(RQ.isEmpty()) {
								idle = true;
								outputIdle = true;
								cout << "Time " << i+1 << " Idle" << endl;
							}else {
								idle = false;
								outputIdle = false;
								currentID = RQ.peekID();
								currentArrival = RQ.peekArrival();
								remainingTime = RQ.peekBurst();
								cout << "Time " << i+1 << " Process " << currentID << endl;
								RQ.pop();
							}
						} else {
							remainingTime--;
						}
					// CPU is currently idle and a new process in RQ is ready for execution
					} else if(!RQ.isEmpty() && i >= RQ.peekArrival()) {
						currentID = RQ.peekID();
						currentArrival = RQ.peekArrival();
						remainingTime = RQ.peekBurst();
						idle = false;
						outputIdle = false;
						cout << "Time " << i << " Process " << currentID << endl;
						RQ.pop();
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
				worstCaseTime = std::max(worstCaseTime, waitingQ.front());
				count++;
				sum += waitingQ.front();
				waitingQ.pop();
			}
			double avgWaitingTime = double(sum) / count;
			printf("Average waiting  time: %.2f\n", avgWaitingTime);
			cout << "Worst-case waiting time: " << worstCaseTime << endl;
		}
	}
	return 0;
}

