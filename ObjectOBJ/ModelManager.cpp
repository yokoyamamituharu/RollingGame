#include "ModelManager.h"
#include "safe_delete.h"

std::map<std::string, Model*> ModelManager::models;
std::list<std::string>ModelManager::modelname;

void ModelManager::SetModelName()
{
	modelname.push_back("player");
	modelname.push_back("playerSphere");
	modelname.push_back("enemy");
	modelname.push_back("strongEnemy");
	modelname.push_back("bullet");
	modelname.push_back("castle");
	modelname.push_back("kabe");
	modelname.push_back("defenseTower");
	modelname.push_back("IntenQ");
	modelname.push_back("tenQ");
	modelname.push_back("battlegrund");
	modelname.push_back("ground");
	modelname.push_back("suana");
	modelname.push_back("shadow");
	modelname.push_back("area");
	modelname.push_back("wave");
	modelname.push_back("particle");
	modelname.push_back("tamesiObj");
	modelname.push_back("level");
	modelname.push_back("yowaienemy");
	modelname.push_back("cloudBurst");
	modelname.push_back("title");

	modelname.push_back("effect_1");
	modelname.push_back("effect_2");
	modelname.push_back("effect_3");
	modelname.push_back("effect_4");
}

void ModelManager::Initialize()
{
	SetModelName();

	for (std::string& name : modelname) {
		models[name] = Model::Create(name);
	}
}

void ModelManager::Finalize()
{
	for (std::string& name : modelname) {
		safe_delete(models[name]);
	}
}

Model* ModelManager::GetModel(std::string name)
{
	if (models[name] == nullptr) {
		assert(0);
	}
	return models[name];
}

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}

