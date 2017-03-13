#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include <pthread.h>
#include <assert.h>
#include <iostream>

namespace RT {
	class Runnable{
	public:
		virtual void run()=0;
		virtual ~Runnable(){};
	};

	class Thread{
	public:
		Thread(Runnable *task);
		pthread_t get_id() const
		{
			return tid_;
		}
	private:
		pthread_t tid_;
		Runnable *task_;
		void run();
		bool spawn();
	};
}

#endif
