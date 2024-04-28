#include "pch.h"
#include "Stage1.h"
#include "Core.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "SceneManger.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "TestScript.h"
#include "Camera.h"
#include "Transform.h"
Stage1::Stage1()
{


}

Stage1::~Stage1()
{

}


void Stage1::Init()
{

	vector<Vertex> vec(4);
	vec[0].pos = vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = vec2(0.f, 0.f);
	vec[1].pos  =vec3(0.5f, 0.5f, 0.5f);
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
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->AddComponent(make_shared<Transform>());
	shared_ptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(vec3(0.f, 100.f, 200.f));
	transform->SetLocalScale(vec3(100.f, 100.f, 1.f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh);
	}
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\box.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetFloat(0, 0.3f);
		material->SetFloat(1, 0.4f);
		material->SetFloat(2, 0.3f);
		material->SetTexture(0, texture);
		meshRenderer->SetMaterial(material);

	}

	gameObject->AddComponent(meshRenderer);

	_gameObjects.push_back(gameObject);
#pragma endregion

#pragma region Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->AddComponent(make_shared<Transform>());
	camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45µµ
	shared_ptr<TestScript> test = make_shared<TestScript>();
	camera->AddComponent(test);
	camera->GetTransform()->SetLocalPosition(vec3(0.f, 100.f, 0.f));
	_gameObjects.push_back(camera);
#pragma endregion


	Super::Init();

}

void Stage1::Awake()
{
	Super::Awake();
}

void Stage1::Start()
{
	Super::Start();
}

void Stage1::Update()
{
	Super::Update();
}

void Stage1::LateUpdate()
{
	Super::LateUpdate();
}



