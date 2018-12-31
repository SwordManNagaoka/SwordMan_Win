/**
* @file Result.hpp
* @brief Resultシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "../Scene/SceneManager.hpp"

namespace Scene
{
	class Result final : public AbstractScene
	{
	private:
		ECS::Entity* backFade = nullptr;
		ECS::Entity* rankUI;
		ECS::Entity* scoreUI;
		std::string stageName;
		int scoreData;
		bool isScoreMoveEnd = false;
		bool isMenu = false;
		bool isGame = false;
		bool isAddbtn = false;
	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame);
		~Result();
		void Update() override;
		void Draw() override;

	};
}