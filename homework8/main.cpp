#include <iostream>
#include "blocking_queue.h"
#include <set>
#include <vector>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex_vec;
const size_t N = 16;
std::vector<size_t> stored_values;

BlockingQueue<int> bq;

//template <typename T>
void* enqueue_test(void* arg)
{
	std::cout << "Hello from enqueue test\n";
	auto vec = *(std::vector<size_t>*)arg;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		bq.enqueue(vec[i]);
	}
	return NULL;
}

//template <typename T>
void* dequeue_test(void* arg)
{
	std::cout << "Hello from dequeue test\n";
	auto count_ptr = (size_t*)arg;
	for (size_t i=0; i < *count_ptr; ++i)
	{
		std::pair<bool, size_t> ret_val = bq.try_dequeue();
		if (!ret_val.first)
		{
			return NULL;
		}
		pthread_mutex_lock(&mutex_vec);
		stored_values.push_back(ret_val.second);
		pthread_mutex_unlock(&mutex_vec);
	}
	return NULL;
}

int main(int argc, char** argv)
{
	if (argc != 1)
	{
		std::cerr << "Wrong arguments\n";
		return -1;
	}
	pthread_mutex_init(&mutex_vec, NULL);

	std::vector<std::vector<size_t>> vecs(2, std::vector<size_t>(N/2));
	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < N/2; ++j)
		{
			vecs[i][j] = j + 1 + i*(N/2);
		}
	}

	void* deq_count = (void*)(new size_t(N/4));

	pthread_t threads[6];
	for (size_t i=0; i < 6; ++i)
	{
		if (i <= 1)
		{
			int err_code = pthread_create(&threads[i], NULL, enqueue_test, (void*)(&vecs[i]));
			if (err_code < 0)
			{
				std::cerr << "Failed to create a thread\n";
				return -1;
			}
		}
		else
		{
			int err_code = pthread_create(&threads[i], NULL, dequeue_test, deq_count);
                        if (err_code < 0)
                        {
                                std::cerr << "Failed to create a thread\n";
                                return -1;
                        }

		}
	}

	void* statuses[6];

	for (size_t i=0; i < 6; ++i)
	{
		pthread_join(threads[i], &statuses[i]);
	}

	std::set<size_t> st(stored_values.begin(), stored_values.end());
	if (st.size() == N)
		std::cout << "Okay\n";
	else
		std::cout << "Chdzec\n";

	pthread_mutex_destroy(&mutex_vec);

	return 0;
}
