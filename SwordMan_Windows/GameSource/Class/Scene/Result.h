/**
* @file Result.hpp
* @brief Resultシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
#include "../Scene/SceneManager.hpp"

namespace Scene
{
	class Result final : public AbstractScene
	{
	public:
		Result(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame);
		~Result();
		void Update() override;
		void Draw() override;
	};
}