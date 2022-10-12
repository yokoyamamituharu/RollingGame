#pragma once
#include "Model.h"
class ModelManager
{
public:
	static void Initialize();
	static ModelManager* GetIns();
public:
	static Model* bulletModel;

};

