#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx12.h"
class Game
{
public:

	void Init(WindowInfo info);
	void Run();
	void ImguiPrint();


private:
	void WaitSync();
	HRESULT D3DCreate();


private:

	ComPtr<ID3D12Device> _device;
	ComPtr<IDXGIFactory> _dxgi;
	ComPtr<ID3D12Debug>	 _debugController;

	ComPtr<ID3D12CommandQueue> _cmdQueue;
	ComPtr<ID3D12GraphicsCommandList> _cmdList;
	ComPtr<ID3D12CommandAllocator> _cmdMemory;

	ComPtr<ID3D12DescriptorHeap> g_pd3dSrvDescHeap;

	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;


	ComPtr<IDXGISwapChain> _swapChain;
	array<ComPtr<ID3D12Resource>, 2> _rtvBuffer;
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	array< D3D12_CPU_DESCRIPTOR_HANDLE, 2> _rtvHandle;
	uint32					_backBufferIndex = 0;


private:

	D3D12_VIEWPORT _viewport;
	D3D12_RECT _scissorRect;
	WindowInfo _info;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

