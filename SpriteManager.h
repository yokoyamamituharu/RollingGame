#pragma once
#include "Sprite.h"

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();

	static SpriteManager* GetIns();
	static void SetModelName();
	static void Initialize();
	static void Finalize();
	//static Sprite* GetSprite(std::string name) { return models[name]; }

private:
	//static std::map<std::string, Model*> models;
	//static std::list<std::string> modelname;
};

