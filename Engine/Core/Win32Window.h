#pragma once
#include <Windows.h>
#include <string>
#include <stdint.h>

namespace Craft
{
	// Win32 API로 생성하는 창 클래스
	class Win32Window
	{
	public:
		Win32Window(
			uint32_t width = 1280,
			uint32_t height = 800,
			HINSTANCE instance = nullptr,
			WNDPROC messageProcedure = nullptr
		);

		~Win32Window();

		bool Initialize();


		// getter
		inline uint32_t Width() const { return width; }
		inline uint32_t Height() const { return height; }
		inline HWND Hangle() const { return handle; }

	private:
		// 창 등록할 때 사용할 클래스 이름
		std::wstring className = L"Craft Engine Class";
		// 타이틀
		std::wstring title{};

		// 크기
		uint32_t width{};
		uint32_t height{};

		// 창 객체
		HWND handle = nullptr;

		// 프로그램 포인터
		HINSTANCE instance = nullptr;

		// 메세지 처리 함수
		WNDPROC messageProcedure = nullptr;

	};



}



