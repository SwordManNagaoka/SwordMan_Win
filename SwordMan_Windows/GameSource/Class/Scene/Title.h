/**
* @file Title.hpp
* @brief Titleシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "../Scene/SceneManager.hpp"
#include "../../ArcheType/Player.hpp"
#include "../../Class/StageLoader.hpp"
#include "../../Class/StageCreator.hpp"
#include "../../Class/CloudCreater.hpp"

namespace Scene
{
	class Title : public AbstractScene
	{
	public:
		Title(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame);
		~Title();
		virtual void Update() override;
		virtual void Draw() override;
	private:
		bool isFadeEnd = false;
		bool isPlayGame = false;
		ECS::Entity* tapStartLogo;
		ECS::Entity* title;
		ECS::Entity* fade;
		CloudCreater cloud;
		StageLoader stageLoader;
		StageCreator stageCreator;
		Easing startEase;
		Easing titleEase;
	};

}
