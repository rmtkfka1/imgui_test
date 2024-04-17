#include "pch.h"
#include "Core.h"
#include "RootSignature.h"
#include "ImguiManager.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "ConstantBuffer.h"
#include "TableHeap.h"
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

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(cb), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(cb), 256);

	_tableHeap = make_unique<TableHeap>();
	_tableHeap->Init(256);

	ImguiManager::GetInstance()->Init();
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
			D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ���
			D3D12_RESOURCE_STATE_RENDER_TARGET); // ���� �����



		_cmdList->SetGraphicsRootSignature(_rootSignautre->GetRootSignature().Get());
		
		/*for (auto& v : _constantBuffers)
		{
			v->Clear();
		}*/

		GetConstantBuffer(CBV_REGISTER::b1)->Clear();

		_tableHeap->Clear();

		_cmdList->SetDescriptorHeaps(1, _tableHeap->GetDescriptorHeap().GetAddressOf());

		_cmdList->ResourceBarrier(1, &barrier);


		_cmdList->RSSetViewports(1, &_viewport);
		_cmdList->RSSetScissorRects(1, &_scissorRect);
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _rtvHandle[_backBufferIndex];
		_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
		_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);


	}

}

void Core::EndRender()
{
	_cmdList->SetDescriptorHeaps(1, _imguiHeap.GetAddressOf());

	ImguiManager::GetInstance()->Render();

	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			_rtvBuffer[_backBufferIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, // ���� �����
			D3D12_RESOURCE_STATE_PRESENT); // ȭ�� ���

		_cmdList->ResourceBarrier(1, &barrier);

		_cmdList->Close();

		// Ŀ�ǵ� ����Ʈ ����

		ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
		_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

		_swapChain->Present(1, 0);


		WaitSync();

		_backBufferIndex = (_backBufferIndex + 1) % 2;

	}
}



void Core::ResourceSet()
{

	_resCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdMemory->Reset();
	_resCmdList->Reset(_resCmdMemory.Get(), nullptr);


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

	result = _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdMemory));
	result = _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdMemory.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

	_cmdList->Close();

	result=_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return result;
}

HRESULT Core::CreateSwapChain()
{

	HRESULT result=S_OK;

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(_info.width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(_info.height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = 2; // ����+�ĸ� ����
	sd.OutputWindow = _info.hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
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

void Core::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}

