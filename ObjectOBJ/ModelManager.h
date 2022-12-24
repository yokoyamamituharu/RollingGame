#pragma once
#include "Model.h"
#include <map>

class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	static ModelManager* GetIns();	
	static void Initialize();
	static void Finalize();

	static Model* GetModel(std::string name);

private:
	static void SetModelName();

private:
	static std::map<std::string, Model*> models;
	static std::list<std::string> modelname;
};