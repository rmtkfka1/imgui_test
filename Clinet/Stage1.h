#pragma once
#include "Scene.h"
class Stage1 : public Scene
{

public:
	Stage1();
	virtual ~Stage1();

	virtual void Init();
	virtual void Update();
	virtual void Render();
};

