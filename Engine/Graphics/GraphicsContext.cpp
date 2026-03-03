#include <cassert>

#include "GraphicsContext.h"
#include "Core/Win32Window.h"
#include "Core/Common.h"

// RenderTargetView(렌더 타겟 뷰): 화면에 그릴 프레임(이미지) 리소스
// View(뷰): 특별한 리소스. CPU <->GPU간의 소통을 위해 사용
// -> CPU에서 리소스를 생성하면 이걸 본따서 GPU에서 복제본을 만들고 사용함
// 여기서는 포워드 렌더러(Forward Renderer) 할거임
// 요새 엔진은 디퍼드 렌더러(Deffered Renderer) 함

namespace Craft
{
	GraphicsContext* GraphicsContext::instance = nullptr;

	Craft::GraphicsContext::GraphicsContext()
	{
		assert(!instance);
		instance = this;
	}

	GraphicsContext::~GraphicsContext()
	{
		SafeRelease(device);
		SafeRelease(context);
		SafeRelease(swapChain);
		SafeRelease(renderTargetView);
		
	}

	void GraphicsContext::Initialize(const Win32Window& window)
	{
		// 멤버 변수 설정
		width = window.Width();
		height = window.Height();

		// 장치 생성
		CreateDevice();

		// SwapChain 생성
		CreateSwapChain(window);

		// 뷰포트 생성
		CreateViewport(window);

		// 렌더 타겟 뷰 생성
		CreateRenderTargetView();

		// Incomplete: 우리 엔진에서는 뷰포트를 바꿀 필요 없음
		context->RSSetViewports(1, &viewport);

	}

	void GraphicsContext::BeginScene(float red, float green, float blue)
	{
		// 그릴 이미지 준비
		// 한 색으로 이미지를 칠하기
		// 렌더링 과정은 단순하게: 빈 도화지 준비 -> 그리기 -> 모니터로 전달

		// 그릴 도화지 설정
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);

		// 빈 도화지로 만들기 -> 한 색상으로 덧칠하기
		float backgroundColor[4]{ red,green,blue,1.0f };
		context->ClearRenderTargetView(renderTargetView, backgroundColor);
	}

	void GraphicsContext::EndScene(uint32_t vsnyc)
	{
		// 모니터에 전달 (백버퍼 - 프론트버퍼 교환)
		swapChain->Present(0, 0);
	}

	GraphicsContext& GraphicsContext::Get()
	{
		assert(instance);
		return *instance;
	}

	void GraphicsContext::CreateDevice()
	{
		// 플래그 지정
		uint32_t flag{};
#if _DEBUG
		flag |= D3D10_CREATE_DEVICE_DEBUG;
#endif

		// Direct3D API 버전 정보
		// 그래픽카드가 지원하는 지 확인하는 용도
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		// 장치 생성
		// Device / DeviceContext 생성

		/*
		IDXGIAdapter* pAdapter,
		D3D_DRIVER_TYPE DriverType,
		HMODULE Software,
		UINT Flags,
		CONST D3D_FEATURE_LEVEL* pFeatureLevels,
		UINT FeatureLevels,
		UINT SDKVersion,
		_COM_Outptr_opt_ ID3D11Device** ppDevice,
		D3D_FEATURE_LEVEL* pFeatureLevel,
		ID3D11DeviceContext** ppImmediateContext
		*/

		// 오류인 경우 음수값 반환됨
		HRESULT result = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&context
		);

		// 성공 확인도 있음 SUCCEEDED(result);
		if (FAILED(result))
		{
			__debugbreak();
			return;
		}
	}

	void GraphicsContext::CreateSwapChain(const Win32Window& window)
	{
		// 스왑체인 생성해주는 객체 얻어오기
		IDXGIFactory* factory = nullptr;
		HRESULT result = CreateDXGIFactory(
			__uuidof(IDXGIFactory),
			reinterpret_cast<void**>(&factory)
		);

		// 예외처리
		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 스왑체인 생성을 위한 자료 설정
		/*
		DXGI_MODE_DESC BufferDesc;
		DXGI_SAMPLE_DESC SampleDesc;
		DXGI_USAGE BufferUsage;
		UINT BufferCount;
		HWND OutputWindow;
		BOOL Windowed;
		DXGI_SWAP_EFFECT SwapEffect;
		UINT Flags;
		*/

		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.Windowed = true;
		swapChainDesc.OutputWindow = window.Handle();
		// 화면 출력 용
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// 사용할 버퍼 수
		swapChainDesc.BufferCount = 2;
		// 안티-앨리어싱(Anti-Aliasing)에 슈퍼샘플링 수준 설정
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0; // 안한다는 것

		// 버퍼(프레임-이미지) 설정
		swapChainDesc.BufferDesc.Width = window.Width();
		swapChainDesc.BufferDesc.Height = window.Height();
		// 이미지 픽셀 포맷(32bit - unsigned - normalized)
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 버퍼 교환 효과 설정
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// 스왑체인 생성
		result = factory->CreateSwapChain(
			device,
			&swapChainDesc,
			&swapChain
		);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 팩토리 객체 해제
		SafeRelease(factory);

	}

	void GraphicsContext::CreateViewport(const Win32Window& window)
	{
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(window.Width());
		viewport.Height = static_cast<float>(window.Height());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	void GraphicsContext::CreateRenderTargetView()
	{
		// 정석적인 방법
		// 이미지 속성 구조체 설정
		// 이걸 기반으로 생성

		// 백버퍼용 렌더타겟뷰 생성
		// 스왑체인에서 기존 백버퍼 가지고 있음
		// 스왑체인에서 버퍼 불러와서 생성(똑같은 모양으로)

		// 스왑체인에서 얻어올 백버퍼 정보를 저장할 변수
		ID3D11Texture2D* backbuffer = nullptr;
		HRESULT result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)); // IID_PPV_ARGS: ID하고 받을 변수 한번에 해주는 매크로

		// 예외처리
		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// RTV 생성
		result = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
		// 예외처리
		if (FAILED(result))
		{
			// 리소스 해제
			SafeRelease(backbuffer);
			
			__debugbreak();
			return;
		}

		// 리소스 해제
		SafeRelease(backbuffer);

	}

}