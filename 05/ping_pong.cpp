#include <iostream> 
#include <thread>
const int n = 1000000;
void print1(int& i) {
	while (i < n) {
		if (i % 2 == 1)
		{
			printf("ping\n");
			i++;
		}
	}
}
void print2(int& i) {
	while (i <= n) {
		if (i % 2 == 0)
		{
			printf("pong\n");
			i++;
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
