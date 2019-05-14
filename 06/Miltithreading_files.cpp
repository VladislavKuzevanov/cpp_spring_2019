#include <iostream>
#include <io.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <thread>
#include <string>

int exept = 0;

void ins_sort(const std::string& file_name, const std::string& file_out_name, int position) {
	std::ifstream instrm(file_name, std::ios::binary);
	if (!instrm.is_open()) {
		exept += 1;
		return;
	}
	instrm.seekg(position * sizeof(uint64_t), 0);
	std::vector <uint64_t> t;
	uint64_t tmp;
	size_t k = 0;
	instrm.read((char *)&tmp, sizeof(tmp));
	while (!instrm.eof() && k < 100) {
		t.push_back(tmp);
		instrm.read((char *)&tmp, sizeof(tmp));
		k++;
	}
	for (size_t i = 0; i < k; i++) {
		for (size_t j = i; j > 0 && t[j - 1] > t[j]; j--) {
			std::swap(t[j - 1], t[j]);
		}
	}
	instrm.close();
	std::ofstream outstrm(file_out_name, std::ios::binary);
	if (outstrm.is_open()) {
		for (size_t i = 0; i < k; i++) {
			outstrm.write((char*)& t[i], sizeof(t[i]));
		}
	}
	outstrm.close();
}


void merge(const std::string& f1,const std::string& f2, const std::string& file_result)
{
	uint64_t tmp1;
	uint64_t tmp2;
	std::ifstream instrm1(f1, std::ios::binary);
	if (!instrm1.is_open()) {
		exept += 1;
		std::cout << f1 + "is not open" << std::endl;
		return;
	}
	std::ifstream instrm2(f2, std::ios::binary);
	if (!instrm2.is_open()) {
		exept += 1;
		std::cout << f2 + "is not open" << std::endl;
		return;
	}
	size_t counter1 = 0;
	size_t counter2 = 0;
	instrm1.seekg(0, std::ios::end);
	int Size1 = instrm1.tellg();
	instrm1.seekg(0);
	size_t size_f1 = Size1 / sizeof(uint64_t);
	instrm2.seekg(0, std::ios::end);
	int Size2 = instrm2.tellg();
	instrm2.seekg(0);
	size_t size_f2 = Size2 / sizeof(uint64_t);
	std::string merge_result = file_result;
	std::ofstream outstrm(merge_result, std::ios::binary);
	if (!outstrm.is_open()) {
		exept += 1;
		std::cout << merge_result + "is not open" << std::endl;
		return;
	}
	instrm1.read((char *)&tmp1, sizeof(tmp1));
	instrm2.read((char *)&tmp2, sizeof(tmp2));
	while (counter1 < size_f1 && counter2 < size_f2) {
		if (tmp1 > tmp2) {
			outstrm.write((char*)& tmp2, sizeof(uint64_t));
			counter2++;
			instrm2.read((char *)&tmp2, sizeof(tmp2));
		}
		else if (tmp2 > tmp1) {
			outstrm.write((char*)& tmp1, sizeof(uint64_t));
			counter1++;
			instrm1.read((char *)&tmp1, sizeof(tmp1));
		}
		else {
			outstrm.write((char*)& tmp1, sizeof(uint64_t));
			counter1++;
			outstrm.write((char*)& tmp2, sizeof(uint64_t));
			counter2++;
			instrm1.read((char *)&tmp1, sizeof(tmp1));
			instrm2.read((char *)&tmp2, sizeof(tmp2));
		}
	}
	if (counter1 == size_f1 && counter2 != size_f2) {
		for (size_t i = 0; i < (size_f2 - counter2); i++) {
			outstrm.write((char*)& tmp2, sizeof(uint64_t));
			instrm2.read((char *)&tmp2, sizeof(tmp2));
		}
	}
	else if (counter2 == size_f2 && counter1 != size_f1) {
		for (size_t i = 0; i < (size_f1 - counter1); i++) {
			outstrm.write((char*)& tmp1, sizeof(uint64_t));
			instrm1.read((char *)&tmp1, sizeof(tmp1));
		}
	}
	instrm1.close();
	instrm2.close();
	outstrm.close();
	std::remove(f1.c_str());
	std::remove(f2.c_str());
}

int main()
{

	const size_t w = 100000; //Initial file size
	uint64_t n;
	const size_t ThreadsNumber = 2; //Number of threads
	std::string file_sort_result = "_0.txt";
	srand((unsigned)time(NULL));
	std::ofstream outstrm("file_base.txt", std::ios::binary);
	if (outstrm.is_open())
	{
		for (int i = 0; i < w; i++) {
			n = rand() % 10;
			outstrm.write((char*)& n, sizeof(n));
		}
	}
	outstrm.close();

	std::ifstream instrm("file_base.txt", std::ios::binary);
	if (!instrm.is_open()) {
		std::cout << "file_base.txt is not open" << std::endl;
		std::cout << "error have been detected" << std::endl;
		return 0;
	}
	instrm.seekg(0, std::ios::end);
	int File_Size = instrm.tellg();
	instrm.seekg(0, std::ios::end);
	instrm.close();
	instrm.seekg(0, 0);
	int Base_Size = File_Size / sizeof(uint64_t);
	std::cout << "!" << Base_Size << std::endl;

	std::cout << "Wait while sorting..." << std::endl;

	uint64_t position_tmp = 0;
	int k = 1;
	while (position_tmp < Base_Size && exept==0) {
		std::vector <std::thread> threads;
		for (size_t i = 0; i < ThreadsNumber; i++) {
			if (position_tmp < Base_Size) {
				std::thread th(ins_sort, "file_base.txt", std::to_string(k) + file_sort_result, position_tmp);
				threads.emplace_back(std::move(th));
				position_tmp = position_tmp + 100;
				k++;
			}
		}
		for (auto& t : threads) {
			t.join();
		}
	}

	if (exept == 0) {
		k = k - 1;
		std::cout << "k = " << k << std::endl;

		std::cout << "Sorting ended." << std::endl;

		std::cout << "Wait while merging..." << std::endl;
	}
	uint64_t file_index = 0;
	while (k != 1 && exept==0) {
		int index = 1;
		std::cout << "k = " << k << std::endl;
		if (k % 2 == 1) {
			std::string s1, s2, s_tmp;
			s1 = std::to_string(k - 1) + "_" + std::to_string(file_index) + ".txt";
			s2 = std::to_string(k) + "_" + std::to_string(file_index) + ".txt";
			s_tmp = std::to_string(k - 1) + "_" + std::to_string(file_index) + "a.txt";
			merge(s1, s2, s_tmp);
			std::string a = (std::to_string(k - 1) + "_" + std::to_string(file_index) + "a.txt");
			const char* b = s1.c_str();
			std::rename(a.c_str(), b);
			k = k - 1;
		}
		if (exept == 0) {
			for (int num = 0; num < k; num += ThreadsNumber * 2) {
				std::vector <std::thread> threads;
				for (int j = 0; j < ThreadsNumber; j++) {
					if (index - 1 < k / 2) {
						std::string s3, s4, s_tmp2;
						s3 = std::to_string(2 * index - 1) + "_" + std::to_string(file_index) + ".txt";
						s4 = std::to_string(2 * index) + "_" + std::to_string(file_index) + ".txt";
						s_tmp2 = std::to_string(index) + "_" + std::to_string(file_index + 1) + ".txt";
						std::thread th([s3, s4, s_tmp2, index, file_index]()->void {
							return merge(s3, s4, s_tmp2);
						});
						threads.emplace_back(std::move(th));
						index = index + 1;
					}
				}
				for (auto& t : threads) {
					t.join();
				}
			}
			k /= 2;
			file_index += 1;
		}
	}
	if (exept == 0) {
		std::cout << "Merging ended" << std::endl;

		std::string result = "RESULT.txt";
		std::string last_file = "1_" + std::to_string(file_index) + ".txt";
		std::rename(last_file.c_str(), result.c_str());
		std::cout << "done" << std::endl << "Your result in RESULT.txt";
	}
	else {
		std::cout << "Error in programm" << std::endl;
	}
	return 0;
}
