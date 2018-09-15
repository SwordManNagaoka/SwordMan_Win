/**
* @file MapLoader.hpp
* @brief csvファイル等からマップデータの読み込をします。
* @author tonarinohito
* @date 2018/9/15
*/
#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include "../ECS/ECS.hpp"
class MapLoader
{
private:
	std::vector<std::vector<int>> mapArray;
	std::vector<size_t> posX;
	std::vector<size_t> posY;
public:
	MapLoader(const std::string& path, const std::string& mapName, const Vec2& velocity ,const size_t xNum, const size_t yNum, const size_t size,
		std::function<ECS::Entity*(const char* , const Vec2& , const Vec2& , const int , const int , const int )> tileMapArcheType)
	{
		mapArray.resize(yNum);	//1次元目
		for (auto i(0u); i < yNum; ++i)
		{
			mapArray[i].resize(xNum);//2次元目
		}
		std::ifstream fin(path);
		//チップIDの読み込み
		for (auto y(0u); y < yNum; ++y)
		{
			for (auto x(0u); x < xNum; ++x)
			{
				fin >> mapArray[y][x];
			}
		}
		fin.close();

		//配置するチップの座標
		posX.resize(xNum);
		posY.resize(yNum);
		for (auto y(0u); y < yNum; ++y)
		{
			posY[y] = y * size;
		}
		for (auto x(0u); x < xNum; ++x)
		{
			posX[x] = x * size;
		}

		for (auto y(0u); y < yNum; ++y)
		{
			for (auto x(0u); x < xNum; ++x)
			{
				tileMapArcheType(mapName.c_str(), Vec2((float)posX[x], (float)posY[y]), Vec2(velocity), int(size), int(size), mapArray[y][x]);
			}
		}
	}

};