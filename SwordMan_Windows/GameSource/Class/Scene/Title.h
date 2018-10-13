/**
* @file Title.hpp
* @brief Titleシーンでの動作を記述します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "Scene.hpp"
#include "../Scene/SceneManager.hpp"

namespace Scene
{
	class Title : public AbstractScene
	{
	public:
		Title(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame);
		~Title();
		virtual void Update() override;
		virtual void Draw() override;
	};

}
