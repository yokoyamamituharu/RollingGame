#pragma once
#include "Model.h"
#include <map>

class ModelManager
{
public:
	ModelManager() = default;
	~ModelManager() = default;

	static void StaticInitialize();
	static void Finalize();

	static Model* GetModel(std::string name);

private:
	static void SetModelName();

private:
	static std::map<std::string, Model*> models;
	static std::list<std::string> modelname;
};