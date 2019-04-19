#include <stdio.h>
#include <stdlib.h>

struct job
{
    int job_id;
    int arrival_time;
    int duration;
    int turnaround_time;
};

void FIFO_Scheduler(struct job *job_list, int num_of_jobs);

int main()
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
    struct job job_list[100];

    while(fgets(line, 1000, fp) != NULL)
    {
        n = sscanf(line, "%i %i %i\n", &job_id, &arrival_time, &duration);
        if(n > 0)
        {
            job_list[index].job_id = job_id;
            job_list[index].arrival_time = arrival_time;
            job_list[index].duration = duration;
        }
        index++;
    }

    // for(int i = 0; i < index; i++)
    // {
    //         printf("JOB #%i: job_id: %i arrival_time: %i duration: %i\n", i + 1, job_list[i].job_id, job_list[i].arrival_time, job_list[i].duration);
    // }
    FIFO_Scheduler(job_list, index);


    fclose(fp);
    return 0;
}

void FIFO_Scheduler(struct job *job_list, int num_of_jobs)
{
    int timer = 0;
    printf("FIFO Scheduler\n---------------\n");
    for(int i = 0; i < num_of_jobs; i++)
    {
        // printf("JOB #%i: job_id: %i arrival_time: %i duration: %i\n", i + 1, job_list[i].job_id, job_list[i].arrival_time, job_list[i].duration);
        
    }
}