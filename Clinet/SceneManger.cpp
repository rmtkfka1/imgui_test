#include "pch.h"
#include "SceneManger.h"
#include "Scene.h"
#include "Stage1.h"
#include "GameObject.h"

void SceneManger::Init()
{
	if (_scene)
		_scene->Init();
}

void SceneManger::Awake()
{
	if (_scene)
		_scene->Awake();
}

void SceneManger::Start()
{
	if (_scene)
		_scene->Start();
}

void SceneManger::Update()
{
	if (_scene)
		_scene->Update();
}

void SceneManger::LateUpdate()
{
	if (_scene)
		_scene->LateUpdate();
}




void SceneManger::ChangeScene(SceneType type)
{

	if (_sceneType == type)
		return;


	switch (type)
	{
	case SceneType::STAGE1:
		_scene = make_shared<Stage1>();
		break;
	case SceneType::STAGE2:
		break;

	default:
		break;
	}

	_sceneType = type;


}


