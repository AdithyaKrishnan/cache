//Cache size program
#include <stdio.h> 
#include <time.h>
#include <stdlib.h>

#define KB 1024 
#define MB 1024 * KB 
#define SIZE 12 * MB // size of array
#define REPS 1024 * MB // times to access memory 
#define MEAN  5 // times to repeat experiment to get mean
struct timespec start;
struct timespec endt; //timestamps

double time_elapsed(struct timespec start, struct timespec endt);

int main() 
{
	int lengthMod;
	float totalTime;
	// possible cache sizes to test for
	int sizes[] = {
		1 * KB, 4 * KB, 8 * KB, 16 * KB, 32 * KB, 64 * KB, 128 * KB, 256 * KB, 
        512 * KB, 1 * MB, 2 * MB, 3 * MB, 4 * MB, 6 * MB, 8 * MB, 10 * MB, 12 * MB
	};
	// init data 
	int *data = new int[SIZE/sizeof(int)];
	// for each possible cache size to test for
	for (int i = 0; i < sizeof(sizes)/sizeof(int); i++) 
	{
		lengthMod = sizes[i] - 1;
		// repeatedly read data
		totalTime = 0;
		for (int j = 0; j < MEAN; j++) 
		{
			clock_gettime(CLOCK_REALTIME, &start); //Start time noted
			for (unsigned int k = 0; k < REPS; k++) 
			{
				// *16 to read to new cache line as much as possible
				// & lengthMod == % sizes[i]/sizeof(int)
				data[(k * 16) & (lengthMod/sizeof(int))]++;
			}
			clock_gettime(CLOCK_REALTIME, &endt); //End time noted
			totalTime += time_elapsed(start, endt);
		}
		// where theres a spike in time --> new level of cache
		printf("%d, %1.2f \n", (sizes[i] / (1 * KB)), totalTime / MEAN);
	}
	// cleanup
	//delete[] data;
}
//Time elapsed from 'start' to 'end' in milliseconds
double time_elapsed(struct timespec start, struct timespec endt)
{
	double t;
	t = (endt.tv_sec - start.tv_sec) * 1000; //diff of tv_sec
	t += (endt.tv_nsec - start.tv_nsec) * 0.000001; //add diff of tv_nsec too
	return t;
}
