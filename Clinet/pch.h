#pragma once
// std::byte 사용하지 않음
#define _HAS_STD_BYTE 0


#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
#include "SimpleMath.h"
#include "SimpleMath.inl"

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>



// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")

#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
#endif

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using vec2 = DirectX::SimpleMath::Vector2;
using vec3 = DirectX::SimpleMath::Vector3;
using vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Color = DirectX::XMFLOAT4;

#include "KeyManager.h"
#include "TimeManager.h"
#include "Helper.h"
struct WindowInfo
{
	int width;
	int height;
	HWND hwnd;

};

struct Vertex
{
	vec3 pos;
	vec4 color;
	vec2 uv;
};

struct TransformParams
{
	Matrix matWVP;
};

struct VertexTextureNormalTangentBlendData
{
	vec3 position = { 0, 0, 0 };
	vec2 uv = { 0, 0 };
	vec3 normal = { 0, 0, 0 };
	vec3 tangent = { 0, 0, 0 };
	vec4 blendIndices = { 0, 0, 0, 0 };
	vec4 blendWeights = { 0, 0, 0, 0 };
};

extern unique_ptr<class Core> core;
enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
};