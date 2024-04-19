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
	for (auto& i : _gameObjects)
	{
		i->Init();
	}
}

void Scene::Update()
{
	for (auto& i : _gameObjects)
	{
		i->Update();
	}
}

void Scene::Render()
{
	for (auto& i : _gameObjects)
	{
		i->Render();
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> object)
{
	_gameObjects.push_back(object);
}
