﻿/**
* @file FPS.hpp
* @brief FPSを扱うクラスです
* @author tonarinohito
* @date 2018/５/11
* 参考元 https://dixq.net/g/03_14.html
*/
#pragma once
#include <memory>
#include <DxLib.h>
#include <chrono>
class Timer final
{
public:
	static float GetTime()
	{
		return Get().time;
	}
	static float GetDeltaTime()
	{
		return Get().deltaTime;
	}
	static int GetFrameRate()
	{
		return Get().frameRate;
	}
	static void Update()
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		Get().deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - Get().prev).count() / 1000.0f;
		Get().prev = now;

		Get().time = std::chrono::duration_cast<std::chrono::milliseconds>(now - Get().begin).count() / 1000.0f;

		Get().frameCount++;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - Get().second).count() >= 1000)
		{
			Get().frameRate = Get().frameCount;
			Get().frameCount = 0;
			Get().second = now;
		}
	}

private:
	struct Property
	{
		float time = 0.0f;
		float deltaTime = 0.0f;
		int frameRate = 0;
		int frameCount = 0;
		std::chrono::high_resolution_clock::time_point begin;
		std::chrono::high_resolution_clock::time_point prev;
		std::chrono::high_resolution_clock::time_point second;
	};

	static Property& Get()
	{
		static std::unique_ptr<Property> prop;

		if (prop == nullptr)
		{
			prop.reset(new Property());

			Get().begin = std::chrono::high_resolution_clock::now();
			Get().prev = std::chrono::high_resolution_clock::now();
			Get().second = std::chrono::high_resolution_clock::now();
		}

		return *prop;
	}
};

