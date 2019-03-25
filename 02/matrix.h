class array_1d
{
	int* mass_;
	int amount;
public:
	array_1d() {}
	void init(int amount) {
		array_1d::amount = amount;
		mass_ = new int[amount];
		for (int i = 0; i < amount; i++) {
			mass_[i] = i;
		}
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
	: public array_1d
{
	array_1d * mass_2d_;
	int rows, columns;
public:
	Matrix(int rows, int columns)
		: rows(rows), columns(columns)
	{
		mass_2d_ = new array_1d[columns];
		for (int i = 0; i < rows; i++) {
			mass_2d_[i].init(columns);
			for (int j = 0; j < columns; j++)
				mass_2d_[i][j] = i + j;
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
	int getRows() {
		return rows;
	}
	int getColumns() {
		return columns;
	}
	Matrix operator*= (const int& x) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				(*this)[i][j] = (*this)[i][j] * x;
			}
		}
		Matrix tmp(rows, columns);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				tmp[i][j] = (*this)[i][j] * x;
			}
		}
		return tmp;
	}
	bool operator== (const Matrix& other) {
		if ((this->columns != other.columns) || (this->rows != other.rows))
			return false;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if ((*this)[i][j] != (other)[i][j])
					return false;
			}
		}
		return true;
	}
	bool operator!= (const Matrix& other) {
		int k = 0;
		if ((this->columns != other.columns) || (this->rows != other.rows))
			return true;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if ((*this)[i][j] == (other)[i][j])
					k += 1;
			}
		}
		if (k > 0)
			return 0;
		else return 1;
	}
};
