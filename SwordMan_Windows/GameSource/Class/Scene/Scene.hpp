/**
* @file Scene.hpp
* @brief Sceneの基底クラスです
* @author tonarinohito
* @date 2018/10/06
*/
#pragma once
//#include <any>
#include <map>
#include <assert.h>
#include <iostream>


class Parameter
{
public:
	Parameter() = default;
	template<typename ValueType>
	void Set(const std::string& key, ValueType value)
	{
		map[key] = value;
	}
	template<typename ValueType>
	ValueType Get(const std::string& key) const
	{
		auto it = map.find(key);
		if (map.end() == it)
		{
			std::cout << "キーのパラメータが存在しません";
			assert(false);
		}
		return it->second;
		//return std::any_cast<ValueType>(it->second);
	}
private:
	std::map<std::string, int> map;
};
