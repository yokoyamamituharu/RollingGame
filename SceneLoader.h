#pragma once
#include<string>
#include <DirectXMath.h>
#include <vector>
#include "ObjectOBJ.h"
#include <fstream>
#include <json.hpp>

class SceneLoader
{
public:	//サブクラス
	struct LevelData {
		//オブジェクト1個分のデータ
		struct ObjectData {
			//ファイル名
			std::string fileName;
			DirectX::XMVECTOR translation;
			DirectX::XMVECTOR rotation;
			DirectX::XMVECTOR scaling;
			DirectX::XMVECTOR colliderTransla;
			DirectX::XMVECTOR colliderScaling;
		};

		//オブジェクトのコンテナ
		std::vector<ObjectData> objects;
	};

public:
	void Initialize();

	void ScanningObjects(nlohmann::json& transform);

	void Update();

	void Draw();
private:

	LevelData* levelData;

	//オブジェクトデータ
	std::vector<ObjectObj*>objects;
};

