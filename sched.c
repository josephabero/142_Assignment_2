#include <stdio.h>
#include <stdlib.h>

int main()
{
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
	int n;

	while(fgets(line, 1000, fp) != NULL)
	{
		printf("%s", line);
		n = sscanf(line, "%i %i %i\n", &job_id, &arrival_time, &duration);
		if(n > 0)
		{
			printf("job_id: %i arrival_time: %i duration: %i\n", job_id, arrival_time, duration);
		}
	}

	fclose(fp);
	return 0;
}