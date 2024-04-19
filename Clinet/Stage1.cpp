#include "pch.h"
#include "Stage1.h"
#include "Core.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "SceneManger.h"
#include "GameObject.h"


Stage1::Stage1()
{


}

Stage1::~Stage1()
{

}

void Stage1::Init()
{
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();

		vector<Vertex> vec(4);

		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		shared_ptr<Texture> texture = make_shared<Texture>();

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

		mesh->Init(vec, indexVec);
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\box.jpg");


		gameObject->Init();
		gameObject->SetMesh(mesh);
		gameObject->SetTexture(texture);
		gameObject->SetShader(shader);

		AddGameObject(gameObject);
	}

}

void Stage1::Update()
{
	for (auto& i : _gameObjects)
	{
		i->Update();
	}
}

void Stage1::Render()
{


	for (auto& i : _gameObjects)
	{
		i->Render();
	}
}
