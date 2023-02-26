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
public:	//�T�u�N���X
	struct LevelData {
		//�I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {			
			std::string objectName;	//�I�u�W�F�N�g�l�[��
			std::string fileName;	//�t�@�C����
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

	void Initialize(const std::string& fileName);

	void ScanningObjects(nlohmann::json& transform);

	void Update();

	void Draw();

	ObjectObj* GetObjectObj(const std::string &objectName);
private:

	LevelData* levelData;

	//�I�u�W�F�N�g�f�[�^	//�v���C���[�Ɠ����蔻����s��Ȃ��I�u�W�F�N�g
	std::vector<ObjectObj*>notTouchObjects;
	//�I�u�W�F�N�g�f�[�^	//�v���C���[�Ɠ����蔻����s�Ȃ��I�u�W�F�N�g
	std::map<std::string,TouchableObject*>touchObjects;

};

