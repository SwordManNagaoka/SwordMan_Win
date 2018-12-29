//----------------------------------------------------
//!@file	PushButton.hpp
//!@brief	ボタンを押したときコンポーネントを作成します
//!@author	日比野　真聖
//!@date	2018/10/13
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../GameController/GameController.h"
#include "../Collision/Collision.hpp"
#include "../Components/BasicComponents.hpp"
#include "../Class/TouchInput.hpp"

namespace ECS
{
	class PushButton : public Component
	{
	public:
		void Initialize() override
		{
			circle = &entity->GetComponent<CircleColiider>();
		}
		void Update() override
		{
			if (isPushed && !isOnlyOne)
			{
				func(callBack);
			}
			else if (TouchInput::GetInput().GetBtnPress(0) == 1)
			{
				float radius = entity->GetComponent<CircleColiider>().radius();
				Vec2 circlePos = Vec2(entity->GetComponent<CircleColiider>().x(), entity->GetComponent<CircleColiider>().y());
				Vec2 touchPos = TouchInput::GetInput().GetTouchIDPos(0);
				if (Collision::CircleAndPoint(circlePos, radius, touchPos))
				{
					//ヒット
					isPushed = true;
					if (onlyOne) { return; }
					if (!isEventFunc) { return; }
					func(callBack);
					onlyOne = true;
				}
			}
		}
		void Draw2D() override {}
	public:
		//!ボタンを押したら登録した関数を毎フレーム呼ぶようにする
		void NotOnlyOne()
		{
			isOnlyOne = false;
		}
		void SetEventFunction(std::function<void(Scene::IOnSceneChangeCallback*)> eventFunc)
		{
			func = eventFunc;
			isEventFunc = true;
		}
		void SetSceneCallBack(Scene::IOnSceneChangeCallback* eventCall)
		{
			callBack = eventCall;
		}
	private:
		void Draw3D() override {}
	private:
		CircleColiider* circle;
		std::function<void(Scene::IOnSceneChangeCallback*)> func;
		Scene::IOnSceneChangeCallback* callBack;
		bool isPushed = false;
		bool isOnlyOne = true;
		bool onlyOne = false;
		bool isEventFunc = false;
	};

}