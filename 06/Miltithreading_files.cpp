#include <iostream>
#include <io.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <thread>
#include <string>

/////////////////////////Cutting the initial file into small one and sort

void ins_sort(std::string file_name, std::string file_out_name, int position) {
	std::ifstream instrm(file_name, std::ios::binary);
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
	std::ofstream outstrm("c:/AMD/" + file_out_name, std::ios::binary);
	if (outstrm.is_open()) {
		for (size_t i = 0; i < k; i++) {
			outstrm.write((char*)& t[i], sizeof(t[i]));
		}
	}
	outstrm.close();
}

/////////////////////////The function of merging two files

void merge(std::string f1, std::string f2, std::string file_result)
{
	uint64_t tmp1;
	uint64_t tmp2;
	std::ifstream instrm1(f1, std::ios::binary);
	std::ifstream instrm2(f2, std::ios::binary);
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
	std::string merge_result = "c:/AMD/" + file_result;
	std::ofstream outstrm(merge_result, std::ios::binary);
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
	/////////////////////////Generating the initial file

	const size_t w = 125000; //////////////----------> Initial file size
	uint64_t n;
	const size_t ThreadsNumber = 8; //////////////----------> Number of threads
	std::string file_sort_result = "_0.txt";
	srand((unsigned)time(NULL));
	std::ofstream outstrm("c:/AMD/file_base.txt", std::ios::binary);
	if (outstrm.is_open())
	{
		for (int i = 0; i < w; i++) {
			n = rand() % 10;
			outstrm.write((char*)& n, sizeof(n));
		}
	}
	outstrm.close();
	
	/////////////////////////Finding the number of elements in the initial file.
	std::ifstream instrm("c:/AMD/file_base.txt", std::ios::binary);
	instrm.seekg(0, std::ios::end);
	int File_Size = instrm.tellg();
	instrm.seekg(0, std::ios::end);
	instrm.close();
	instrm.seekg(0, 0);
	int Base_Size = File_Size / sizeof(uint64_t);
	std::cout << "!" << Base_Size << std::endl;

	/////////////////////////Spliting a large file into smaller sorted ones

	std::cout << "Wait while sorting..." << std::endl;

	uint64_t position_tmp = 0;
	int k = 1;
	while (position_tmp < Base_Size) {
		std::vector <std::thread> threads;
		for (size_t i = 0; i < ThreadsNumber; i++) {
			if (position_tmp < Base_Size) {
				//std::cout << "!!!" << position_tmp << std::endl;
				std::thread th(ins_sort, "c:/AMD/file_base.txt", std::to_string(k) + file_sort_result, position_tmp);
				threads.emplace_back(std::move(th));
				position_tmp = position_tmp + 100;
				k++;
			}
		}
		for (auto& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}

	k = k - 1;
	std::cout << "k = " << k << std::endl;
	
	std::cout << "Sorting ended." << std::endl;

	/////////////////////////Merging sorted files

	std::cout << "Wait while merging..." << std::endl;

	uint64_t fail_index = 0;
	while (k != 1) {
		int index = 1;
		std::cout << "k = " << k << std::endl;
		if (k % 2 == 1) {
			std::string s1, s2;
			s1 = "c:/AMD/" + std::to_string(k - 1) + "_" + std::to_string(fail_index) + ".txt";
			s2 = "c:/AMD/" + std::to_string(k) + "_" + std::to_string(fail_index) + ".txt";
			merge(s1, s2, std::to_string(k - 1) + "_" + std::to_string(fail_index) + "a.txt");
			std::string a = ("c:/AMD/" + std::to_string(k - 1) + "_" + std::to_string(fail_index) + "a.txt");
			const char* b = s1.c_str();
			std::rename(a.c_str(), b);
			k = k - 1;
		}
		for (int num = 0; num < k; num += ThreadsNumber * 2) {
			std::vector <std::thread> threads;
			for (int j = 0; j < ThreadsNumber; j++) {
				if (index - 1 < k / 2) {
					std::string s3, s4;
					s3 = "c:/AMD/" + std::to_string(2 * index - 1) + "_" + std::to_string(fail_index) + ".txt";
					s4 = "c:/AMD/" + std::to_string(2 * index) + "_" + std::to_string(fail_index) + ".txt";
					std::thread th([s3, s4, index, fail_index]()->void {
						return merge(s3, s4, std::to_string(index) + "_" + std::to_string(fail_index + 1) + ".txt");
					});
					threads.emplace_back(std::move(th));
					index = index + 1;
				}
			}
			for (auto& t : threads) {
				if (t.joinable()) {
					t.join();
				}
			}
		}
		k /= 2;
		fail_index += 1;
	}

	std::cout << "Merging ended" << std::endl;

	std::string result = "c:/AMD/RESULT.txt";
	std::rename(("c:/AMD/1_"+std::to_string(fail_index) + ".txt").c_str(), result.c_str());
	std::cout << "done" << std::endl << "Your result in c:/AMD/RESULT.txt";
	return 0;
}
