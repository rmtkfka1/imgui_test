#pragma once


class RootSignature;
class ConstantBuffer;
class TableHeap;
#include "ConstantBuffer.h"
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
	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
	shared_ptr<TableHeap> GetTableHeap() { return _tableHeap; }
	ComPtr<ID3D12DescriptorHeap> GetImguiHeap() { return _imguiHeap; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return	_resCmdList; }

	void ResourceSet();

private:


	void WaitSync();
	HRESULT CreateDevice();
	HRESULT CreateCmdQueue();
	HRESULT CreateSwapChain();
	HRESULT CreateDepthBuffer();

	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);



private:

	ComPtr<ID3D12Device> _device;
	ComPtr<IDXGIFactory> _dxgi;
	ComPtr<ID3D12Debug>	 _debugController;

	ComPtr<ID3D12CommandQueue> _cmdQueue;

	ComPtr<ID3D12GraphicsCommandList> _cmdList;
	ComPtr<ID3D12CommandAllocator> _cmdMemory;

	ComPtr<ID3D12CommandAllocator>		_resCmdMemory;
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;


	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	//ImGUI
	ComPtr<ID3D12DescriptorHeap> _imguiHeap;

	//SwapChain
	ComPtr<IDXGISwapChain> _swapChain;
	array<ComPtr<ID3D12Resource>, 2> _rtvBuffer;
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	array< D3D12_CPU_DESCRIPTOR_HANDLE, 2> _rtvHandle;
	uint32					_backBufferIndex = 0;

	//depthBuffer
	ComPtr<ID3D12Resource> _depthBuffer;
	ComPtr<ID3D12DescriptorHeap> _depthHeap;



private:
	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
	shared_ptr<RootSignature> _rootSignautre;
	shared_ptr<TableHeap> _tableHeap;

private:

	D3D12_VIEWPORT _viewport;
	D3D12_RECT _scissorRect;
	WindowInfo _info;


};

