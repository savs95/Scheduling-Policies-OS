/*
 * Author : Apoorv Vikram Singh
 * Roll No : IMT2013006
 * Email : ApoorvVikram.Singh@iiitb.org
 * Date : Sept - 7 - 2015
 * Version : 3
 */

#include<stdio.h>
#include<stdlib.h>

//Process Structure
//tim is the time-stamp
//Forming array of pointers of the structures

struct Process {
	int process_id;
	int arrival_time;
	int process_length;
	int priority;
	int finish_time;
	int done;
	int times_scheduling;
	int length_left;
};

//Like a constructor for a class
struct Process* NewProcess(int id, int time, int len, int pri) {
	struct Process* p = malloc(sizeof(struct Process));
	p->process_id = id;
	p->arrival_time = time;
	p->process_length = len;
	p->priority = pri;
	p->done = 0;
	p->times_scheduling = 0;
	p->length_left = len;
	return p;
}

int comparator(const void * a1 , const void * b1) {
	struct Process * a = *(struct Process**)a1;
	struct Process * b = *(struct Process**)b1;
	if(a == NULL)
		return 1;
	if(b == NULL)
		return 0;
	if(a->arrival_time > b->arrival_time)
		return 1;
	else if(a->arrival_time == b->arrival_time) {
		if(a->arrival_time > b->arrival_time)
			return 0;
		else
			return 1;
	}
	else
		return 0;
}

void RoundRobin(struct Process *ptr[0], int n, int quantum) {
	int i=0, tim=0, size=0, flag=0;
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	if(ptr[0]->arrival_time > tim)
		tim=ptr[0]->arrival_time;
	while(size < n) {
		i=0, flag=0;
		for(i=0;i<n;i++) {
			if((ptr[i]->arrival_time <= tim) && (ptr[i]->done == 0)) {
				ptr[i]->times_scheduling += 1;
				if(ptr[i]->length_left -quantum <= 0) {
					ptr[i]->done = 1;
					tim = tim+ptr[i]->length_left;
					ptr[i]->length_left = 0;
					size += 1;
					ptr[i]->finish_time = tim;
					flag = 1;
				}
				else {
					tim += quantum;
					ptr[i]->length_left -= quantum;
					flag = 1;
				}
			}
		}
		if(flag == 0)
			tim += 1;
	}
}

//Does round robin well with the given quantum, with FCFS

void SRTN(struct Process *ptr[0], int n) {
	int i=0, tim=0, size=0, srtn=100000, temp, index=-1, prev_index=-1;
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	if(ptr[0]->arrival_time > tim)
		tim=ptr[0]->arrival_time;
	while(size < n) {
		i=0, srtn=10000, index=-1;
		for(i=0;i<n;i++) {
			if(ptr[i]->arrival_time <= tim && ptr[i]->done ==0){
				temp= ptr[i]->length_left;
				if (temp < srtn){
					srtn = temp;
					index = i;
				}
			}
		}
		if(index!=-1) {
			tim++;
			ptr[index]->length_left -= 1;
			if(ptr[index]->length_left == 0) {
				ptr[index]->done = 1;
				size++;
				ptr[index]->finish_time = tim;
				if(index!=prev_index)
					ptr[index]->times_scheduling += 1;
				prev_index = index;
			}
			else {
				if(index!=prev_index)
					ptr[index]->times_scheduling += 1;
				prev_index = index;
			}

		}
		else
			tim++;
	}
}

//SRTN, checks at every arrival

void FCFS(struct Process *ptr[0], int n) {
	printf("------------------------\n");
	int waiting, running, terminated;
	int tim=0,i=0;
	float mean_wait=0, mean_turnaround=0;
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	for(i=0;i<n;i++) {
		if(tim-(ptr[i]->arrival_time)>0)
			waiting = tim-(ptr[i]->arrival_time);
		else
			waiting = 0;
		tim += ptr[i]->process_length;
		running = ptr[i]->process_length;
		terminated = waiting+running;
		mean_wait += waiting;
		mean_turnaround += terminated;


		ptr[i]->finish_time = ptr[i]->arrival_time+waiting+running;
		ptr[i]->done = 1;
		ptr[i]->times_scheduling = 1;
	}

}

//Updates everything in the structure of process FCFS

void SJF(struct Process *ptr[0], int n) {
	int waiting, running, terminated, temp, temp_id;
	int tim=0,i=0,j=0,k=0;
	float mean_wait=0, mean_turnaround=0;
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	for(i=0;i<n;i++){
		if(tim < ptr[i]->arrival_time)
			tim = ptr[i]->arrival_time;
		if((ptr[i]->arrival_time <= tim) && (ptr[i]->done == 0)) {
			temp = ptr[i]->process_length;
			temp_id = i;
			for(j=0;j<n;j++) {
				if((ptr[j]->arrival_time < tim) && (ptr[j]->process_length < temp) && (ptr[j]->done == 0)) {
					temp = ptr[j]->process_length;
					temp_id = j;
				}
			}
			ptr[temp_id]->done=1;

			if(tim - ptr[temp_id]->arrival_time > 0)
				waiting = tim - ptr[temp_id]->arrival_time;
			else
				waiting = 0;

			terminated = waiting+ptr[temp_id]->process_length;
			running = ptr[temp_id]->process_length;
			mean_wait += waiting;
			mean_turnaround += terminated;

			ptr[temp_id]->finish_time = ptr[temp_id]->arrival_time+waiting+running;
			ptr[temp_id]->done = 1;
			ptr[temp_id]->times_scheduling = 1;
			tim += ptr[temp_id]->process_length;

		}
		else {
			for(k=0;k<n;k++){
				if((ptr[k]->arrival_time <= tim) && (ptr[k]->done == 0)) {
					temp = ptr[k]->process_length;
					temp_id = k;
					for(j=0;j<n;j++) {
						if((ptr[j]->arrival_time < tim) && (ptr[j]->process_length < temp) && (ptr[j]->done == 0)) {
							temp = ptr[j]->process_length;
							temp_id = j;
						}
					}
					ptr[temp_id]->done=1;

					if(tim - ptr[temp_id]->arrival_time > 0)
						waiting = tim - ptr[temp_id]->arrival_time;
					else
						waiting = 0;

					terminated = waiting+ptr[temp_id]->process_length;
					running = ptr[temp_id]->process_length;
					mean_wait += waiting;
					mean_turnaround += terminated;

					ptr[temp_id]->finish_time = ptr[temp_id]->arrival_time+waiting+running;
					ptr[temp_id]->done = 1;
					ptr[temp_id]->times_scheduling = 1;
					tim += ptr[temp_id]->process_length;

				}

			}
		}
	}

}
//checks for shortest job first after completion of every process

void HRRN(struct Process *ptr[0], int n) {
	int waiting, running, terminated, temp, temp_id;
	int tim=0,i=0,j=0, k=0;
	float mean_wait=0, mean_turnaround=0, response_ratio2, response_ratio1;
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	for(i=0; i<n; i++) {
		if(tim < ptr[i]->arrival_time)
			tim = ptr[i]->arrival_time;
		if((ptr[i]->arrival_time <= tim) && (ptr[i]->done == 0)) {
			temp = ptr[i]->process_length;
			temp_id = i;
			response_ratio1 = (float)((tim-ptr[i]->arrival_time)+(ptr[i]->process_length))/(float)(ptr[i]->process_length);
			for(j=0;j<n;j++) {
				if((ptr[j]->arrival_time < tim) && (ptr[j]->done == 0)){
					response_ratio2 = (float)((tim-ptr[j]->arrival_time)+(ptr[j]->process_length))/(float)(ptr[j]->process_length);
					if(response_ratio1 < response_ratio2) {
						temp = ptr[j]->process_length;
						temp_id = j;
						response_ratio1 = response_ratio2;
					}
				}
			}
			ptr[temp_id]->done=1;

			if(tim - ptr[temp_id]->arrival_time > 0)
				waiting = tim - ptr[temp_id]->arrival_time;
			else
				waiting = 0;

			terminated = waiting+ptr[temp_id]->process_length;
			running = ptr[temp_id]->process_length;
			mean_wait += waiting;
			mean_turnaround += terminated;

			ptr[temp_id]->finish_time = ptr[temp_id]->arrival_time+waiting+running;
			ptr[temp_id]->done = 1;
			ptr[temp_id]->times_scheduling = 1;
			tim += ptr[temp_id]->process_length;
		}
		else {
			for(k=0;k<n;k++){
				if((ptr[k]->arrival_time <= tim) && (ptr[k]->done == 0)) {
					temp = ptr[k]->process_length;
					temp_id = k;
					response_ratio1 = (float)((tim-ptr[k]->arrival_time)+(ptr[k]->process_length))/(float)(ptr[k]->process_length);
					for(j=0;j<n;j++) {
						if((ptr[j]->arrival_time < tim) && (ptr[j]->done == 0)){
							response_ratio2 = (float)((tim-ptr[j]->arrival_time)+(ptr[j]->process_length))/(float)(ptr[j]->process_length);
							if(response_ratio1 < response_ratio2) {
								temp = ptr[j]->process_length;
								temp_id = j;
								response_ratio1 = response_ratio2;
							}
						}
					}
					ptr[temp_id]->done=1;

					if(tim - ptr[temp_id]->arrival_time > 0)
						waiting = tim - ptr[temp_id]->arrival_time;
					else
						waiting = 0;

					terminated = waiting+ptr[temp_id]->process_length;
					running = ptr[temp_id]->process_length;

					ptr[temp_id]->finish_time = ptr[temp_id]->arrival_time+waiting+running;
					ptr[temp_id]->done = 1;
					ptr[temp_id]->times_scheduling = 1;
					tim += ptr[temp_id]->process_length;
					mean_wait += waiting;
					mean_turnaround += terminated;

				}

			}
		}

	}

}
//Checks for highest response ratio every time a process finishes

void priority (struct Process *ptr[0], int n) {
	int waiting, running, terminated, temp, temp_id;
	int tim=0,i=0,j=0,k=0;
	float mean_wait=0, mean_turnaround=0;
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	for(i=0;i<n;i++){
		if(tim < ptr[i]->arrival_time)
			tim = ptr[i]->arrival_time;
		if((ptr[i]->arrival_time <= tim) && (ptr[i]->done == 0)) {
			temp = ptr[i]->priority;
			temp_id = i;
			for(j=0;j<n;j++) {
				if((ptr[j]->arrival_time < tim) && (ptr[j]->priority < temp) && (ptr[j]->done == 0)) {
					temp = ptr[j]->priority;
					temp_id = j;
				}
			}
			ptr[temp_id]->done=1;

			if(tim - ptr[temp_id]->arrival_time > 0)
				waiting = tim - ptr[temp_id]->arrival_time;
			else
				waiting = 0;

			terminated = waiting+ptr[temp_id]->process_length;
			running = ptr[temp_id]->process_length;

			ptr[temp_id]->finish_time = ptr[temp_id]->arrival_time+waiting+running;
			ptr[temp_id]->done = 1;
			ptr[temp_id]->times_scheduling = 1;
			tim += ptr[temp_id]->process_length;
			mean_wait += waiting;
			mean_turnaround += terminated;

		}
		else {
			for(k=0;k<n;k++){
				if((ptr[k]->arrival_time <= tim) && (ptr[k]->done == 0)) {
					temp = ptr[k]->priority;
					temp_id = k;
					for(j=0;j<n;j++) {
						if((ptr[j]->arrival_time < tim) && (ptr[j]->priority < temp) && (ptr[j]->done == 0)) {
							temp = ptr[j]->priority;
							temp_id = j;
						}
					}
					ptr[temp_id]->done=1;

					if(tim - ptr[temp_id]->arrival_time > 0)
						waiting = tim - ptr[temp_id]->arrival_time;
					else
						waiting = 0;

					terminated = waiting+ptr[temp_id]->process_length;
					running = ptr[temp_id]->process_length;

					ptr[temp_id]->finish_time = ptr[temp_id]->arrival_time+waiting+running;
					ptr[temp_id]->done = 1;
					ptr[temp_id]->times_scheduling = 1;
					tim += ptr[temp_id]->process_length;
					mean_wait += waiting;
					mean_turnaround += terminated;

				}

			}
		}
	}


}
//Same like SJF only with priority

int CheckQueue(struct Process *queue[0], int n, int tim) {
	int i=0, flag=0;
	for(i=0;i<n;i++) {
		if(queue[i]->arrival_time <= tim && queue[i]->done == 0)
			flag++;
	}
	if (flag==0)
		return 0;
	else
		return 1;
}

//Checks if the queue is empty or not

int QuantumRR(struct Process *ptr[0], int n, int tim, int quantum, int *prev) {
	int i=0, flag=0, ret=-1, temp=-1;
	if(*prev != -1) {
		for(i=0;i<n;i++) {
			if(ptr[i]->process_id == *prev) {
				temp = i;
				break;
			}
		}
	}

	if(temp==-1) {
		for(i=0;i<n;i++) {
			if(ptr[i]->arrival_time <= tim && ptr[i]->done ==0) {
				if(ptr[i]->length_left - quantum > 0) {
					ptr[i]->length_left -= quantum;
					if(*prev != ptr[i]->process_id) {
						ptr[i]->times_scheduling ++;
						*prev = ptr[i]->process_id;

					}
					flag++;
					ret = quantum;
					break;
				}
				else {
					ret = ptr[i]->length_left;
					ptr[i]->length_left = 0;
					ptr[i]->done = 1;
					ptr[i]->finish_time = tim+ret;
					if(*prev != ptr[i]->process_id) {
						ptr[i]->times_scheduling ++;
						*prev = ptr[i]->process_id;
					}
					flag++;
					break;
				}
			}
		}
	}
	else {
		for(i=temp+1;i<n;i++) {
			if(ptr[i]->arrival_time <= tim && ptr[i]->done ==0) {
				if(ptr[i]->length_left - quantum > 0) {
					ptr[i]->length_left -= quantum;
					if(*prev != ptr[i]->process_id) {
						ptr[i]->times_scheduling ++;
						*prev = ptr[i]->process_id;

					}
					flag++;
					ret = quantum;
					break;
				}
				else {
					ret = ptr[i]->length_left;
					ptr[i]->length_left = 0;
					ptr[i]->done = 1;
					ptr[i]->finish_time = tim+ret;
					if(*prev != ptr[i]->process_id) {
						ptr[i]->times_scheduling ++;
						*prev = ptr[i]->process_id;
					}
					flag++;
					break;
				}
			}




		}

		if(flag==0) {
			for(i=0;i<n;i++) {
				if(ptr[i]->arrival_time <= tim && ptr[i]->done ==0) {
					if(ptr[i]->length_left - quantum > 0) {
						ptr[i]->length_left -= quantum;
						if(*prev != ptr[i]->process_id) {
							ptr[i]->times_scheduling ++;
							*prev = ptr[i]->process_id;

						}
						flag++;
						ret = quantum;
						break;
					}
					else {
						ret = ptr[i]->length_left;
						ptr[i]->length_left = 0;
						ptr[i]->done = 1;
						ptr[i]->finish_time = tim+ret;
						if(*prev != ptr[i]->process_id) {
							ptr[i]->times_scheduling ++;
							*prev = ptr[i]->process_id;
						}
						flag++;
						break;
					}
				}
			}
		}
	}
	return ret;

}

//RR only for a quantum



void MLQ(struct Process *q1[0], int i1, struct Process *q2[0], int i2, struct Process *q3[0], int i3, struct Process* ptr[0], int n) {
	int t1,t2,t3,tim=0, flag=0, i=0, prev=-1;
	while(flag < n) {
		flag = 0;
		for(i=0;i<n;i++) {
			if(ptr[i]->done == 1)
				flag++;
		}
		if(CheckQueue(q1,i1,tim)!=0) {
			t1 = QuantumRR(q1,i1,tim,1,&prev);
			tim += t1;
		}
		else if (CheckQueue(q2,i2,tim)!=0) {
			t2 = QuantumRR(q2,i2,tim,2,&prev);
			tim += t2;
		}
		else if (CheckQueue(q2,i2,tim)!=0) {
			t3 = QuantumRR(q3,i3,tim,3,&prev);
			tim += t3;
		}
		else {
			tim += 1;
		}
	}
}

//MLQ with simple code, it keeps check of every queue on top after it's quantum

void QAssgn(struct Process *ptr[0], int n) {
	int i=0, i1=0, i2=0, i3=0;
	// st<=2, 2<st<=5, st>=6
	qsort(ptr , n , sizeof(struct Process*) , &comparator);
	struct Process *q1[20];
	struct Process *q2[20];
	struct Process *q3[20];
	for(i=0;i<n;i++) {
		if (ptr[i]->process_length <=2 ) {
			q1[i1]=ptr[i];
			i1++;
		}
		else if ((ptr[i]->process_length >2) && (ptr[i]->process_length <=5)) {
			q2[i2]=ptr[i];
			i2++;
		}
		else {
			q3[i3]=ptr[i];
			i3++;
		}
	}
	MLQ(q1,i1,q2,i2,q3,i3,ptr,n);
}
//Assigns the queue and then calls MLQ

void ChangeQueue(struct Process* q1[0], int *s1, struct Process* q2[0], int *s2, int tim) {
	int i=0;
	for(i=0;i<*s1;i++) {
		if(q1[i]->arrival_time <= tim) {
			if(q1[i]->done == 1) {
				q1[i]=NULL;
				*s1 -= 1;
			}
			else {
				q2[*s2]=q1[i];
				q1[i]=NULL;
				*s2 += 1;
			}
		}
	}
	qsort(q1 , *s1 , sizeof(struct Process*) , &comparator);
}
//Changes queue for MLFQ

void MLFQ(struct Process* ptr[0], int n) {
	int t1,t2,t3,tim=0, flag=0, i=0, prev=-1;
	int i1=0,i2=0,i3=0;
	struct Process *q1[20];
	struct Process *q2[20];
	struct Process *q3[20];
	for(i=0;i<n;i++) {
		q1[i]=ptr[i];
		i1 = i;
	}
	while(flag < n) {
		flag = 0;
		for(i=0;i<n;i++) {
			if(ptr[i]->done == 1)
				flag++;
		}
		if(CheckQueue(q1,i1,tim)!=0) {
			t1 = QuantumRR(q1,i1,tim,1,&prev);
			ChangeQueue(q1,&i1,q2,&i2,tim);
			tim += t1;
		}
		else if (CheckQueue(q2,i2,tim)!=0) {
			t2 = QuantumRR(q2,i2,tim,2,&prev);
			ChangeQueue(q2,&i2,q3,&i3,tim);
			tim += t2;
		}
		else if (CheckQueue(q2,i2,tim)!=0) {
			t3 = QuantumRR(q3,i3,tim,3,&prev);
			tim += t3;
		}
		else {
			tim += 1;
		}
	}
}
//MLFQ implementation, similar to MLQ



int main() {
	struct Process* ptr[20];
	int n,a,b,c,d,i;
	float mean_wt=0, mean_tat=0, temp;
	scanf("%d",&n);
	for(i=0;i<n;i++){
		scanf("%d %d %d %d", &a, &b, &c, &d);
		struct Process* p1 = NewProcess(a,b,c,d);
		ptr[i]=p1;
	}
	RoundRobin(ptr,n,3);
	//QAssgn(ptr,n);
	for(i=0;i<n;i++) {
		temp = (ptr[i]->finish_time - ptr[i]->arrival_time -ptr[i]->process_length);
		mean_wt += temp;
		mean_tat += ptr[i]->finish_time - ptr[i]->arrival_time;
	}
	for(i=0;i<n;i++) {
		printf("Process Id %d \n", ptr[i]->process_id );
		printf("Process length %d \n", ptr[i]->process_length);
		printf("Arrival time %d \n", ptr[i]->arrival_time);
		printf("Waiting Time %d \n", ptr[i]->finish_time - ptr[i]->arrival_time -ptr[i]->process_length);
		printf("Finish Time %d \n", ptr[i]->finish_time);
		printf("Scheduled times %d \n", ptr[i]->times_scheduling);
		printf("TAT %d \n", ptr[i]->finish_time- ptr[i]->arrival_time);
		printf("------------------------------------\n");
	}
	printf("Mean wait time %f \n",mean_wt/n);
	printf("Mean TAT %f \n", mean_tat/n);

	//Prints all the process details
	return 0;
}

