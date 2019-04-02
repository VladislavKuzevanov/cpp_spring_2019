#include <iostream>
#include <stdlib.h>
#include <math.h>

class BigInt {
	int amount;
	char* data_;
	bool sign;
public:
	BigInt() {
		this->amount = 1;
		this->sign = 0;
		this->data_ = new char[amount];
		this->data_[0] = 0 + '0';
	} // <- конструктор по умолчанию 
	BigInt(const BigInt& copied)
		: data_(new char[copied.amount])
		, amount(copied.amount)
		, sign(copied.sign)
	{
		std::copy(copied.data_, copied.data_ + amount, data_);
	}
	BigInt& operator=(const BigInt& copied)
	{
		if (this == &copied)
			return *this;
		char* tmp = new char[copied.amount];
		delete[] data_;
		data_ = tmp;
		amount = copied.amount;
		sign = copied.sign;
		std::copy(copied.data_, copied.data_ + amount, data_);
		return *this;
	}
	BigInt(const int64_t& value)
	{
		int64_t tmp1 = value;
		if (value < 0) {
			sign = 1;
			tmp1 = tmp1 * (-1);
		}
		else
			sign = 0;

		int64_t tmp2 = tmp1;
		amount = 0;
		while ((tmp1 / 10) != 0)
		{
			amount += 1;
			tmp1 = tmp1 / 10;
		}
		amount += 1;
		data_ = new char[amount];
		for (size_t i = 1; i < amount + 1; i++) {
			data_[i - 1] = tmp2 % 10 + '0';
			tmp2 = (tmp2 / 10);
		}
	}
	BigInt& operator= (const int64_t& value) {
		int64_t tmp1 = value;

		if (value < 0) {
			sign = 1;
			tmp1 *= -1;
		}
		else
			sign = 0;
		int64_t tmp2 = tmp1;
		amount = 0;
		while ((tmp1 / 10) != 0)
		{
			amount += 1;
			tmp1 = tmp1 / 10;
		}
		amount += 1;
		char* tmpdata_ = new char[amount];
		delete[](data_);
		data_ = tmpdata_;
		for (size_t i = 0; i < amount; i++) {
			data_[i] = tmp2 % 10 + '0';
			tmp2 = tmp2 / 10;
		}
		return *this;
	}
	~BigInt() {} //<-деструктор
	friend std::ostream& operator<<(std::ostream &out, const BigInt &BigInt)
	{
		if (BigInt.sign)
			out << "-";
		for (int i = BigInt.amount - 1; i >= 0; i--)
			out << BigInt.data_[i];
		return out;
	}
	bool operator>(const BigInt& other) const {
		if (*this == other)
			return 0;
		if (this->sign > other.sign) return 0;
		if (this->sign < other.sign) return 1;
		if (this->amount > other.amount) return 1;
		if (this->amount < other.amount) return 0;
		for (size_t i = this->amount; i > 0; i--)
		{
			if (this->data_[i - 1] < other.data_[i - 1])
			{
				if (this->sign == 1) return 1;
				else return 0;
			}
			else if (this->data_[i - 1] > other.data_[i - 1])
			{
				if (this->sign == 1) return 0;
				else return 1;
			}
		}
	}
	bool operator<=(const BigInt& other) const {
		return !(*this > other);
	}
	bool operator<(const BigInt& other) const {
		if (*this == other)
			return 0;
		if (this->sign > other.sign) return 1;
		if (this->sign < other.sign) return 0;
		if (this->amount > other.amount) return 0;
		if (this->amount < other.amount) return 1;

		for (size_t i = this->amount; i > 0; i--)
		{
			if (this->data_[i - 1] > other.data_[i - 1])
			{
				if (this->sign) return 1;
				else return 0;
			}
			else if (this->data_[i - 1] < other.data_[i - 1])
			{
				if (this->sign) return 0;
				else return 1;
			}
		}
		return 0;
	}
	bool operator>=(const BigInt& other) const {
		return !(*this < other);
	}
	bool operator==(const BigInt& other) const
	{
		if (this == &other)
			return true;
		for (size_t i = 1; i < amount + 1; ++i) {
			if (data_[i - 1] != other.data_[i - 1])
				return false;
		}
		return true;
	}
	bool operator!=(const BigInt& other) const
	{
		return !(*this == other);
	}
	bool operator>(const int64_t& value) const
	{
		BigInt other = value;
		return this->operator>(other);
	}
	bool operator<=(const int64_t& value) const {
		BigInt other = value;
		return !(*this > other);
	}
	bool operator<(const int64_t& value) const
	{
		BigInt other = value;
		return this->operator<(other);
	}
	bool operator>=(const int64_t& value) const {
		BigInt other = value;
		return !(*this < other);
	}
	bool operator==(const int64_t& value) const
	{
		BigInt other = value;
		return this->operator==(other);
	}
	bool operator!=(const int64_t& value) const
	{
		BigInt other = value;
		return !(*this == other);
	}
	BigInt operator-() const //<-унарный минус
	{
		BigInt tmp(*this);
		tmp.sign = !tmp.sign;
		return tmp;
	}
	static void invers(char* str, size_t size) {
		for (int i = 0; i < size; i++) {
			str[i] = '9' - str[i] + '0';
		}
	}
	BigInt operator+(const BigInt& other) const {
		int maxsize = fmax(this->amount, other.amount) + 1;
		char* tmpdata1 = new char[maxsize];
		std::copy(this->data_, this->data_ + this->amount, tmpdata1);
		char* tmpdata2 = new char[maxsize];
		std::copy(other.data_, other.data_ + other.amount, tmpdata2);
		for (int i = this->amount; i < maxsize; i++)
			tmpdata1[i] = 0 + '0';
		for (int i = other.amount; i < maxsize; i++)
			tmpdata2[i] = 0 + '0';
		/*for (int i = 0; i < maxsize; i++)
			std::cout << tmpdata1[i];
		std::cout << std::endl;
		for (int i = 0; i < maxsize; i++)
			std::cout << tmpdata2[i];
		std::cout << std::endl;*/
		BigInt c;
		c.data_ = new char[maxsize];
		int i = maxsize - 1;
		int k = 0;
		while (i != -1) {
			if (tmpdata1[i] > tmpdata2[i]) {
				k = 1;
				break;
			}
			else if (tmpdata1[i] < tmpdata2[i]) {
				k = -1;
				break;
			}
			else
				i -= 1;
		}
		//std::cout << "k = " << k << std::endl;
		if ((k == 0) && (this->sign != other.sign)) {
			c.amount = 1;
			delete[] c.data_;
			c.data_ = new char[1];
			c.data_[0] = 0 + '0';
			c.sign = 0;
			return c;
		}
		else if (k == 1)
			c.sign = this->sign;
		else
			c.sign = other.sign;
		//std::cout << "c.sign=" << c.sign << std::endl;
		int ost = 0;
		if (this->sign == other.sign) {
			for (int i = 0; i < maxsize; i++) {
				ost = ost + (tmpdata1[i] - '0') + (tmpdata2[i] - '0');
				c.data_[i] = ost % 10 + '0';
				ost = ost / 10;
			}
			/*for (int i = 0; i < maxsize; i++)
				std::cout << c.data_[i];
			std::cout << std::endl;*/
			int t = 0;
			while (c.data_[maxsize - 1 - t] == '0')
				t += 1;
			c.amount = maxsize - (t);
			return c;
		}
		else {
			//std::cout << tmpdata1 << std::endl;
			//std::cout << tmpdata2 << std::endl;
			if (k == 1)
				invers(tmpdata1, this->amount);
			else
				invers(tmpdata2, other.amount);
			//std::cout << tmpdata1 << std::endl;
			//std::cout << tmpdata2 << std::endl;
			for (int i = 0; i < maxsize; i++) {
				ost = ost + (tmpdata1[i] - '0') + (tmpdata2[i] - '0');
				c.data_[i] = ost % 10 + '0';
				ost = ost / 10;
			}
			int t = 0;
			while (c.data_[maxsize - 1 - t] == '0')
				t += 1;
			invers(c.data_, maxsize - t);
			c.amount = maxsize - t;
		}
		int t = 0;
		while (c.data_[c.amount - 1 - t] == '0')
			t += 1;
		c.amount = c.amount - t;
		return c;
	}
	BigInt operator-(const BigInt& other) const {
		return this->operator+ (-(other));
	}
	BigInt operator+(const int64_t& value) const {
		BigInt tmp = value;
		return this->operator+ (tmp);
	}
	BigInt operator-(const int64_t& value) const {
		BigInt tmp = value;
		return this->operator- (-(tmp));
	}
};
