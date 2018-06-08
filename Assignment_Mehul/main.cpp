#include <iostream>
#include "main.h"


// Class C'tor
// Which will create a shared memory
myMemory::myMemory(int key)
{
	bThrdAlive = true;
  key_ = key; 
  std::cout << "Inside myMemory c'tor" << std::endl;
	shm_id = shmget(key, 115 * sizeof(uint64_t), IPC_CREAT | 0666);
	if (shm_id < 0)
		throw "Unable to get shared memory";

	shm_ptr = (uint64_t *) shmat(shm_id, 0, 0);
	if (shm_ptr == nullptr)
		throw "Unable to get shared memory pointer";
}

// Class destructor
// It will detach shared memory and remove its entry
myMemory::~myMemory()
{
  std::cout << "Inside myMemory destrc'tor" << std::endl;
	if (shmdt(shm_ptr) != 0)
		//Detach memory
		 throw "Unable to detach memory";
	shmctl(shm_id, IPC_RMID, 0);
	//Remove share memory segment
}

// Worker Thread function
// This will work till bThrdAlive is not false
void myMemory::primaryFunc(int id, uint64_t * ptr)
{

	while (bThrdAlive) {
    //std::cout << "Inside promary thread" << std::endl;
    std::unique_lock<std::mutex> myLock(mu_);
    ptr[ 0 ] = static_cast<uint64_t>(id);
    myLock.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    myLock.lock();
    std::cout << "Data: " << *ptr << " " << static_cast<uint64_t>(id)<< std::endl;
    //std::cout << "Ending promary thread: " << id << std::endl;
	}
}

// This will start worker thread with unique id and make a entry in map
void myMemory::start_thread(const int id)
{
  std::thread thrd = std::thread(&myMemory::primaryFunc, this, id, shm_ptr);
	thrd.detach();
//tm_[id] = std::move(thrd);
	tm_[id] = thrd.native_handle();
  std::cout << "Thread created with id " << id << std::endl;
}
#if 0
void myMemory::thread_join()
{
#if 1
	for (auto itm = tm_.begin(); itm != tm_.end(); itm++) {
		//itm->second.join();
	}
#endif
  std::cout << "Thread join finish" << std::endl;
  //std::for_each(tm_.begin(), tm_.end(), std::mem_fn(&std::thread::join));
}
#endif

// This will stop all curretnly runing worker threads
// By setting bThrdAlive = false and calling thread destructors
void myMemory::stop_thread(const int id)
{
  ThreadMap::const_iterator it = tm_.find(id);
	bThrdAlive = false;
	if (it != tm_.end()) {
    //it->second.std::thread::~thread(); //thread not killed
		pthread_cancel(it->second);
		//tm_.erase(id);
    std::cout << "Thread " << id << " killed:" << std::endl;
	}
}

// This is ^C handler, which will call stop_thread once ^C recv
void myMemory::my_handler(int s)
{
	printf("Caught signal %d\n", s);
//for (std::unordered_map < int, std::thread >::iterator itm = tm_.begin(); itm != tm_.end(); itm++) {
	for (auto itm = tm_.begin(); itm != tm_.end(); ++itm) {
	  std::cout << "killing thread For: id= " << itm->first << std::endl;
		stop_thread(itm->first);
	}
//for (std::unordered_map < int, std::thread >::iterator itm = tm_.begin(); itm != tm_.end(); itm++) {
  //for (auto itm = tm_.begin(); itm != tm_.end(); ++itm) {
	//  tm_.erase(itm->first);
	//}
  tm_.clear();
	//for_each(tm_.begin(), tm_.end(), std::bind(&myMemory::my_handler, std::placeholders::_1));
	std::cout << "Exiting signal handler" << std::endl;
}


// This is call back function for signal handling
std::function < void (int)>callback_wrapper;
void callback_function(int val){
  callback_wrapper(val);
}

int main() {
  myMemory m(1234);

	callback_wrapper = std::bind(&myMemory::my_handler, &m, std::placeholders::_1);
	signal(SIGINT, callback_function);
	m.start_thread(1);
	m.start_thread(2);
	m.start_thread(3);
	m.start_thread(4);
	//std::	 thread primaryThd(primaryFunc);
	//primaryThd.join();
	
  // Let's wait till other thread completes
  std::this_thread::sleep_for(std::chrono::seconds(1));
  while(m.getAlive()) { 
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}
