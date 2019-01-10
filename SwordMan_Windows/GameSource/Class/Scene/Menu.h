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
		bool isPlay = false;
		Easing cR[2];
		Easing cL[2];
		Easing logo[2];
		Easing hiscore[2];

		void easingMove();
		void easingOutMove();
		CloudCreater cloud;
		StageLoader stageLoader;
		StageCreator stageCreator;
		ECS::Entity* number;
		ECS::Entity* cursor_R;
		ECS::Entity* cursor_L;
		ECS::Entity* scoreBoard;
		ECS::Entity* stageUI[3];
		ECS::Entity* clearUI;
		int index = 0;
		int preIndex = 0;
		int score[3]{0};
		int clearFlag[3]{ 0 };
		void indexAdd();
		void indexSub();
	public:
		Menu(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame);

		void Finalize() override;
		void Update() override;
		void Draw() override;
	};
}
