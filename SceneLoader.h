#pragma once
#include<string>
#include <DirectXMath.h>
#include <vector>
#include "ObjectOBJ.h"
#include <fstream>
#include <json.hpp>

class SceneLoader
{
public:	//�T�u�N���X
	struct LevelData {
		//�I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {
			//�t�@�C����
			std::string fileName;
			DirectX::XMVECTOR translation;
			DirectX::XMVECTOR rotation;
			DirectX::XMVECTOR scaling;
			DirectX::XMVECTOR colliderTransla;
			DirectX::XMVECTOR colliderScaling;
		};

		//�I�u�W�F�N�g�̃R���e�i
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

	//�I�u�W�F�N�g�f�[�^
	std::vector<ObjectObj*>objects;

	std::vector<ColliderData*>colliders;
};
