###Scheduling Policies OS
- policy.c includes both preemptive and non-preemptive policies.
- Non-preemptive : FCFS, SJF, HRRN and Priority.
- Preemptive : RoundRobin, SRTN.
- To run a policy, go to line 632 of the code and write the policies with the parameters.
- Compile using gcc filename
- Run using ./a.out < input
- The input contains number of process, process_id, arrival time, process length and priority.
- The format for input is shown in config.txt.
