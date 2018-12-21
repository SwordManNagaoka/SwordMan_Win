/**
* @file Menu.hpp
* @brief Menuシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "../Scene/SceneManager.hpp"
#include "../../Class/StageCreator.hpp"
#include "../../Class/CloudCreater.hpp"
#include "../../Class/StageLoader.hpp"
#include <array>
namespace Scene
{
	class Menu final : public AbstractScene
	{
	private:
		CloudCreater cloud;
		StageLoader stageLoader;
		StageCreator stageCreator;
		ECS::Entity* cursor_R;
		ECS::Entity* cursor_L;
		ECS::Entity* stageUI[3];
		int index = 0;
		int preIndex = 0;
		int score = 0;
		void indexAdd();
		void indexSub();
	public:
		Menu(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame);

		void Finalize() override;
		void Update() override;
		void Draw() override;
	};
}
