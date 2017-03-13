#include "runnable.hpp"

namespace RT {

void *dispatch(void *arg)
{
	Runnable *task = (Runnable*)arg;
	assert(task);
	task->run();
	pthread_exit(0);
	return (void*)0;
}


Thread::Thread(Runnable *task)
{
	this->task_ = task;
	this->spawn();
}

bool Thread::spawn()
{
	return pthread_create(&tid_, 0, dispatch, (void*)task_);
}

}
