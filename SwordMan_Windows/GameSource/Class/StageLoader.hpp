/**
* @file StageLoader.hpp
* @brief csvファイル等からマップデータと敵データの読み込みをします。
* @author tonarinohito
* @date 2018/9/15
* @par History
- 2018/09/21 feveleK5563
-# 「マップパラメータファイル」から必要なデータを全て取得できるようにした
*/
#pragma once
#include <sstream>
#include <fstream>
#include "DXFilieRead.hpp"
#include "StageData.hpp"
#include "../ResourceManager/ResourceManager.hpp"

class StageLoader
{
private:
	StageParam	stageParam;
	StageArrayData	mapData;
	StageArrayData	enemyData;
	StageArrayData	skyData;
public:
	StageLoader() {} 
	StageLoader(const std::string& mapParamPath)
	{
		//ファイルを開く
		std::ifstream fin(mapParamPath);
		if (fin.is_open() == 0)
		{
			printfDx("Error!!!");
		}

		//各種パラメーターを読み込む
		fin >> stageParam.mapImage >> stageParam.mapDataPath >>
			stageParam.enemyConstitutionPath >>
			stageParam.skyImage >> stageParam.skyDataPath >>
			stageParam.mapWidth >> stageParam.mapHeight >>
			stageParam.chipSize >>
			stageParam.xSpeed >>
			stageParam.backImagePath[0] >>
			stageParam.backImagePath[1] >>
			stageParam.backImagePath[2] >>
			stageParam.chipImagePath >>
			stageParam.enemyDataPath[0] >>
			stageParam.enemyDataPath[1] >>
			stageParam.enemyDataPath[2] >>
			stageParam.enemyDataPath[3] >>
			stageParam.enemyDataPath[4];

		fin.close();

		//ここで使用するリソースを読み込む
		for (int i = 0; i < 1; ++i)
		{
			//背景
			std::stringstream ss;
			ss << i;
			ResourceManager::GetGraph().Load(stageParam.backImagePath[i], stageParam.skyImage + ss.str());
		}
		//マップチップ
		ResourceManager::GetGraph().Load(stageParam.chipImagePath, stageParam.mapImage);
	}
	void LoadStage(const std::string mapParamPath)
	{
		//ファイルを開く
		std::ifstream fin(mapParamPath);
		if (fin.is_open() == 0)
		{
			printfDx("Error!!!");
		}

		//各種パラメーターを読み込む
		fin >> stageParam.mapImage >> stageParam.mapDataPath >>
			stageParam.enemyConstitutionPath >>
			stageParam.skyImage >> stageParam.skyDataPath >>
			stageParam.mapWidth >> stageParam.mapHeight >>
			stageParam.chipSize >>
			stageParam.xSpeed >>
			stageParam.backImagePath[0] >>
			stageParam.backImagePath[1] >>
			stageParam.backImagePath[2] >>
			stageParam.chipImagePath >>
			stageParam.enemyDataPath[0] >>
			stageParam.enemyDataPath[1] >>
			stageParam.enemyDataPath[2] >>
			stageParam.enemyDataPath[3] >>
			stageParam.enemyDataPath[4];
		fin.close();

		//ここで使用するリソースを読み込む
		for (int i = 0; i < 1; ++i)
		{
			//背景
			std::stringstream ss;
			ss << i;
			ResourceManager::GetGraph().Load(stageParam.backImagePath[i], stageParam.skyImage + ss.str());
		}
		//マップチップ
		ResourceManager::GetGraph().Load(stageParam.chipImagePath, stageParam.mapImage);
	}
	
	//マップと敵配置の構成、敵種類データを読み込む
	void LoadStageConstitution()
	{
		mapData.resize(stageParam.mapHeight);	//1次元目
		enemyData.resize(stageParam.mapHeight);
		skyData.resize(stageParam.mapHeight);
		for (auto i(0u); i < stageParam.mapHeight; ++i)
		{
			mapData[i].resize(stageParam.mapWidth);	//2次元目
			enemyData[i].resize(stageParam.mapWidth);
			skyData[i].resize(stageParam.mapWidth);
		}

		//マップ構成ファイルを開く
		std::ifstream mapfin(stageParam.mapDataPath);
		std::ifstream enemyfin(stageParam.enemyConstitutionPath);
		std::ifstream skyfin(stageParam.skyDataPath);
		if ((mapfin.is_open() == 0) ||
			(enemyfin.is_open() == 0) ||
			(skyfin.is_open() == 0))
		{
			printfDx("Error!!!");
		}

		//チップIDの読み込み
		for (size_t y = 0; y < stageParam.mapHeight; ++y)
		{
			for (size_t x = 0; x < stageParam.mapWidth; ++x)
			{
				mapfin >> mapData[y][x];
				enemyfin >> enemyData[y][x];
				skyfin >> skyData[y][x];
			}
		}
		mapfin.close();
		enemyfin.close();
		skyfin.close();

		//敵種類データの読み込み
		for (size_t i = 0; i < stageParam.enemyDataPath.size(); ++i)
		{
			std::ifstream enemyfin(stageParam.enemyDataPath[i]);
			enemyfin >> stageParam.enemyData[i].size.x >> stageParam.enemyData[i].size.y >>
						stageParam.enemyData[i].imageName >>
						stageParam.enemyData[i].animNum >>
						stageParam.enemyData[i].changeAnimFrameTime >>
						stageParam.enemyData[i].moveSpeed >>
						stageParam.enemyData[i].jumpPower;
			enemyfin.close();
		}
	}

	const StageParam& GetStageParam()
	{
		return stageParam;
	}
	const StageArrayData& GetStageData()
	{
		return mapData;
	}
	const StageArrayData& GetEnemyData()
	{
		return enemyData;
	}
	const StageArrayData& GetSkyData()
	{
		return skyData;
	}
};

