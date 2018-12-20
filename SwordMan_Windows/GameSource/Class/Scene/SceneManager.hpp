/**
* @file SceneManager.hpp
* @brief Sceneオブジェクトを管理します
* @author 日比野　真聖
* @date 2018/10/15
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Utility.hpp"
#include "../../Utility/Parameter.hpp"
#include <map>




namespace Scene
{
	enum class SceneName
	{
		Title,
		Game,
		Pause,
		Menu,
		Result,
		BackToScene,	//前のシーンに戻る
	};
	enum class SceneStack
	{
		Non,		//何もしない(自身のスタックを残す)
		OneClear,	//自身のスタックを1つクリア
		AllClear,	//全てのスタックをクリア
	};

	//シーン変更時のコールバック
	class IOnSceneChangeCallback
	{
	public:
		IOnSceneChangeCallback() = default;
		virtual ~IOnSceneChangeCallback() = default;
		virtual void OnSceneChange(const SceneName& scene, Parameter* parame, const SceneStack& stackClear) = 0;
		virtual void StackAllClear() = 0;
	};


	class AbstractScene
	{
	public:
		AbstractScene(IOnSceneChangeCallback* sceneCallback) noexcept
		{
			callBack = sceneCallback;
		}
		virtual ~AbstractScene() = default;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Finalize() {};
		IOnSceneChangeCallback& GetCallback() const noexcept
		{
			return *callBack;
		}
	private:
		IOnSceneChangeCallback* callBack;
	};
}