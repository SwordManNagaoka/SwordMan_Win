/**
* @file Title.hpp
* @brief Titleシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
namespace Scene
{
	class Title final : public ISceneBase
	{
	public:
		Title();
		void Update() override;
		void Draw() override;
		void Release() override;
	};
}
