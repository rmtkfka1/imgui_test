#pragma once


class RootSignature;

class Core
{
public:

	void Init(WindowInfo info);

	void Update();

	void StartRender();
	void EndRender();



public:
	const WindowInfo& GetWindowInfo() { return _info; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }
	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignautre; }
	ComPtr<ID3D12DescriptorHeap> GetImguiHeap() { return _imguiHeap; }

private:


	void WaitSync();
	HRESULT CreateDevice();
	HRESULT CreateCmdQueue();
	HRESULT CreateSwapChain();





private:

	ComPtr<ID3D12Device> _device;
	ComPtr<IDXGIFactory> _dxgi;
	ComPtr<ID3D12Debug>	 _debugController;

	ComPtr<ID3D12CommandQueue> _cmdQueue;
	ComPtr<ID3D12GraphicsCommandList> _cmdList;
	ComPtr<ID3D12CommandAllocator> _cmdMemory;

	ComPtr<ID3D12DescriptorHeap> _imguiHeap;

	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;


	ComPtr<IDXGISwapChain> _swapChain;
	array<ComPtr<ID3D12Resource>, 2> _rtvBuffer;
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	array< D3D12_CPU_DESCRIPTOR_HANDLE, 2> _rtvHandle;
	uint32					_backBufferIndex = 0;

private:

	shared_ptr<RootSignature> _rootSignautre;


private:

	D3D12_VIEWPORT _viewport;
	D3D12_RECT _scissorRect;
	WindowInfo _info;


};

