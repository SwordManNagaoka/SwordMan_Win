#pragma once
#include <string>
#include <stdio.h>


//struct Converter
//{
//	//long double型からstring型へ変換
//	static const std::string ToString(const long double value)
//	{
//		std::string result;
//		result.resize(printf("%Lf", value) + 1);
//		int length = sprintf(&result[0],"%Lf",value);
//		if (length < 0)
//		{
//			//std::__throw_runtime_error("ToString");
//		}
//		result.resize(static_cast<std::size_t>(length));
//		return result;
//	}
//	//double型からstring型へ変換
//	static const std::string ToString(const double value)
//	{
//		return ToString(static_cast<long double>(value));
//	}
//	//float型からsting型へ変換
//	static const std::string ToString(const float value)
//	{
//		return ToString(static_cast<long double>(value));
//	}
//	//unsigned long long型からstring型へ変換
//	static const std::string ToString(const unsigned long long value)
//	{
//		std::string result;
//		result.resize(21); // strlen("-9223372036854775808") + 1;
//		int length = sprintf(&result[0], "%llu", value);
//		if (length < 0)
//		{
//			//std::__throw_runtime_error("ToString");
//		}
//		result.resize(static_cast<std::size_t>(length));
//		return result;
//	}
//	//long long型からstring型へ変換
//	static const std::string ToString(const long long value)
//	{
//		std::string result;
//		result.resize(21); // strlen("-9223372036854775808") + 1;
//		int length = sprintf(&result[0], "%lld", value);
//		if (length < 0)
//		{
//			//std::__throw_runtime_error("ToString");
//		}
//		result.resize(static_cast<std::size_t>(length));
//		return result;
//	}
//	//unsigned long型からstring型へ変換
//	static const std::string ToString(const unsigned long value)
//	{
//		std::string result;
//		result.resize(11); // strlen("4294967295") + 1;
//		int length = sprintf(&result[0], "%lu", value);
//		if (length < 0)
//		{
//			//std::__throw_runtime_error("ToString");
//		}
//		result.resize(static_cast<std::size_t>(length));
//		return result;
//	}
//	//long型からstring型へ変換
//	static const std::string ToString(long value)
//	{
//		std::string result;
//		result.resize(12); // strlen("-2147483648") + 1;
//		int length = sprintf(&result[0], "%ld", value);
//		if (length < 0)
//		{
//			//std::__throw_runtime_error("ToString");
//		}
//		result.resize(static_cast<std::size_t>(length));
//		return result;
//	}
//	//unsigned型からstring型へ変換
//	static const std::string ToString(unsigned value)
//	{
//		return ToString(static_cast<unsigned long>(value));
//	}
//	//int型からstring型へ変換
//	static const std::string ToString(const int value)
//	{
//		return ToString(static_cast<long>(value));
//	}
//};