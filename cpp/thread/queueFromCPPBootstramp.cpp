#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <sstream>

static const int QUEUE_SIZE  = 4;
static const int TOTAL_ITEMS  = 10;

std::condition_variable queue_not_full;
std::condition_variable queue_not_empty;
std::mutex queue_mutex;
int queue_data[QUEUE_SIZE];
int read_position = 0;
int write_position = 0;

std::mutex push_mutex;
std::mutex pop_mutex;

int push_counter=0;
int pop_counter=0;


std::mutex print_mutex;
void print_string(std::string str)
{
    std::lock_guard<std::mutex> lock (print_mutex);
    std::cout << str << std::endl;
}

void queue_push(int thd, int item)
{
  std::unique_lock<std::mutex> lock(queue_mutex);

  // item buffer is full, just wait here.
  while(((write_position + 1) % QUEUE_SIZE) == read_position) {
    std::ostringstream ostr;
    ostr << thd << ": waiting to pop...";
    print_string(ostr.str());
    queue_not_full.wait(lock);
  }
  queue_data[write_position++] = item;
  if (write_position == QUEUE_SIZE)
    write_position = 0;
  queue_not_empty.notify_all();
  lock.unlock();
}

int queue_pop(int thd)
{
  int data;
  std::unique_lock<std::mutex> lock(queue_mutex);
  // item buffer is empty, just wait here.
  while(write_position == read_position) {
    std::ostringstream ostr;
    ostr << thd << ": waiting to push...";
    print_string(ostr.str());
    queue_not_empty.wait(lock);
  }

  data = queue_data[read_position++];
  if (read_position >= QUEUE_SIZE)
    read_position = 0;
  queue_not_full.notify_all();
  lock.unlock();
  return data;
}

void push_thread(int thd)
{
  bool ready_to_exit = false;
  while(!ready_to_exit) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
//    std::unique_lock<std::mutex> lock(push_mutex);
    if (push_counter < TOTAL_ITEMS) {
      ++push_counter;
      queue_push(thd, push_counter);
      std::ostringstream ostr;
      ostr << thd << ": push " << push_counter << ", push_counter is " << push_counter;
      print_string(ostr.str());
    } else
        ready_to_exit = true;
//    lock.unlock();
  }
}

void pop_thread(int thd)
{
    bool ready_to_exit = false;
    while(!ready_to_exit) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
//        std::unique_lock<std::mutex> lock(pop_mutex);
        if (pop_counter < TOTAL_ITEMS) {
            int item = queue_pop(thd);
            ++pop_counter;
           std::ostringstream ostr;
           ostr << thd << ": pop " << item << ", pop_counter is " << pop_counter;
           print_string(ostr.str());
        } else
          ready_to_exit = true;
//        lock.unlock();
    }
}

int main()
{
    std::thread push_thread1(push_thread,1);
    std::thread push_thread2(push_thread,2);
    std::thread push_thread3(push_thread,3);
    std::thread push_thread4(push_thread,4);

    std::thread pop_thread1(pop_thread,5);
    std::thread pop_thread2(pop_thread,6);
    std::thread pop_thread3(pop_thread,7);
    std::thread pop_thread4(pop_thread,8);

    push_thread1.join();
    push_thread2.join();
    push_thread3.join();
    push_thread4.join();

    pop_thread1.join();
    pop_thread2.join();
    pop_thread3.join();
    pop_thread4.join();
}
