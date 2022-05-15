#include "sched.h"
#include <stdio.h>

void FCFS (
    int number_of_jobs,
    const int job_submitted_time [],
    const int job_required_time [],
    int job_sched_start []
) {
	int i = 0, time = 0;
	for (i = 0; i < number_of_jobs; i++) {
		if (time <= job_submitted_time[i]) {
			time = job_submitted_time[i];
		}
		job_sched_start[i] = time;
        time += job_required_time[i];
	}
}

struct Job {
	int id;
	int submit;
	int require;
};

struct Job jobs[2005];

int cmp(struct Job a, struct Job b) {
	if (a.require == b.require) {
		if (a.submit == b.submit) {
			return a.id - b.id;
		}
		return a.submit - b.submit;
	}
	return a.require - b.require;
}

int getStandard(int i, int j){
	struct Job key = jobs[i];
	while (i < j){
		while (i < j && cmp(jobs[j], key) >= 0){
			j--;
		}
		if (i < j){
			jobs[i] = jobs[j];
		}
		while (i < j && cmp(jobs[i], key) <= 0){
			i++;
		}
		if (i < j){
			jobs[j] = jobs[i];
		}
	}
	jobs[i] = key;
	return i;
}

void qsort(int low, int high) {
	if (low < high) {
		int standard = getStandard(low, high);
		qsort(low, standard - 1);
		qsort(standard + 1, high);
	}
}

void SJF (
    int number_of_jobs,
    const int job_submitted_time [],
    const int job_required_time [],
    int job_sched_start []
) {
	int cnt = 0, i = 0, time = 0;
	jobs[i].id = i;
    jobs[i].submit = job_submitted_time[i];
    jobs[i].require = job_required_time[i];
	for (cnt = 0; cnt < number_of_jobs; cnt++) {
		if (time <= jobs[cnt].submit) {
			time = jobs[cnt].submit;
		}
		job_sched_start[jobs[cnt].id] = time;
        time += jobs[cnt].require;
	//	printf("now time: %d\n", time);
		if (time <= job_submitted_time[cnt+1]) {
			time = job_submitted_time[cnt+1];
		}
		int sid = cnt;
		do {
			sid++;
			if (jobs[sid].id > 0) continue;
			jobs[sid].id = sid;
			jobs[sid].submit = job_submitted_time[sid];
	        jobs[sid].require = job_required_time[sid];
		} while (sid < number_of_jobs-1 && job_submitted_time[sid+1] <= time);
		qsort(cnt+1, sid);
	//	int tmp;
	//	for (tmp = cnt+1; tmp <= sid; tmp++) {
	//		printf("%d ", jobs[tmp].id);
	//	}
	//	printf("\n");
	}
}
