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
namespace Scene
{
	class Game final : public ISceneBase
	{
	private:
		StageLoader stageLoader;
		StageCreator stageCreator;
	public:
		Game();
		void Update() override;
		void Draw() override;
		void Release() override;
	};
}
