/**
* @file Input.hpp
* @brief ���͊֘A�̏������܂Ƃ߂܂�
*/
#pragma once
#include <memory>
#include <DxLib.h>
/*!
@class Input
@brief �L�[�C�x���g�N���X�ł�
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
		int key[256];	//�L�[�̓��͏�Ԃ��i�[����
		/**
		* @brief �L�[�̓��͏�Ԃ��X�V���܂�
		*/
		void Update_Key()
		{
			char tmpKey[256];
			GetHitKeyStateAll(tmpKey);	//���݂̃L�[�̏�Ԃ��i�[
			for (int i = 0; i < 256; ++i)
			{
				if (tmpKey[i] != 0)
				{
					++key[i];
				}
				else  //������Ă��Ȃ����
				{
					key[i] = 0;
				}
			}
		}
		/**
		* @brief �؂̏�Ԃ��擾���܂�
		* @param (keycode) ���ׂ����L�[�R�[�h
		* @return �����Ă���t���[�����B������Ă��Ȃ��ꍇ0���Ԃ�
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
