#include <iostream> 
#include <thread> 
#include <mutex> 

void print1(int& i) {
	while (i < 1000000) {
		if (i % 2 == 1)
		{
			printf("ping\n");
			i++;
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
			printf("pong\n");
			i++;
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
