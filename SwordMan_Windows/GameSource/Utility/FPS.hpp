/**
* @file FPS.hpp
* @brief FPSを扱うクラスです
* @author tonarinohito
* @date 2018/５/11
* 参考元 https://dixq.net/g/03_14.html
*/
#pragma once
#include <memory>
#include <DXlib.h>
#include <chrono>
class FPS final
{
private:
	class Singleton final
	{
	private:
		int startTime;         //測定開始時刻
		int count;             //カウンタ
		float fps;             //fps
		static constexpr int ave = 30;//平均を取るサンプル数
		static constexpr int FPS = 30;	//設定したFPS
	public:
		Singleton() :
			startTime(0),
			count(0),
			fps(0)
		{}

		void Update()
		{
			if (count == 0) 
			{ 
				startTime = GetNowCount();
			}
			if (count == ave) 
			{ 
				int t = GetNowCount();
				fps = 1000.f / ((t - startTime) / (float)ave);
				count = 0;
				startTime = t;
			}
			++count;
		}
		void Draw()
		{
			DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", fps);
		}
		void Wait()
		{
			int tookTime = GetNowCount() - startTime;	//かかった時間
			int waitTime = count * 1000 / FPS - tookTime;	//待つべき時間
			if (waitTime > 0) 
			{
				//usleepはμ秒
				Sleep(waitTime);	//待機
			}
		}
	};

public:
	static Singleton& Get()
	{
		static std::unique_ptr<Singleton> instance = std::make_unique<Singleton>();
		return *instance;
	}
};
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

