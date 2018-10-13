/**
* @file StageData.hpp
* @brief マップを形成する各種データの構造体
* @author feveleK5563
* @date 2018/9/20
*/
#pragma once
#include <string>
#include <array>
#include <vector>
#include "../../Components/ComponentDatas/EnemyData.hpp"

//ステージを構成する各種パラメータ
struct StageParam
{
	std::string		mapImage;		//マップの画像名
	std::string		mapDataPath;	//マップ上のブロック配置データへのパス

	std::string		enemyConstitutionPath;	//敵の配置データへのパス

	std::string		skyImage;		//背景(空)の画像名
	std::string		skyDataPath;	//背景(空)の配置データへのパス

	size_t	mapWidth, mapHeight;	//マップの広さ
	int		chipSize;				//マップチップ(ブロック)画像の大きさ(横幅)
	int		xSpeed;					//X方向への速度
	std::array<std::string, 3>	backImagePath;	//背景画像へのパス(3つ)
	std::string					chipImagePath;	//マップチップ画像へのパス

	std::array<std::string, 5>	enemyDataPath;	//敵の種類データへのパス
	std::array<ECS::EnemyCommonData, 5> enemyData;	//敵の種類データ
};

//マップ上のブロック配置データ
typedef std::vector<std::vector<int>> StageArrayData;