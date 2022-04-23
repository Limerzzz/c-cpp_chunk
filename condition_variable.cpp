#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;


void printId(int id){
	std::unique_lock<std::mutex> lck(mtx);
	// prevent false awakening
	while(!ready){
		cv.wait(lck);	
	}
	std::cout<< "thread:"<< std::this_thread::get_id() << "id: "<< id << std::endl;
}

void go(){
	std::unique_lock<std::mutex> lck(mtx);
	ready = true;
	cv.notify_all();
}


int main(){
	std::thread threads[10];
	for(int i = 0; i < 10; ++i){
		threads[i] = std::thread(printId, i);	
	}

	std::cout << "create done." << std::endl;

	go();
	for(auto& t : threads){
		t.join();	
	}
	std::cout << "process done." << std::endl;
	return 0;
}
