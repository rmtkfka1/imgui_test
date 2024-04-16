#include "pch.h"
#include "RootSignature.h"
#include "Core.h"
void RootSignature::Init()
{

	//D3D12_ROOT_PARAMETER param[1];

		//param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	//param[0].Constants.Num32BitValues = 4;
	//param[0].Constants.ShaderRegister = 0;
	//param[0].Constants.RegisterSpace = 0;
	//param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	CD3DX12_DESCRIPTOR_RANGE range[]
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,3,0), // b0 b1 b2
	};


	CD3DX12_ROOT_PARAMETER param[3];

	param[0].InitAsDescriptorTable(_countof(range), range);
	param[1].InitAsConstantBufferView(4);
	param[2].InitAsConstants(4, 3);



	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	desc.NumParameters = _countof(param);
	desc.pParameters = param;
	desc.NumStaticSamplers = 0;
	desc.pStaticSamplers = nullptr;
	desc.Flags = d3dRootSignatureFlags;



	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);

	core->GetDevice();
	core->GetDevice()->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));

}
