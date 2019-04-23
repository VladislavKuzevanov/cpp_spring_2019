#include <iostream> 
#include <thread>
#include <condition_variable>
const int n = 500000;
int amount_in_process = 1;
std::mutex mtx;
std::condition_variable c_v;

void print1() {
	for (int i = 0; i < n; i++) {
		std::unique_lock<std::mutex> lock(mtx);
		c_v.wait(lock, []() {return amount_in_process > 0; });
		printf("ping\n");
		--amount_in_process;
		c_v.notify_one();
	}
}
void print2() {
	for (int i = 0; i < n; i++) {
		std::unique_lock<std::mutex> lock(mtx);
		c_v.wait(lock, []() {return amount_in_process == 0; });
		printf("pong\n");
		++amount_in_process;
		c_v.notify_one();
	}
}
int main() {
	std::thread th1(print1);
	std::thread th2(print2);
	th1.join();
	th2.join();
	return 0;
}
