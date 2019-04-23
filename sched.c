#include <stdio.h>
#include <stdlib.h>

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

void getJobList(struct job job_list[], int *num_of_jobs);

void FIFO_Scheduler(struct job *job_list, int num_of_jobs);
void SJF_Scheduler(struct job *job_list, int num_of_jobs);
void BJF_Scheduler(struct job *job_list, int num_of_jobs);
void STCF_Scheduler(struct job *job_list, int num_of_jobs);
void RR_Scheduler(struct job *job_list, int num_of_jobs);

void calculateTotalTime(struct job* running_job);
void calculateResponseTime(struct job* running_job);

void printSchedulerResults(struct job* resulting_job_list, int num_of_jobs);

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
    FIFO_Scheduler(job_list, num_of_jobs);
    SJF_Scheduler(job_list, num_of_jobs);
    BJF_Scheduler(job_list, num_of_jobs);
    STCF_Scheduler(job_list, num_of_jobs);
    RR_Scheduler(job_list, num_of_jobs);

    
    return 0;
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

void FIFO_Scheduler(struct job *job_list, int num_of_jobs)
{
    int timer = job_list[0].arrival_time;
    printf("\nFIFO Scheduler\n---------------\n");

    for(int i = 0; i < num_of_jobs; i++)
    {
        // printf("JOB #%i: job_id: %i arrival_time: %i duration: %i\n", 
        //     i + 1, job_list[i].job_id, job_list[i].arrival_time, job_list[i].duration);
        // printf("JOB %i IS RUNNING.\n", job_list[i].job_id);
        job_list[i].start_time = timer;
        timer += job_list[i].duration;
        job_list[i].finish_time = timer;
        calculateTotalTime(&job_list[i]);
        calculateResponseTime(&job_list[i]);
    }

    printSchedulerResults(job_list, num_of_jobs);
}

void SJF_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\nSJF Scheduler\n---------------\n");
}

void BJF_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\nBJF Scheduler\n---------------\n");
}

void STCF_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\nSTCF Scheduler\n---------------\n");   
}

void RR_Scheduler(struct job *job_list, int num_of_jobs)
{
    // insert code
    printf("\nRR Scheduler\n---------------\n");
}

void calculateTotalTime(struct job* running_job)
{
    running_job->total_time = running_job->finish_time - running_job->arrival_time;
}

void calculateResponseTime(struct job* running_job)
{
    running_job->response_time = running_job->start_time - running_job->arrival_time;
}

void printSchedulerResults(struct job* resulting_job_list, int num_of_jobs)
{
    int avg_total_time = 0, avg_response_time = 0;
    for(int i = 0; i < num_of_jobs; i++)
    {
        printf("JOB #%i Start Time: %i Finish Time: %i Total Time: %i Response Time: %i\n", 
            resulting_job_list[i].job_id, resulting_job_list[i].start_time, resulting_job_list[i].finish_time, resulting_job_list[i].total_time, resulting_job_list[i].response_time);

        avg_total_time += resulting_job_list[i].total_time;
        avg_response_time += resulting_job_list[i].response_time;
    }

    avg_total_time /= num_of_jobs;
    avg_response_time /= num_of_jobs;
    printf("AVG Total Time: %i AVG Response Time: %i\n", avg_total_time, avg_response_time);
}