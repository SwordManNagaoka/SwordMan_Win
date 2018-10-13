/**
* @file Result.hpp
* @brief Resultシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
namespace Scene
{
	class Result final : public ISceneBase
	{
	public:
		Result();
		~Result();
		void Update() override;
		void Draw() override;
		void Release() override;
	};
}