/**
* @file Menu.hpp
* @brief Menuシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
#include "../Scene/SceneManager.hpp"

namespace Scene
{
	class Menu final : public AbstractScene
	{
	public:
		~Menu();
		void Update() override;
		void Draw() override;
	};
}
