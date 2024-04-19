#include "pch.h"
#include "Game.h"
#include "Core.h"
#include "Shader.h"
#include "Mesh.h"
#include "ImguiManager.h"
#include "Texture.h"
unique_ptr<Shader> _shader = make_unique<Shader>();
unique_ptr<Mesh> _mesh = make_unique<Mesh>();
shared_ptr<Texture> _texture = make_shared<Texture>();

void Game::Init(WindowInfo info)
{
	_info = info;

	core->Init(_info);


	vector<Vertex> vec(4);

	vec[0].pos = vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = vec2(0.f, 0.f);
	vec[1].pos = vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = vec2(1.f, 0.f);
	vec[2].pos = vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = vec2(1.f, 1.f);
	vec[3].pos = vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = vec2(0.f, 1.f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	_mesh->Init(vec, indexVec);
	_shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	_texture->Init(L"..\\Resources\\Texture\\box.jpg");



}

void Game::Run()
{
	core->Update();

	core->StartRender();

	
	_shader->SetPipelineState();
	_mesh->SetTexture(_texture);
	{
	
		_mesh->Render();

	}

	core->EndRender();
}
