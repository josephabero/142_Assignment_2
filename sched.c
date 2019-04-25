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
    kDuration_SJF = 1,
    kDuration_BJF = 2
};

void getJobList(struct job job_list[], int *num_of_jobs);

void FIFO_Scheduler(struct job *job_list, int num_of_jobs);
void SJF_Scheduler(struct job *job_list, int num_of_jobs);
void BJF_Scheduler(struct job *job_list, int num_of_jobs);
void STCF_Scheduler(struct job *job_list, int num_of_jobs);
void RR_Scheduler(struct job *job_list, int num_of_jobs);

void calculateTotalTime(struct job* running_job);
void calculateResponseTime(struct job* running_job);

// int compareArrivalTime(const void * a, const void * b);
// int compareDurationTime(const void * a, const void * b);
void sortJobs(struct job job_list[], int num_of_jobs, enum Compare_t comparison);

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

    sortJobs(job_list, num_of_jobs, kDuration_BJF);

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

    int current_shortest = -1;
    int timer = job_list[0].arrival_time;
    int job_to_run = 0;


    // for(int i = 0; i < num_of_jobs; i++)
    // {
    //     job_to_run = 0;
    //     // check if following jobs have arrival times that are equal
    //     while()
    //     {
            
    //     }
    // }
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

    int timer = 0;
    int equal_arrival_check = 0;
    int run_list_index = 0;

    int running_job_index = 0;

    int run_time;

    struct job run_list[100];

    for(int i = 0; i < num_of_jobs; i++)
    {
        run_time = abs(timer - job_list[running_job_index].arrival_time);
        for(int j = 0; j < num_of_jobs; j++)
        {
            if((job_list[j].arrival_time <= timer) && (job_list[j].finish_time < 0))
            {

            }
        }
    }
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

// int compareArrivalTime(const void * a, const void * b)
// {
//     if(((struct job *)a)->arrival_time > ((struct job *)b)->arrival_time){
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

// int compareDurationTime(const void * a, const void * b)
// {
//     if(((struct job *)a)->duration > ((struct job *)b)->duration){
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

void sortJobs(struct job job_list[], int num_of_jobs, enum Compare_t comparison)
{
    struct job temp;

    for(int i = 0; i < num_of_jobs; i++)
    {
        for(int j = 0; j < num_of_jobs - i - 1; j++)
        {
            if(comparison == kArrival)
            {
                printf("Compare Arrival\n");
                if(job_list[j].arrival_time < job_list[j+1].arrival_time)
                {
                    temp = job_list[j]; 
                    job_list[j] = job_list[j+1]; 
                    job_list[j+1] = temp; 
                }
            }
            else if(comparison == kDuration_SJF)
            {
                printf("Compare Duration_SJF\n");
                if(job_list[j].duration > job_list[j+1].duration)
                {
                    printf("swap %i and %i\n", job_list[j].job_id, job_list[j+1].job_id);
                    temp = job_list[j]; 
                    job_list[j] = job_list[j+1]; 
                    job_list[j+1] = temp; 
                }
            }
            else if(comparison == kDuration_BJF)
            {
                printf("Compare Duration_SJF\n");
                if(job_list[j].duration < job_list[j+1].duration)
                {
                    printf("swap %i and %i\n", job_list[j].job_id, job_list[j+1].job_id);
                    temp = job_list[j]; 
                    job_list[j] = job_list[j+1]; 
                    job_list[j+1] = temp; 
                }
            }
        }
    }
}

// void sortJobs(struct job job_list[], int num_of_jobs, enum Compare_t comparison)
// {
//     // enum Compare_t compare;

//     // compare = kArrival;
//     if(comparison == kArrival)
//     {
//         qsort((void *)job_list, sizeof(job_list)/sizeof(job_list[0]), sizeof(job_list[0]), compareArrivalTime);
//     }
//     else if(comparison == kDuration)
//     {
//         qsort((void *)job_list, sizeof(job_list)/sizeof(job_list[0]), sizeof(job_list[0]), compareDurationTime);
//     }
    
// }

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