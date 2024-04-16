#include "pch.h"
#include "Core.h"
#include "RootSignature.h"
#include "ImguiManager.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "ConstantBuffer.h"
void Core::Init(WindowInfo info)
{
	_info = info;

	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	RECT rect = { 0, 0, info.width, info.height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(info.hwnd, 0, 100, 100, info.width, info.height, 0);

	CreateDevice();
	CreateCmdQueue();
	CreateSwapChain();


	_rootSignautre = make_unique<RootSignature>();
	_rootSignautre->Init();

	_constantBuffer = make_unique<ConstantBuffer>();
	_constantBuffer->Init(256,10);

	//ImguiManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init(info.hwnd);
	TimeManager::GetInstance()->Init();



}

void Core::Update()
{
	KeyManager::GetInstance()->Update();
	TimeManager::GetInstance()->Update();
}


void Core::StartRender()
{


	{
		_cmdMemory->Reset();
		_cmdList->Reset(_cmdMemory.Get(), nullptr);

		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			_rtvBuffer[_backBufferIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
			D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물
		_cmdList->ResourceBarrier(1, &barrier);


		_cmdList->SetGraphicsRootSignature(_rootSignautre->GetRootSignature().Get());
		_cmdList->SetDescriptorHeaps(1, core->GetConstantBuffer()->GetTabledescHeap().GetAddressOf());

		_constantBuffer->Clear();

		_cmdList->RSSetViewports(1, &_viewport);
		_cmdList->RSSetScissorRects(1, &_scissorRect);
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _rtvHandle[_backBufferIndex];
		_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
		_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);


	}

}

void Core::EndRender()
{
	/*ImguiManager::GetInstance()->Render();*/

	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			_rtvBuffer[_backBufferIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
			D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

		_cmdList->ResourceBarrier(1, &barrier);

		_cmdList->Close();

		// 커맨드 리스트 수행

		ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
		_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

		_swapChain->Present(1, 0);


		WaitSync();

		_backBufferIndex = (_backBufferIndex + 1) % 2;

	}
}



void Core::WaitSync()
{

	
	_fenceValue++;
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		::WaitForSingleObject(_fenceEvent, INFINITE);
	}


}



HRESULT Core::CreateDevice()
{
	HRESULT result = S_OK;

#ifdef _DEBUG
	result=::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif

	result=::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));
	result=::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));

	return result;
}

HRESULT Core::CreateCmdQueue()
{
	HRESULT result = S_OK;

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	result=_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&_cmdQueue));
	result=_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdMemory));
	result=_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdMemory.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	_cmdList->Close();

	result=_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return result;
}

HRESULT Core::CreateSwapChain()
{

	HRESULT result=S_OK;

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(_info.width); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<uint32>(_info.height); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = 2; // 전면+후면 버퍼
	sd.OutputWindow = _info.hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result=_dxgi->CreateSwapChain(_cmdQueue.Get(), &sd, &_swapChain);



	D3D12_DESCRIPTOR_HEAP_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	desc.NumDescriptors = 2;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	result=_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_rtvHeap));

	int32 rtvHeapSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int32 i = 0; i < 2; i++)
	{
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		_device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);

	}

	{

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_imguiHeap));

	}
	return result;

}

