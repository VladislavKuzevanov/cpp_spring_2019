#include <new>
#include <iostream>
class array_1d
{
	int* mass_;
	size_t amount;
public:
	array_1d() {}
	array_1d(int number) {
		mass_ = new int[number];
		amount = number;
	}
	const int& operator[](size_t i) const
	{
		if (i >= amount)
			throw std::out_of_range("");
		return mass_[i];
	}
	int& operator[](size_t i)
	{
		if (i >= amount)
			throw std::out_of_range("");
		return mass_[i];
	}
	~array_1d() {
		delete[] mass_;
	}
};
class Matrix
{
	array_1d * mass_2d_;
	size_t rows, columns;
public:
	Matrix(size_t rows, size_t columns)
		: rows(rows), columns(columns)
	{
		mass_2d_ = static_cast<array_1d*>(operator new[](rows * sizeof(array_1d)));
		for (int i = 0; i < rows; i++)
		{
			new (mass_2d_ + i) array_1d(columns);
		}
	};
	array_1d& operator[] (size_t i)
	{
		if (i >= rows)
			throw std::out_of_range("");
		return mass_2d_[i];
	}
	const array_1d& operator[] (size_t i) const
	{
		if (i >= rows)
			throw std::out_of_range("");
		return mass_2d_[i];
	}
	int getRows() const {
		return rows;
	}
	int getColumns() const {
		return columns;
	}
	Matrix& operator*= (int x) {
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				(*this)[i][j] = (*this)[i][j] * x;
			}
		}
		return *this;
	}
	bool operator== (const Matrix& other) {
		if ((this->columns != other.columns) || (this->rows != other.rows))
			return false;
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < columns; j++) {
				if ((*this)[i][j] != (other)[i][j])
					return false;
			}
		}
		return true;
	}
	bool operator!= (const Matrix& other) {
		return !(*this == other);
	}
	~Matrix() {
		for (int i = 0; i < rows; i++)
		{
			mass_2d_[i].~array_1d();
		}
		operator delete[](mass_2d_);
	}
};
