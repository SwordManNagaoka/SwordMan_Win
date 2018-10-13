/**
* @file SceneManager.hpp
* @brief Sceneオブジェクトを管理します
* @author tonarinohito
* @date 2018/10/06
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Utility.hpp"
#include "../Scene/Scene.hpp"
#include <map>
#include <any>



namespace Scene
{
	enum class SceneName
	{
		Title,
		Game,
		Pause,
		BackToScene,	//前のシーンに戻る
	};

	//シーン変更時のコールバック
	class IOnSceneChangeCallback
	{
	public:
		IOnSceneChangeCallback() = default;
		virtual ~IOnSceneChangeCallback() = default;
		virtual void OnSceneChange(const SceneName& scene, const Parameter& parame, const bool stackClear) = 0;
	};


	class AbstractScene
	{
	public:
		AbstractScene(IOnSceneChangeCallback* sceneCallback)
		{
			callBack = sceneCallback;
		}
		virtual ~AbstractScene() = default;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	protected:
		IOnSceneChangeCallback* callBack;
	};
}