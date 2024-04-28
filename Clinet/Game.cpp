#include "pch.h"
#include "Game.h"
#include "Core.h"
#include "Shader.h"
#include "Mesh.h"
#include "ImguiManager.h"
#include "Texture.h"
#include "SceneManger.h"
#include "AssimpTool.h"

AssimpTool assimpTool;


void Game::Init(WindowInfo info)
{
	_info = info;

	core->Init(_info);

	SceneManger::GetInstance()->ChangeScene(SceneType::STAGE1);
	SceneManger::GetInstance()->Init();
	SceneManger::GetInstance()->Awake();
	SceneManger::GetInstance()->Start();

}

void Game::Run()
{
	core->Update();
	core->StartRender();


	SceneManger::GetInstance()->Update();
	SceneManger::GetInstance()->LateUpdate();

	
	core->EndRender();


}
