/**
* @file Scene.hpp
* @brief シーンのインターフェースです
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
namespace Scene
{
	class ISceneBase
	{
	public:
		ISceneBase() = default;
		virtual ~ISceneBase() = default;
		//!Entityの更新をまとめます
		virtual void Update() = 0;
		//!Entityの描画をまとめます
		virtual void Draw() = 0;
		//!リソースやエンティティの開放を行います
		virtual void Release() = 0;
	};

}
