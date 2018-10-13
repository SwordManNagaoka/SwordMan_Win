/**
* @file   Pause.hpp
* @brief  Pauseシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
#include "../Scene/SceneManager.hpp"

namespace Scene
{
	class Pause final : public AbstractScene
	{
	public:
		Pause(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame);
		~Pause();
		void Update() override;
		void Draw() override;
	private:
	};
}