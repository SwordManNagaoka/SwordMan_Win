/**
* @file Game.hpp
* @brief Gameシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "../../Utility/Parameter.hpp"
#include "../../Class/StageLoader.hpp"
#include "../../Class/StageCreator.hpp"
#include "../Scene/SceneManager.hpp"
#include "../../Class/CloudCreater.hpp"
namespace Scene
{
	class Game : public AbstractScene
	{
	private:
		CloudCreater cloud;
		Parameter player;
		StageLoader stageLoader;
		StageCreator stageCreator;
		bool isIntrusion = false;
	public:
		Game(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame);
		~Game();
		virtual void Update() override;
		virtual void Draw() override;
	};
}
