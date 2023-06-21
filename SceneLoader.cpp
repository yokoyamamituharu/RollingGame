#include "SceneLoader.h"
#include "ModelManager.h"
#include "../Collider/Collision.h"
#include "Useful.h"
#include "safe_delete.h"

SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
	notTouchObjects.clear();
	for (auto& object : touchObjects) {
		//safe_delete(object);
	}
	for (auto itr = touchObjects.begin(); itr != touchObjects.end(); itr++) {
		safe_delete(itr->second);
	}
	touchObjects.clear();
}

void SceneLoader::Initialize(const std::string& fileName, std::list<std::shared_ptr<DefenseTower>>* towers)
{
	//�p�X
	const std::string fullpath = std::string("Resources/levels/") + fileName + ".json";

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);

	//�t�@�C���I�[�v���̎��s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	levelData = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	ScanningObjects(deserialized);

	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		//���f�������擾�ł��ĂȂ�������X�L�b�v
		if (objectData.fileName == "")continue;
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = ModelManager::GetModel(objectData.fileName);
		if (objectData.isTower) {
			std::shared_ptr<DefenseTower>tower = DefenseTower::CreateA();
			//���W
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			tower->GetObjectObj()->SetPosition(pos);
			//��]�p
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			tower->GetObjectObj()->SetRotation(rot);
			//�X�P�[�����O
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			tower->GetObjectObj()->SetScale(scale);
			towers->emplace_back(std::move(tower));
		}
		else {
			TouchableObject* newObject = TouchableObject::Create(model);
			//���W
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);
			//��]�p
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			if (objectData.isLoadObj) {
				rot.z -= 90.0f;
			}
			newObject->SetRotation(rot);
			//�X�P�[�����O
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);
			//���O��o�^
			newObject->SetName(objectData.fileName);
			//�z��ɓo�^
			//touchObjects.insert(objectData.fileName,);
			touchObjects[objectData.objectName] = newObject;
		}
	}


}

void SceneLoader::ScanningObjects(nlohmann::json& deserialized)
{
	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		if (type.compare("MESH") == 0) {
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//�t�@�C����
				objectData.fileName = object["file_name"];
			}

			//�I�u�W�F�N�g��
			objectData.objectName = object["name"];

			//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.translation.m128_f32[0] = (float)transform["translation"][1];
			objectData.translation.m128_f32[1] = (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = (float)-transform["translation"][0];
			objectData.translation.m128_f32[3] = 1.0f;
			//��]�p
			objectData.rotation.m128_f32[0] = (float)-transform["rotation"][1];
			objectData.rotation.m128_f32[1] = (float)-transform["rotation"][2];
			objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];			
			objectData.rotation.m128_f32[3] = 0.0f;
			//�X�P�[�����O
			objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scaling.m128_f32[3] = 0.0f;

			if (object.contains("collider") == 0) {
				nlohmann::json& colliderTransform = object["collider"];
				//���s�ړ�
				objectData.colliderTransla.m128_f32[0] = (float)transform["translation"][1];
				objectData.colliderTransla.m128_f32[1] = (float)transform["translation"][2];
				objectData.colliderTransla.m128_f32[2] = (float)-transform["translation"][0];
				objectData.colliderTransla.m128_f32[3] = 1.0f;
				//�X�P�[�����O
				objectData.colliderScaling.m128_f32[0] = (float)transform["scaling"][1];
				objectData.colliderScaling.m128_f32[1] = (float)transform["scaling"][2];
				objectData.colliderScaling.m128_f32[2] = (float)transform["scaling"][0];
				objectData.colliderScaling.m128_f32[3] = 0.0f;
			}

			if (object.contains("isTower")) {				
				objectData.isTower = true;
			}
			if (object.contains("isLoadObj")) {
				objectData.isLoadObj = true;
			}
		}

		//�ċA����		
		if (object.contains("children")) {
			nlohmann::json& transform = object["children"];
			//�p�X
			const std::string fullpath = std::string("Resources/levels/") + "scene.json";

			//�t�@�C���X�g���[��
			std::ifstream file;

			//�t�@�C�����J��
			file.open(fullpath);

			//�t�@�C���I�[�v���̎��s���`�F�b�N
			if (file.fail()) {
				assert(0);
			}

			//JSON�����񂩂�𓀂����f�[�^
			nlohmann::json deserialized;

			//��
			file >> deserialized;

			//���������x���f�[�^�t�@�C�����`�F�b�N
			assert(deserialized.is_object());
			assert(deserialized.contains("name"));
			assert(deserialized["name"].is_string());

			//"name"�𕶎���Ƃ��Ď擾
			std::string name = deserialized["name"].get<std::string>();
			//���������x���f�[�^�t�@�C�����`�F�b�N
			assert(name.compare("scene") == 0);
			ScanningObjects(transform);
		}
	}
}

void SceneLoader::Update()
{
	for (auto& object : notTouchObjects) {
		object->Update();
	}
	for (auto itr = touchObjects.begin(); itr != touchObjects.end(); itr++) {
		if (itr->second->GetModel() == ModelManager::GetModel("kabe")) continue;
		itr->second->Update();
	}
}

void SceneLoader::Draw()
{
	for (auto& object : notTouchObjects) {
		object->Draw();
	}
	for (auto itr = touchObjects.begin(); itr != touchObjects.end(); itr++) {
		itr->second->Draw();
	}
}

ObjectObj* SceneLoader::GetObjectObj(const std::string& objectName)
{
	return touchObjects[objectName];
}
