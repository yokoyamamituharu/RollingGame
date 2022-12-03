#include "SceneLoader.h"
#include "ModelManager.h"




void SceneLoader::Initialize()
{
	//�p�X
	const std::string fullpath = std::string("Resources/levels/")+"scene.json";

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);

	//�t�@�C���I�[�v���̎��s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//JSON�������񂩂�𓀂����f�[�^
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
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		//decltype(models)::iterator it = models.find(objectData.fileName);
		//if (it != models.end()) { model = it->second; }
		model = ModelManager::GetModel(objectData.fileName);
		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		ObjectObj* newObject = ObjectObj::Create(model);
		//���W
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);
		//��]�p
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->SetRotation(rot);
		//�X�P�[�����O
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);
		//�z��ɓo�^
		objects.push_back(newObject);
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

			if (type.compare("Collider") == 0) {

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

			//JSON�������񂩂�𓀂����f�[�^
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
	for (auto& object : objects) {
		object->Update();
	}
}

void SceneLoader::Draw()
{
	for (auto& object : objects) {
		object->Draw();
	}
}
