# Build & Run #
**Build:**	make

  **Run:**	./run &lt;file name&gt; &lt;SJF, SRTF, NP, PP&gt;
# About #
This program displays a simulation run of a CPU Scheduling algorithm listed below:

- Shortest Job First 
- Shortest Remaining Time First
- Non-Preemptive Priority
- Preemptive Priority

Calculations for **CPU utilization, average waiting time, and worst-case  waiting time** are displayed at the end of each run. 

Sample output for: **./run test5.txt SRTF**

	Time 0 Process 1
	Time 13 Process 3
	Time 14 Process 5
	Time 15 Process 3
	Time 19 Process 1
	Time 26 Process 4
	Time 46 Idle
	Time 200 Process 2
	CPU Utilization: 24%
	Average waiting time: 6.60
	Worst-case waiting time: 26

# Test Files #
Files contain data inputs: process Id, arrival time, burst time, priority
# Scheduling Algorithms #
For more info: 
http://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/5_CPU_Scheduling.html
