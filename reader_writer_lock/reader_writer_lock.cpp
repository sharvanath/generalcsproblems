#include<atomic>
#include<iostream>
#include<list>
#include<mutex>
#include<condition_variable>
#include<assert.h>
#include<thread>

enum Mode {
 READ,
 WRITE
};

class ReaderWriterLock {
 public:
  int lock(Mode mode) {
    std::unique_lock<std::mutex> lck(mu_);
    if (mode == READ) {
      if (!(curr_mode_ == READ && waiting_writers_ == 0)) {
        waiting_readers_++;
        readers_cv_.wait(lck, [this](){return num_writers_ == 0;});
        waiting_readers_--;
      }
      curr_mode_ = READ;
      num_readers_++;
    } else if (mode == WRITE) {
      waiting_writers_++;
      writers_cv_.wait(lck, [this](){return num_readers_ == 0 and num_writers_ == 0;});
      waiting_writers_--;
      num_writers_++;
      curr_mode_ = WRITE;
    } else {
      return -1;
    }
    return 0;
  }

  // Fix this, get rid of arg
  int unlock(Mode mode) {
    std::unique_lock<std::mutex> lck(mu_);
    if (mode == READ) {
      num_readers_--;
      if (num_readers_ == 0) {
        writers_cv_.notify_all();
      }
    } else if (mode == WRITE) {
      num_writers_--;
      if (num_writers_ == 0) {
        writers_cv_.notify_all();
        readers_cv_.notify_all();
      }
    } else {
      return -1;
    }
    return 0;
  }

  private:
   std::mutex mu_;
   std::condition_variable readers_cv_, writers_cv_;
   Mode curr_mode_ = READ;
   int waiting_readers_ = 0, waiting_writers_ = 0;
   int num_writers_ = 0, num_readers_ = 0;
};

int main() {
  ReaderWriterLock x;
  std::atomic<int> y(0), z(0);

  std::thread t1([&]{x.lock(READ); assert(z==0); y++; std::cout<<"here1\n"; y--; x.unlock(READ);});
  std::thread t2([&]{x.lock(READ); assert(z==0); y++; std::cout<<"here2\n"; y--; x.unlock(READ);});
  std::thread t4([&]{x.lock(WRITE); assert(z==0 && y==0); z++; std::cout<<"here4\n"; z--; x.unlock(WRITE);});
  std::thread t5([&]{x.lock(WRITE); assert(z==0 && y==0); z++; std::cout<<"here4\n"; z--; x.unlock(WRITE);});
  std::thread t3([&]{x.lock(READ); assert(z==0); y++; std::cout<<"here3\n"; y--; x.unlock(READ);});
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  return 0;
}
