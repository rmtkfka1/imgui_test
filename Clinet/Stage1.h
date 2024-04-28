#pragma once
#include "Scene.h"
class Stage1 : public Scene
{
	using Super = Scene;

public:
	Stage1();
	virtual ~Stage1();


	virtual void Init();
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();


};

