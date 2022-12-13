#pragma once
#include<string>
#include <DirectXMath.h>
#include <vector>
#include "ObjectObj.h"
#include <fstream>
#include <json.hpp>
#include "TouchableObject.h"

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

	struct ColliderData {
		DirectX::XMFLOAT3 translation;
		DirectX::XMFLOAT3 scaling;
	};

public:
	SceneLoader();
	~SceneLoader();

	void Initialize();

	void ScanningObjects(nlohmann::json& transform);

	void Update();

	void Draw();

	bool Collision(XMFLOAT3 playerpos, XMFLOAT3 radius);
private:

	LevelData* levelData;

	//オブジェクトデータ
	std::vector<ObjectObj*>objects;
	//オブジェクトデータ
	std::list<TouchableObject*>touchObjects;

	std::vector<ColliderData*>colliders;
};

