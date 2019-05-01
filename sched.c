#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct job
{
    int job_id;
    int arrival_time;
    int duration;
    int start_time;
    int finish_time;
    int total_time;
    int response_time;
    int run_progress;
};

enum Compare_t
{
    kArrival = 0,
    kArrivalDur_SJF = 1,
    kDuration_SJF = 2,
    kDuration_BJF = 3,
    kStart_Time = 4,
    kFinish_Time = 5
};

void getJobList(struct job job_list[], int *num_of_jobs);
void resetList(struct job job_list[], int num_of_jobs);
int getCurrentJobList(struct job *job_list, struct job *current_job_list, int num_of_jobs, int timer);
int getNextJobTime(struct job *job_list, int num_of_jobs);

void FIFO_Scheduler(struct job *job_list, int num_of_jobs);
void SJF_Scheduler(struct job *job_list, int num_of_jobs);
void BJF_Scheduler(struct job *job_list, int num_of_jobs);
void STCF_Scheduler(struct job *job_list, int num_of_jobs);
void RR_Scheduler(struct job *job_list, int num_of_jobs);

void calculateTotalTime(struct job* running_job);
void calculateResponseTime(struct job* running_job);

void swap(struct job* a, struct job* b);
void sortJobs(struct job job_list[], int num_of_jobs, enum Compare_t comparison);

void printSchedulerResults(struct job* resulting_job_list, int num_of_jobs);
void printJobs(struct job* job_list, int num_of_jobs);


int main()
{
    struct job job_list[100];
    int num_of_jobs;


    getJobList(job_list, &num_of_jobs);

    for(int i = 0; i < num_of_jobs; i++)
    {
            printf("JOB #%i: job_id: %i arrival_time: %i duration: %i\n", 
                i + 1, job_list[i].job_id, job_list[i].arrival_time, job_list[i].duration);
    }

    sortJobs(job_list, num_of_jobs, kArrival);

    // printJobs(job_list, num_of_jobs);

    FIFO_Scheduler(job_list, num_of_jobs);
    SJF_Scheduler(job_list, num_of_jobs);
    BJF_Scheduler(job_list, num_of_jobs);
    STCF_Scheduler(job_list, num_of_jobs);
    RR_Scheduler(job_list, num_of_jobs);

    
    return 0;
}

void resetList(struct job job_list[], int num_of_jobs)
{
    for(int i = 0; i < num_of_jobs; i++)
    {
        job_list[i].start_time = -1;
        job_list[i].finish_time = -1;
        job_list[i].total_time = -1;
        job_list[i].response_time = -1;
        job_list[i].run_progress = 0;
    }
}

void getJobList(struct job job_list[], int *num_of_jobs)
{
    // 1. GET JOB LIST from JOBS.DAT
    FILE *fp;

    printf("Opening jobs.dat file.\n");
    fp = fopen("jobs.dat", "r");
    if(fp == NULL)
    {
        perror("Error while opening file.\n");
        // exit(EXIT_FAILURE);
    }

    char line[1000];
    int job_id, arrival_time, duration;
    int n, index = 0;

    while(fgets(line, 1000, fp) != NULL)
    {
        n = sscanf(line, "%i %i %i\n", &job_id, &arrival_time, &duration);
        if(n > 0)
        {
            job_list[index].job_id = job_id;
            job_list[index].arrival_time = arrival_time;
            job_list[index].duration = duration;
            job_list[index].start_time = -1;
            job_list[index].finish_time = -1;
            job_list[index].total_time = -1;
            job_list[index].response_time = -1;
            job_list[index].run_progress = 0;
        }
        index++;
    }
    fclose(fp);

    // Store num_of_jobs
    *num_of_jobs = index;
}



int getCurrentJobList(struct job *job_list, struct job *current_job_list, int num_of_jobs, int timer)
{
	int num_of_curr = 0;
    // printf("\ngetting curr job list: %i\n", num_of_jobs);
	for (int i = 0; i < num_of_jobs; i++)
	{
		// printf("arrival time: job[%i]: %i %i  %i\n", job_list[i].job_id, job_list[i].arrival_time, job_list[i].finish_time, timer);
		// printf("bool: %i\n", (job_list[i].finish_time < 0) && (job_list[i].arrival_time <= timer));
		if((job_list[i].finish_time < 0) & (job_list[i].arrival_time <= timer))
		{
			current_job_list[num_of_curr] = job_list[i];
			num_of_curr++;
		}
	}
    // printf("\nreturning curr job list: %i\n", num_of_curr);
    return num_of_curr;
}

void FIFO_Scheduler(struct job *job_list, int num_of_jobs)
{
    int timer = job_list[0].arrival_time;
    printf("\n\t\t\t\tFIFO Scheduler\n");
    printf("-----------------------------------------------------------------------------------\n");

    for(int i = 0; i < num_of_jobs; i++)
    {
        if(job_list[i].arrival_time > timer)
        {
            timer = job_list[i].arrival_time;
        }
        job_list[i].start_time = timer;
        timer += job_list[i].duration;
        job_list[i].finish_time = timer;
        calculateTotalTime(&job_list[i]);
        calculateResponseTime(&job_list[i]);
    }

    printSchedulerResults(job_list, num_of_jobs);
    resetList(job_list, num_of_jobs);
}

void SJF_Scheduler(struct job *job_list, int num_of_jobs)
{
    printf("\n\t\t\t\tSJF Scheduler\n");
    printf("-----------------------------------------------------------------------------------\n");
    struct job current_job_list[100];

    int timer = job_list[0].arrival_time;
    int num_of_curr = 0;
    
   
   for(int i = 0; i < num_of_jobs; i++)
   {
    	num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);

        if(num_of_curr == 0)
        {
            timer = getNextJobTime(job_list, num_of_jobs);
            num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);
        }
    	sortJobs(current_job_list, num_of_curr, kDuration_SJF);
        // printf("\ncurr list\n");
        // printJobs(current_job_list, num_of_curr);
        // printf("\n");

    	for(int i = 0; i < num_of_jobs; i++)
    	{
    		if( job_list[i].job_id == current_job_list[0].job_id)
    		{
                job_list[i].start_time = timer;
                // printf("id: %i start: %i timer: %i\n", job_list[i].job_id, job_list[i].start_time, timer);
    			job_list[i].finish_time = timer + job_list[i].duration;
    			timer = job_list[i].finish_time;
                calculateTotalTime(&job_list[i]);
                calculateResponseTime(&job_list[i]);
    		}
    	}
    }

    sortJobs(job_list, num_of_jobs, kStart_Time);
    printSchedulerResults(job_list, num_of_jobs);
    resetList(job_list, num_of_jobs);
    /*
        1. Get Current Jobs
            getCurrentJobList(current_job_list);
        2. sortJobs(current_job_list, num_of_jobs, kDurantion_SJF);
        3. Run current_job_list[0]
    */
}

void BJF_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\n\t\t\t\tBJF Scheduler\n");
    printf("-----------------------------------------------------------------------------------\n");

    struct job current_job_list[100];

    int timer = job_list[0].arrival_time;
    int num_of_curr = 0;

  	for(int i = 0; i < num_of_jobs; i++)
  	{

  		num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);
        if(num_of_curr == 0)
        {
            timer = getNextJobTime(job_list, num_of_jobs);
            num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);
        }
  		sortJobs(current_job_list, num_of_curr, kDuration_BJF);


  		for ( int i = 0; i < num_of_jobs; i++)
  		{
  			if( job_list[i].job_id == current_job_list[0].job_id)
  			{
  				job_list[i].start_time = timer;
    			job_list[i].finish_time = timer + job_list[i].duration;
    			timer = job_list[i].finish_time;
                calculateTotalTime(&job_list[i]);
                calculateResponseTime(&job_list[i]);
  			}
  		}
  	}
  	sortJobs(job_list, num_of_jobs, kStart_Time);
    printSchedulerResults(job_list, num_of_jobs);
    resetList(job_list, num_of_jobs);
}

void STCF_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\n\t\t\t\tSTCF Scheduler\n");
    printf("-----------------------------------------------------------------------------------\n");  

    int timer = job_list[0].arrival_time;
    int done = 1;
    int num_of_curr = 0;
    struct job current_job_list[100];

    do
    {
        done = 1;
        num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);

        // if 0 jobs returned, update timer
        if(num_of_curr == 0)
        {
            timer = getNextJobTime(job_list, num_of_jobs);
            num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);
        }
        

        sortJobs(current_job_list, num_of_curr, kDuration_SJF);
        // if(timer == 10)
        // {
        //     for(int i=0; i < num_of_curr; i++)
        //     {
        //         printf("CURRENT: %i\n", current_job_list[i].job_id);
        //     }
        // }
        // printf("running job %i : %i\n", current_job_list[0].job_id, timer);
        // Evaluate if job has finished
        for(int i=0; i < num_of_jobs; i++)
        {
            if(job_list[i].job_id == current_job_list[0].job_id)
            {
                if(job_list[i].start_time < 0)
                {
                    job_list[i].start_time = timer;
                }
                job_list[i].run_progress += 1;
                if(job_list[i].run_progress == job_list[i].duration)
                {
                    job_list[i].finish_time = timer + 1;
                    // printf("finished job %i: %i\n", job_list[i].job_id, job_list[i].finish_time);
                    calculateTotalTime(&job_list[i]);
                    calculateResponseTime(&job_list[i]);
                }
            }
            if(job_list[i].finish_time < 0)
            {
                done = 0;
            }
        }
        timer += 1;

    }while(!done);

    sortJobs(job_list, num_of_jobs, kFinish_Time);
    printSchedulerResults(job_list, num_of_jobs);
    resetList(job_list, num_of_jobs);
}

void RR_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\n\t\t\t\tRR Scheduler\n");
    printf("-----------------------------------------------------------------------------------\n");

    int done = 1;
    int num_of_curr = 0;
    int timer = job_list[0].arrival_time;
    struct job current_job_list[100];

    do
    {
        done = 1;
        num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);

        // if 0 jobs returned, update timer
        if(num_of_curr == 0)
        {
            timer = getNextJobTime(job_list, num_of_jobs);
            num_of_curr = getCurrentJobList(job_list, current_job_list, num_of_jobs, timer);
        }


        // printf("enter loop: %i\n", num_of_curr);
        // for(int i=0; i < num_of_curr; i++)
        // {
        //     printf("CURRENT: %i\n", current_job_list[i].job_id);
        // }
        for(int curr_index = 0; curr_index < num_of_curr; curr_index++)
        {
            // printf("running job %i : %i\n", current_job_list[curr_index].job_id, timer);
            for(int job_index = 0; job_index < num_of_jobs; job_index++)
            {
                if(job_list[job_index].job_id == current_job_list[curr_index].job_id)
                {
                    if(job_list[job_index].start_time < 0)
                    {
                        job_list[job_index].start_time = timer;
                    }

                    job_list[job_index].run_progress++;

                    if(job_list[job_index].run_progress == job_list[job_index].duration)
                    {
                        job_list[job_index].finish_time = timer + 1;
                        calculateTotalTime(&job_list[job_index]);
                        calculateResponseTime(&job_list[job_index]);
                    }
                }
                if(job_list[job_index].finish_time < 0)
                {
                    done = 0;
                }
            }
            timer++;
        }
        
    }while(!done);

    sortJobs(job_list, num_of_jobs, kFinish_Time);
    printSchedulerResults(job_list, num_of_jobs);
    resetList(job_list, num_of_jobs);
}

void calculateTotalTime(struct job* running_job)
{
    running_job->total_time = running_job->finish_time - running_job->arrival_time;
}

void calculateResponseTime(struct job* running_job)
{
    running_job->response_time = running_job->start_time - running_job->arrival_time;
}

void swap(struct job* a, struct job* b)
{
    struct job temp = *a; 
    *a = *b; 
    *b = temp;
}


void sortJobs(struct job job_list[], int num_of_jobs, enum Compare_t comparison)
{
    // Bubble Sort
    for(int i = 0; i < num_of_jobs; i++)
    {
        for(int j = 0; j < num_of_jobs - i - 1; j++)
        {
            if(comparison == kArrival)
            {
                if(job_list[j].arrival_time > job_list[j+1].arrival_time)
                {
                    swap(&job_list[j], &job_list[j+1]);
                }

            }
            else if(comparison == kArrivalDur_SJF)
            {
            	if(job_list[j].arrival_time > job_list[j+1].arrival_time && job_list[j].duration > job_list[j+1].duration)
            	{
            		swap(&job_list[j], &job_list[j+1]);
            	}		

            }
            else if(comparison == kDuration_SJF)
            {
                if(job_list[j].duration > job_list[j+1].duration)
                {
                    swap(&job_list[j], &job_list[j+1]);
                }


            }
            else if(comparison == kDuration_BJF)
            {
                if(job_list[j].duration < job_list[j+1].duration)
                {
                    swap(&job_list[j], &job_list[j+1]);
                }
            }
            else if(comparison == kStart_Time)
            {
                if(job_list[j].start_time > job_list[j+1].start_time)
                {
                    swap(&job_list[j], &job_list[j+1]);
                }
            }
            else if(comparison == kFinish_Time)
            {
                if(job_list[j].finish_time > job_list[j+1].finish_time)
                {
                    swap(&job_list[j], &job_list[j+1]);
                }
            }
        }
    }
}

void printSchedulerResults(struct job* resulting_job_list, int num_of_jobs)
{
    printf("\t\t Start Time \t Finish Time \t Total Time \t Response Time \n");
    printf("\t      ===================================================================\n");
    int avg_total_time = 0, avg_response_time = 0;
    for(int i = 0; i < num_of_jobs; i++)
    {
        printf("JOB %4i: %12i %16i %16i %16i\n", 
            resulting_job_list[i].job_id, resulting_job_list[i].start_time, resulting_job_list[i].finish_time, 
            resulting_job_list[i].total_time, resulting_job_list[i].response_time);

        avg_total_time += resulting_job_list[i].total_time;
        avg_response_time += resulting_job_list[i].response_time;
    }

    avg_total_time /= num_of_jobs;
    avg_response_time /= num_of_jobs;
    printf("\nAVG Total Time: %i AVG Response Time: %i\n\n", avg_total_time, avg_response_time);
}

void printJobs(struct job* job_list, int num_of_jobs)
{
    for(int i = 0; i < num_of_jobs; i++)
    {
            printf("JOB #%i: job_id: %i arrival_time: %i duration: %i start: %i finish: %i\n", 
                i + 1, job_list[i].job_id, job_list[i].arrival_time, job_list[i].duration, job_list[i].start_time, job_list[i].finish_time);
    }
}

int getNextJobTime(struct job* job_list, int num_of_jobs)
{
    int next_time = -1;
    for(int i = 0; i < num_of_jobs; i++)
    {
        if((next_time < 0) && (job_list[i].finish_time < 0))
        {
            next_time = job_list[i].arrival_time;
        }
        else if((job_list[i].arrival_time < next_time) && (job_list[i].finish_time > 0))
        {
            next_time = job_list[i].arrival_time;
        }
    }
    return next_time;
}