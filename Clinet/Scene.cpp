#include "pch.h"
#include "Scene.h"

Scene::Scene()
{

}



Scene::~Scene()
{
}

void Scene::Init()
{
}

void Scene::Update()
{
}

void Scene::Render()
{
}

void Scene::AddGameObject(shared_ptr<GameObject> object)
{
	_gameObjects.push_back(object);
}
