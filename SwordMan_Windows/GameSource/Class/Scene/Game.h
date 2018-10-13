/**
* @file Game.hpp
* @brief Gameシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
#include "../../Class/StageLoader.hpp"
#include "../../Class/StageCreator.hpp"
#include "../Scene/Scene.hpp"
#include "../Scene/SceneManager.hpp"

namespace Scene
{
	class Game : public AbstractScene
	{
	private:
		Parameter player;
		StageLoader stageLoader;
		StageCreator stageCreator;
	public:
		Game(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame);
		~Game();
		virtual void Update() override;
		virtual void Draw() override;
	};
}
