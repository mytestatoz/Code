#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <thread>
#include <signal.h>
#include <unordered_map>
#include <iterator>
#include <functional>
#include <mutex>
#include <pthread.h>
#include <memory>
class myMemory {
  public:
    myMemory(int key);
    ~myMemory();
    void primaryFunc(int, uint64_t *);
    void start_thread(const int);
    void stop_thread(const int);
    void my_handler(int);
    bool getAlive() { 
         std::this_thread::sleep_for(std::chrono::seconds(1));
         return bThrdAlive;
    }
  private:
    int shm_id;
    key_t key_;
    uint64_t *shm_ptr;
    //typedef std::unordered_map<int, std::thread> ThreadMap;
    typedef std::unordered_map<int, pthread_t> ThreadMap;
    ThreadMap tm_;
    bool bThrdAlive;
    std::mutex mu_;
};
