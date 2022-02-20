#include <iostream>
#include <pthread.h>
#include <errno.h>

// mutex variable for avoiding raise conditions
pthread_mutex_t mutex_ball;
// condition variable to have an order of functions
pthread_cond_t cond_ball;
// boolean variable for order
bool ball_in_our_side = true;

// function that prints ping iff the other function (pong) has printed pong
// the only exception is the start of the match (start of the program)
void* ping(void*)
{
	// locking mutex variable
	pthread_mutex_lock(&mutex_ball);
	// we print ping when the ball is in our side that is
	// the other function has printed pong or now is the start of the match
	while (ball_in_our_side)
	{
		// hitting the ball (printing ping)
		std::cout << "ping\n";
		// the ball passes the net (now the other function must print pong)
		ball_in_our_side = false;
		// waiting for the opponent to hit the ball (to print pong)
		pthread_cond_wait(&cond_ball, &mutex_ball);
	}
	// unlocking the mutex variable
	pthread_mutex_unlock(&mutex_ball);
	return NULL;
}

void* pong(void*)
{
	while (true)
	{
		// locking mutex variable
		pthread_mutex_lock(&mutex_ball);
		// checking  whether the ball is in the other side
		if (!ball_in_our_side)
	        {
	        	// the opponent hits the ball (printing pong)
	                std::cout << "pong\n";
	                // the ball passes the net (now the other function must print ping)
	                ball_in_our_side = true;
	                // we see the ball in our side (it is time to print ping in other function)
	                pthread_cond_signal(&cond_ball);
	        }
	        // unlocking mutex variable
	        pthread_mutex_unlock(&mutex_ball);
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
	// initializing mutex variable
	pthread_mutex_init(&mutex_ball, NULL);
	// initializing cond variable
	pthread_cond_init(&cond_ball, NULL);
	// creating threads
	pthread_t thread1, thread2;
	int error_code1 = pthread_create(&thread1, NULL, ping, NULL);
	// checking for possible errors
	if (error_code1 < 0)
	{
		std::cerr << "Failed to create a thread\n";
		return -1;
	}
	int error_code2 = pthread_create(&thread2, NULL, pong, NULL);
	// checking for possible errors
        if (error_code2 < 0)
        {
                std::cerr << "Failed to create a thread\n";
                return -1;
        }
	// waiting for threads in main thread
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

	// calling destructor for cond and mutex variables
	pthread_cond_destroy(&cond_ball);
	pthread_mutex_destroy(&mutex_ball);

	return 0;
}

