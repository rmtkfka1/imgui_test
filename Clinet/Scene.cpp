#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
Scene::Scene()
{

}



Scene::~Scene()
{
}


void Scene::Init()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Init();
	}
}


void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}



void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}

}


void Scene::AddGameObject(shared_ptr<GameObject> object)
{
	_gameObjects.push_back(object);
}
