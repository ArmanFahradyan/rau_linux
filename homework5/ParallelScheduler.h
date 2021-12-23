#pragma once

#include <pthread.h>
#include <queue>

class ParallelScheduler
{
private:
	size_t N;
	std::vector<pthread_t> threads;
	pthread_mutex_t queue_mutex;
	pthread_cond_t cond;
	std::queue<std::pair<void (*f)(void*), void*>> func_queue;
public:
	ParallelScheduler(size_t);
	void run(void (*start_routine)(void), void*);
};
