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

