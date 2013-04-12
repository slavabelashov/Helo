#include <iostream>
#include <stdio.h>
#include <pthread.h>
using namespace std;

int fail;
volatile int mutex=0;
int thread_amt = 333;
int repeat = 767;

int counter = 0;
void *stream_func(void* arg)
{
	int result;
	int i=0;
	while(i<repeat)
	{
		asm volatile("pusha");
		asm volatile
		(
			"lock cmpxchg %1,%3"
			: "=a" (result) : "r" (1), "0" (0), "m" (mutex) : "memory", "cc"
		);
		asm volatile("popa");

		if(!result)
		{
			i++;
			counter++;
			mutex=0;
		}
	}
}

int main()
{
	int i;
    pthread_t thread[thread_amt];

	for(i=0; i<thread_amt; i++)
	{
		fail = pthread_create(&thread[i], NULL, stream_func, NULL);
		if(fail)
		{
			cout << "Not create a thread" << i << "because" << fail << endl; 
			return 0;
		}
	}
	for(i=0; i<thread_amt; i++)
	{
		fail = pthread_join(thread[i], NULL);
		if(fail)
		{
			cout << "Not join a thread" << i << "because" << fail << endl; 
			return 0;
		}
	}
	cout << "Threads(" << thread_amt << ") * Repeats(" << repeat << ") = " << counter << " (255411)" << endl;
	return 0;
}