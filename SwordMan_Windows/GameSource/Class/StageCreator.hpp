/**
* @file StageLoader.hpp
* @brief csvファイル等からマップデータと敵データの読み込みをします。
* @author feveleK5563
* @date 2018/10/4
*/

#pragma once
#include <functional>
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../Utility/Counter.hpp"
#include "StageData.hpp"
#include "../System/System.hpp"
#include "../ArcheType/Map.hpp"
#include "../ArcheType/Enemy.hpp"
#include "../ArcheType/Sky.hpp"

class StageCreator
{
private:
	StageParam stageParam;
	Counter cntTime;
	Counter cntCreatMapNum;

public:
	//マップパラメータを設定
	void SetMapParam(const StageParam& setMapParam)
	{
		stageParam = setMapParam;
		cntTime.Reset();
		cntCreatMapNum.Reset();

		cntCreatMapNum.SetEndTime(stageParam.mapWidth - 1, 0);
	}

	//平坦なマップで画面内を埋める
	void FillUpFlatMap()
	{
		int setNum = (System::SCREEN_WIDIH / stageParam.chipSize) + 1;
		int excess = (setNum * stageParam.chipSize) - System::SCREEN_WIDIH;

		int flatMap[8]{ -1, -1, -1, -1, -1, -1, 0, 1 };
		int sc = cntCreatMapNum.IsMax() ? 10 : 0;
		int flatSky[8]{ sc, sc + 11, sc + 22, sc + 33, sc + 44, sc + 55, sc + 66, sc + 77};

		for (int i = 0; i < setNum; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				Vec2 pos(float((i * stageParam.chipSize) - excess), float(j * stageParam.chipSize));
				Vec2 velocity(float(stageParam.xSpeed), 0.f);
				ECS::MapArcheType()(stageParam.mapImage.c_str(), pos, velocity, stageParam.chipSize, stageParam.chipSize, flatMap[j]);
				ECS::SkyArcheType()((stageParam.skyImage + "0").c_str(), pos, velocity, stageParam.chipSize, stageParam.chipSize, flatSky[j]);
			}
		}
	}

	//マップと敵の自動生成
	//マップ、敵データを指定しない場合はフラットな地形が生成される
	void Run(const StageArrayData* mapData = nullptr, const StageArrayData* skyData = nullptr, const StageArrayData* enemyData = nullptr)
	{
		int setChipNum = GetSetMapChipNum();
		for (int i = 0; i < setChipNum; ++i)
		{
			if (mapData == nullptr || cntCreatMapNum.IsMax())
			{
				CreateFlatMap(i);
				continue;
			}
			else
			{
				CreateMap(i, *mapData, *skyData);
			}

			if (enemyData != nullptr)
			{
				CreateEnemy(i, *enemyData);
			}
			cntCreatMapNum.Add();
		}
	}

private:
	//平坦なマップを生成する
	void CreateFlatMap(int i)
	{
		int flatMap[8]{ -1, -1, -1, -1, -1, -1, 0, 1 }; 
		int sc = cntCreatMapNum.IsMax() ? 10 : 0;
		int flatSky[8]{ sc, sc + 11, sc + 22, sc + 33, sc + 44, sc + 55, sc + 66, sc + 77 };

		for (int y = 0; y < 8; ++y)
		{
			Vec2 pos(float(System::SCREEN_WIDIH + (i * stageParam.chipSize)), float(y * stageParam.chipSize));
			Vec2 velocity(float(stageParam.xSpeed), 0.f);
			ECS::MapArcheType()(stageParam.mapImage.c_str(), pos, velocity, stageParam.chipSize, stageParam.chipSize, flatMap[y]);
			ECS::SkyArcheType()((stageParam.skyImage + "0").c_str(), pos, velocity, stageParam.chipSize, stageParam.chipSize, flatSky[y]);
		}
	}

	//マップデータを参照し、マップを生成する
	void CreateMap(int i, const StageArrayData& mapData, const StageArrayData& skyData)
	{
		int x = cntCreatMapNum.GetCurrentCount();

		for (int y = 0; y < stageParam.mapHeight; ++y)
		{
			Vec2 pos(float(System::SCREEN_WIDIH + (i * stageParam.chipSize)), float(y * stageParam.chipSize));
			Vec2 velocity(float(stageParam.xSpeed), 0.f);
			ECS::MapArcheType()(stageParam.mapImage.c_str(), pos, velocity, stageParam.chipSize, stageParam.chipSize, mapData[y][x]);
			ECS::SkyArcheType()((stageParam.skyImage + "0").c_str(), pos, velocity, stageParam.chipSize, stageParam.chipSize, skyData[y][x]);
		}
	}

	//敵のデータを参照し、敵を生成する
	void CreateEnemy(int i, const StageArrayData& enemyConstitution)
	{
		ECS::EnemyCommonArcheType enemyArcheType;
		int x = cntCreatMapNum.GetCurrentCount();

		for (int y = 0; y < stageParam.mapHeight; ++y)
		{
			int en = enemyConstitution[y][x];
			if (en < 0)
				continue;

			Vec2 pos(float(System::SCREEN_WIDIH + (i * stageParam.chipSize)), float(y * stageParam.chipSize));
			stageParam.enemyData[en].pos = pos;
			enemyArcheType(stageParam.enemyData[en], en);
		}
	}

	//時間を計測し、配置できるマップチップの数を返す
	int GetSetMapChipNum()
	{
		int nowTime = cntTime.GetCurrentCount();
		int setChip = 0;

		if (stageParam.chipSize % stageParam.xSpeed)
		{
			if (nowTime % stageParam.chipSize == 0)
				setChip = stageParam.xSpeed;
		}
		else
		{
			if (nowTime % (stageParam.chipSize / stageParam.xSpeed) == 0)
				setChip = 1;
		}

		cntTime.Add();

		return setChip;
	}
};