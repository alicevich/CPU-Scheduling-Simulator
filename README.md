## CPU-Scheduling-Simulator ##

**Build:**	make

  **Run:**	./a.out <file name\> <SJF, SRTF, NP, PP\>

# About #
This program displays a simulation run of a CPU Scheduling algorithm listed below:

- Shortest Job First 
- Shortest Remaining Time First
- Non-Preemptive Priority
- Preemptive Priority

Calculations for **CPU utilization, average waiting time, and worst-case  waiting time** are displayed at the end of each run. 

Sample output for test4.txt:

	Time 0 Process 1
	Time 2 Process 2
	Time 4 Process 3
	Time 5 Process 2
	Time 7 Process 4
	Time 10 Process 1
	CPU Utilization: 100%
	Average waiting time: 2.75
	Worst-case waiting time: 8

# Test Files #
Files contain data inputs: process Id, arrival time, burst time, priority

# Scheduling Algorithms #

For more info: 
http://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/5_CPU_Scheduling.html

