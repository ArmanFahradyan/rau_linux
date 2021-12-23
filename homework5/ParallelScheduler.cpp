#include <pthread.h>
#include <vector>

ParallelScheduler::ParallelScheduler(size_t thread_number)
	: ParallelScheduler::N(thread_number)
	, ParallelScheduler::threads(std::vector<pthread_t>(thread_number))
{
}

ParallelScheduler::run(void (*start_routine)(void*))
{
	int lock_id = pthread_mutex_lock(&ParallelScheduler::queue_mutex);
	if (lock_id != 0)
	{
		exit(lock_id);
	}
	ParallelScheduler::func_queue.push({start_routine, arg});
	int unlock_id = pthread_mutex_unlock(&ParallelScheduler::queue_mutex);
	if (unlock_id != 0)
	{
		exit(unlock_id);
	}
}
