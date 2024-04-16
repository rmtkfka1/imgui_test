#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);
	void PushTable(uint32 regi);
	ComPtr<ID3D12DescriptorHeap> GetTabledescHeap() { return _tableHeap; }

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();
	void CreateView();
	void CreateTableHeap(uint32 count);


private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE* _mappedBuffer = nullptr;
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	uint32					_currentIndex = 0;

	///////////////////////////////////////////////////
	ComPtr<ID3D12DescriptorHeap> _cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _cbvHeapBegin{};
	D3D12_CPU_DESCRIPTOR_HANDLE _cbvCurrentHandle{};
	uint32 _handleIncrementSize = 0;

	///////////////////////////////////////////////////

	ComPtr<ID3D12DescriptorHeap> _tableHeap;
	uint64					_groupSize = 0;
	uint64					_groupCount = 0;
	uint32					_currentGroupIndex = 0;

};

