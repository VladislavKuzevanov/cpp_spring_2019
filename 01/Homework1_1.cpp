#include <iostream>
#include <cmath>
#include "numbers.dat"
int is_prime(int a){
	int temp = sqrt(a);
	if (a == 2)
		return 1;
	else if (a == 1)
		return 0;
	for (int i = 2; i<=temp; i++)
	{
		if ((a % i) == 0)
       			return 0;//вернуть 0, если число не простое
	}
	return 1;//вернуть 1, если число простое
}
int Calc (const int& size, char& first_num, char& second_num,const  int a[]){
	int begin, end, k = 0, amount = 0;//begin и end - индексы последнего вхождения левой границы в массив и первое вхождение второй границы в массив
	for (int i = 0; i < size;i++)
	{
		if (a[i] == std::atoi(&first_num))
		{
			begin = i;
			k+=1;
			break;
		}
	}
	if (k == 0)
		begin = -1;
	k = 0;
	for (int i = size; i > 0; i--)
	{
		if (a[i] == std::atoi(&second_num))
		{
			end = i;
			k = 1;
			break;
		}
	}
	if (k == 0)
		end = -1;
	if (begin == -1 || end == -1)
		return 0;
	else if (begin > end)
		return 0;
	else
	{
		for (int i = begin; i <= end; i++)
		{
			if (is_prime(a[i]))
			{
				amount +=1;
				//std::cout << a[i] << " ";
			}
		}
	}
	return amount;

}
int main(int argc, char* argv[])
{
	//for (int i = 0; i < Size; i++)
	//{
		//std::cout << Data[i] << std::endl;
	//}
	for (int i = 1; i < argc; ++i)
	{
		//int v = std::atoi(argv[i]);
		//std::cout << v << " ";
        }
	//std::cout << std::endl;
	if (argc % 2 == 1){
		for (int i = 1; i+1 < argc; i = i+2)
		{
			std::cout << Calc (Size,* argv[i],* argv[i+1], Data) << std::endl;
		}
	}
	else
		std::cout << -1;
	return 0;
}


