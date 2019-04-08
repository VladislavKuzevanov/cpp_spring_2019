#include <string.h>
#include<iostream>
#include <sstream>

enum class Error
{
	NoError,
	IncorrectType,
	CorruptedArchive
	
};
class Serializer
{
	std::ostream &out_;
	static constexpr char Separator = ' ';

public:
	explicit Serializer(std::ostream& out)
		: out_(out) {}

	template <class T>
	Error save(T& object) {
		return object.serialize(*this);
	}
	template <class... ArgsT>
	Error operator()(ArgsT&&... args) {
		return process(std::forward<ArgsT>(args)...);
	}
private:
	template<class T>
	Error process(T value) {
		if (std::is_same<uint64_t, decltype(value)>::value) {
			out_ << value << Separator;
			return Error::NoError;
		}
		else if (std::is_same<bool, decltype(value)>::value) {
			out_ << (value ? "true" : "false") << Separator;
			return Error::NoError;
		}
		else
			return Error::IncorrectType;
	}
	template <class T, class... ArgsT>
	Error process(T value, ArgsT&&... args) {
		if (std::is_same<uint64_t, decltype(value)>::value) {
			out_ << value << Separator;
		}
		else if (std::is_same<bool, decltype(value)>::value) {
			out_ << (value ? "true" : "false") << Separator;
		}
		else
			return Error::IncorrectType;

		return process(std::forward<ArgsT>(args)...);
	}
};
class Deserializer
{
	std::istream &in_;
	static constexpr char Separator = ' ';
	static bool is_digital(const std::string &str) {
		for (auto i : str) {
			if ((i - '0' < 0) || (i - '0' > 9)) {
				return false;
			}
			return true;
		}
	}

public:
	explicit Deserializer(std::istream& in)
		: in_(in) {}

	template <class T>
	Error load(T& object) {
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args) {
		return process(std::forward<ArgsT>(args)...);
	}

private:

	template<class T>
	Error process(T &value) {
		std::string tmp;
		std::stringstream str_stream;
		uint64_t temp_val;
		if (std::is_same<uint64_t&, decltype(value)>::value) {
			in_ >> tmp;
			if (is_digital(tmp)) {
				str_stream << tmp << Separator;
				str_stream >> temp_val;
				value = temp_val;
				return Error::NoError;
			}
			else {
				return Error::CorruptedArchive;
			}
		}
		else if (std::is_same<bool&, decltype(value)>::value) {
			in_ >> tmp;
			if (tmp == "true") {
				value = true;
				return Error::NoError;
			}
			else if (tmp == "false") {
				value = false;
				return Error::NoError;
			}
			else {
				return Error::CorruptedArchive;
			}
		}
		else {
			return Error::IncorrectType;
		}
	}

	template <class T, class... ArgsT>
	Error process(T &value, ArgsT&&... args) {
		std::string tmp;
		std::stringstream str_stream;
		uint64_t temp_val;
		if (std::is_same<bool&, decltype(value)>::value) {
			//std::cout << "Bool" << std::endl;
			in_ >> tmp;
			if (tmp == "true") {
				value = true;
				return process(std::forward<ArgsT>(args)...);
			}
			else if (tmp == "false") {
				value = false;
				return process(std::forward<ArgsT>(args)...);
			}
			else {
				return Error::CorruptedArchive;
			}
		}
		else if (std::is_same<uint64_t&, decltype(value)>::value) {
			in_ >> tmp;
			if (is_digital(tmp)) {
				str_stream << tmp << Separator;
				str_stream >> temp_val;
				value = temp_val;
				return process(std::forward<ArgsT>(args)...);
			}
			else {
				return Error::CorruptedArchive;
			}
		}
		else {
			return Error::IncorrectType;
		}
		return process(std::forward<ArgsT>(args)...);
	}
};
