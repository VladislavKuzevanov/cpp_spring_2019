#include <iostream> 
#include <thread>
#include <atomic>
const int n = 1000000;
std::atomic<int> i (1);
void print1() {
	while (i < n) {
		if (i % 2 == 1)
		{
			printf("ping\n");
			i+=1;
		}
	}
}
void print2() {
	
	while (i < n) {
		if (i % 2 == 0)
		{
			printf("pong\n");
			i+=1;
		}
	}
}
int main()
{
	std::thread th1(print1);
	std::thread th2(print2);
	th1.join();
	th2.join();
	return 0;
}
