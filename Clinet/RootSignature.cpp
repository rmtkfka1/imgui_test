#include "pch.h"
#include "RootSignature.h"
#include "Core.h"
void RootSignature::Init()
{
	_samplerDesc= CD3DX12_STATIC_SAMPLER_DESC(0);
	////D3D12_ROOT_PARAMETER param[1];

	//	//param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	////param[0].Constants.Num32BitValues = 4;
	////param[0].Constants.ShaderRegister = 0;
	////param[0].Constants.RegisterSpace = 0;
	////param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT-1, 1), // b1~b4
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4
	};


	CD3DX12_ROOT_PARAMETER param[2];
	param[0].InitAsConstantBufferView(0); // b0
	param[1].InitAsDescriptorTable(_countof(ranges), ranges);


	D3D12_ROOT_SIGNATURE_FLAGS flag  =D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_SIGNATURE_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	desc.NumParameters = _countof(param);
	desc.pParameters = param;
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers = &_samplerDesc;
	desc.Flags = flag;



	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	core->GetDevice()->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));



	//CD3DX12_DESCRIPTOR_RANGE ranges[] =
	//{
	//	CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT-1, 1), // b0~b4
	//	CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4
	//};

	//CD3DX12_ROOT_PARAMETER param[1];
	//param[0].InitAsDescriptorTable(_countof(ranges), ranges);

	//D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	//sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	//ComPtr<ID3DBlob> blobSignature;
	//ComPtr<ID3DBlob> blobError;
	//::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	//core->GetDevice()->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
}
