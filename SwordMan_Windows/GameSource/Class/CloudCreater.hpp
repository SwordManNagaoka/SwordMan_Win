/**
* @file Cloud.hpp
* @brief 背景の雲の原型を生成し続けますり
* @author tonarinohito
* @date 2018/10/16
*/
#pragma once
#include "../../ArcheType/Cloud.hpp"
#include "../../Utility/Counter.hpp"

//!背景の雲を一定間隔で生成し続けます
class CloudCreater final
{
private:
	Counter cnt;
	Random rand;
public:
	CloudCreater() :
		cnt(0, 1,0,270)
	{}
	void Run()
	{
		++cnt;
		if (cnt.IsMax())
		{
			ECS::Cloud()("cloud");
			cnt.Reset();
		}
	}
};