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
			std::string objectName;	//オブジェクトネーム
			std::string fileName;	//ファイル名
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

	void Initialize(const std::string& fileName);

	void ScanningObjects(nlohmann::json& transform);

	void Update();

	void Draw();

	ObjectObj* GetObjectObj(const std::string &objectName);
private:

	LevelData* levelData;

	//オブジェクトデータ	//プレイヤーと当たり判定を行わないオブジェクト
	std::vector<ObjectObj*>notTouchObjects;
	//オブジェクトデータ	//プレイヤーと当たり判定を行なうオブジェクト
	std::map<std::string,TouchableObject*>touchObjects;

};

