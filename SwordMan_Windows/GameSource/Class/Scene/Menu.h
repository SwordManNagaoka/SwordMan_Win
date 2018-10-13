/**
* @file Menu.hpp
* @brief Menuシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
namespace Scene
{
	class Menu final : public ISceneBase
	{
	public:
		void Update() override;
		void Draw() override;
		void Release() override;
	};
}
