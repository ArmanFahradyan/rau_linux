#pragma once
#include <pthread.h>
#include <errno.h>
#include <list>

template <typename T>
class BlockingQueue
{
private:
	std::list<T> queue;
	pthread_mutex_t mutex_queue;
	pthread_cond_t cond_elem;
	bool will_be_elem = true;
public:
	BlockingQueue()
	{
		pthread_mutex_init(&mutex_queue, NULL);
		pthread_cond_init(&cond_elem, NULL);
	}

	~BlockingQueue()
	{
		pthread_cond_destroy(&cond_elem);
		pthread_mutex_destroy(&mutex_queue);
	}

	void enqueue(const T& new_val)
	{
		pthread_mutex_lock(&mutex_queue);
		queue.push_back(new_val);
		will_be_elem = true;
		pthread_cond_signal(&cond_elem);
		pthread_mutex_unlock(&mutex_queue);
	}

	//void* enqueue(void* arg)
	//{
	//	T* new_val_ptr = (T*)arg;
	//	enqueue(*new_val_ptr);
	//	return NULL;
	//}

	std::pair<bool, T> try_dequeue()
	{
		while (true)
		{
			pthread_mutex_lock(&mutex_queue);
			if (!queue.empty())
			{
				T ret_val = queue.front();
				queue.pop_front();
				pthread_mutex_unlock(&mutex_queue);
				return {true, ret_val};
			}
			pthread_cond_wait(&cond_elem, &mutex_queue);
			if (!will_be_elem)
			{
				pthread_mutex_unlock(&mutex_queue);
				return {false, T()};
			}
		}
	}

	//void* try_dequeue(void*)
	//{
	//	auto ret_val = try_dequeue();
	//	return (void*)(&ret_val);
	//}

	void release()
	{
		pthread_mutex_lock(&mutex_queue);
		will_be_elem = false;
		pthread_cond_broadcast(&cond_elem);
		pthread_mutex_unlock(&mutex_queue);
	}

	//void* release(void*)
	//{
	//	release();
	//	return NULL;
	//}
};







