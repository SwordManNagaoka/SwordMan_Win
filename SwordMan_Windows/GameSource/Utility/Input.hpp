/**
* @file Input.hpp
* @brief 入力関連の処理をまとめます
*/
#pragma once
#include <memory>
#include <DxLib.h>
/*!
@class Input
@brief キーイベントクラスです
*/
class Input final
{
private:
	class Singleton
	{
	public:
		Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	public:
		int key[256];	//キーの入力状態を格納する
		/**
		* @brief キーの入力状態を更新します
		*/
		void Update_Key()
		{
			char tmpKey[256];
			GetHitKeyStateAll(tmpKey);	//現在のキーの状態を格納
			for (int i = 0; i < 256; ++i)
			{
				if (tmpKey[i] != 0)
				{
					++key[i];
				}
				else  //押されていなければ
				{
					key[i] = 0;
				}
			}
		}
		/**
		* @brief 木の状態を取得します
		* @param (keycode) 調べたいキーコード
		* @return 押しているフレーム数。押されていない場合0が返る
		*/
		int GetKeyFrame(int keycode)
		{
			return key[keycode];	
		}
	};
public:
	inline static Singleton& Get()
	{
		static std::unique_ptr<Singleton> inst =
			std::make_unique<Singleton>();
		return *inst;
	}
};
