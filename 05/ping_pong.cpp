#include <iostream> 
#include <thread> 
#include <mutex> 

std::mutex mtx;
void print1(int& i) {
	while (i < 1000000) {
		if (i % 2 == 1)
		{
			mtx.lock();
			printf("ping\n");
			i++;
			mtx.unlock();
		}
		else {
			std::this_thread::yield();
		}
	}
}
void print2(int& i) {
	while (i <= 1000000) {
		if (i % 2 == 0)
		{
			mtx.lock();
			printf("pong\n");
			i++;
			mtx.unlock();
		}
		else {
			std::this_thread::yield();
		}
	}
}
int main()
{
	int i = 1;
	std::thread th1(print1, std::ref(i));
	std::thread th2(print2, std::ref(i));
	th1.join();
	th2.join();
	return 0;
}
