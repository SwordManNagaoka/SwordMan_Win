/**
* @file Utility.hpp
* @brief 便利な処理群です
* @author tonarinohito
* @date 2018/8/29
*/
#pragma once
#include <iostream>
#include <chrono>

//デバッグビルドのみ有効にする
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif
namespace Memory
{
	template<class T>void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	template<class T>void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	template<class T>void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}

/*!
@class ProcessingTime
@brief 処理時間を計測します。
	   インスタンス化されたら計測を開始し、
	   デストラクタで計測結果をコンソールに書き出します
*/
template<class T = std::chrono::milliseconds> class ProcessingTime final
{
private:

	std::string name;
	std::chrono::system_clock::time_point start;
	bool isActive;

public:
	ProcessingTime(const std::string& name = "Process", bool start = true) :
		name(name),
		isActive(start)
	{
		if (start)
		{
			this->Restart();
		}
	}
	~ProcessingTime()
	{
		this->Stop();
	}

	//計測のリスタート
	void Restart()&
	{
		start = std::chrono::system_clock::now();
		isActive = true;
	}
	//計測を終了し出力
	void Stop()&
	{
		if (!isActive)
			return;
		const auto end = std::chrono::system_clock::now();
		const auto elapsed = std::chrono::duration_cast<T>(end - start).count();
		/*if (typeid(T) == typeid(std::chrono::nanoseconds))
		{
			DOUT << elapsed << " [nanoseconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::microseconds))
		{
			DOUT << elapsed << " [microseconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::milliseconds))
		{
			DOUT << elapsed << " [milliseconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::seconds))
		{
			DOUT << elapsed << " [seconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::minutes))
		{
			DOUT << elapsed << " [minutes]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::hours))
		{
			DOUT << elapsed << " [hours]" << std::endl;
			return;
		}*/
		isActive = false;
	}

};
