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
	objects.clear();
	for (auto& object : touchObjects) {
		//safe_delete(object);
	}
	touchObjects.clear();
}

void SceneLoader::Initialize()
{
	//パス
	const std::string fullpath = std::string("Resources/levels/") + "scene.json";

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);

	//ファイルオープンの失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	ScanningObjects(deserialized);

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		//decltype(models)::iterator it = models.find(objectData.fileName);
		//if (it != models.end()) { model = it->second; }
		model = ModelManager::GetModel(objectData.fileName);

		if (objectData.fileName == "ground") {
			//モデルを指定して3Dオブジェクトを生成
			ObjectObj* newObject = ObjectObj::Create(model);
			//座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);
			//回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation(rot);
			//スケーリング
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);
			//配列に登録
			objects.push_back(newObject);
		}
		else {
			TouchableObject* newObject = TouchableObject::Create(model);
			//座標
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, objectData.translation);
			newObject->SetPosition(pos);
			//回転角
			DirectX::XMFLOAT3 rot;
			DirectX::XMStoreFloat3(&rot, objectData.rotation);
			newObject->SetRotation(rot);
			//スケーリング
			DirectX::XMFLOAT3 scale;
			DirectX::XMStoreFloat3(&scale, objectData.scaling);
			newObject->SetScale(scale);
			//配列に登録
			//touchObjects.insert(objectData.fileName,);
			touchObjects[objectData.fileName] = newObject;
		}



		//トランスフォームを参照してコライダーを生成する
		//ColliderData* newCollider = new ColliderData;
		////座標
		//DirectX::XMFLOAT3 cpos = { 0,0,0 };
		//DirectX::XMStoreFloat3(&pos, objectData.colliderTransla);
		//newCollider->translation = cpos;
		////回転角
		//DirectX::XMFLOAT3 cscale = { 0,0,0 };
		//DirectX::XMStoreFloat3(&cscale, objectData.colliderScaling);
		//newCollider->scaling = cscale;
		//colliders.push_back(newCollider);
	}

}

void SceneLoader::ScanningObjects(nlohmann::json& deserialized)
{
	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパロメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.m128_f32[0] = (float)transform["translation"][1];
			objectData.translation.m128_f32[1] = (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = (float)-transform["translation"][0];
			objectData.translation.m128_f32[3] = 1.0f;
			//回転角
			objectData.rotation.m128_f32[0] = (float)-transform["rotation"][1];
			objectData.rotation.m128_f32[1] = (float)-transform["rotation"][2];
			//objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
			objectData.rotation.m128_f32[2] = 0.0f;
			objectData.rotation.m128_f32[3] = 0.0f;
			//スケーリング
			objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scaling.m128_f32[3] = 0.0f;

			if (object.contains("collider") == 0) {
				nlohmann::json& colliderTransform = object["collider"];
				//平行移動
				objectData.colliderTransla.m128_f32[0] = (float)transform["translation"][1];
				objectData.colliderTransla.m128_f32[1] = (float)transform["translation"][2];
				objectData.colliderTransla.m128_f32[2] = (float)-transform["translation"][0];
				objectData.colliderTransla.m128_f32[3] = 1.0f;
				//スケーリング
				objectData.colliderScaling.m128_f32[0] = (float)transform["scaling"][1];
				objectData.colliderScaling.m128_f32[1] = (float)transform["scaling"][2];
				objectData.colliderScaling.m128_f32[2] = (float)transform["scaling"][0];
				objectData.colliderScaling.m128_f32[3] = 0.0f;
			}
		}

		//再帰処理		
		if (object.contains("children")) {
			nlohmann::json& transform = object["children"];
			//パス
			const std::string fullpath = std::string("Resources/levels/") + "scene.json";

			//ファイルストリーム
			std::ifstream file;

			//ファイルを開く
			file.open(fullpath);

			//ファイルオープンの失敗をチェック
			if (file.fail()) {
				assert(0);
			}

			//JSONもおじ列から解凍したデータ
			nlohmann::json deserialized;

			//解凍
			file >> deserialized;

			//正しいレベルデータファイルかチェック
			assert(deserialized.is_object());
			assert(deserialized.contains("name"));
			assert(deserialized["name"].is_string());

			//"name"を文字列として取得
			std::string name = deserialized["name"].get<std::string>();
			//正しいレベルデータファイルかチェック
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
	for (auto& object : touchObjects) {
		object.
	}
}

void SceneLoader::Draw()
{
	for (auto& object : objects) {
		object->Draw();
	}
	for (auto& object : touchObjects) {
		object->Draw();
	}
}

bool SceneLoader::Collision(XMFLOAT3 playerpos, XMFLOAT3 radius)
{
	/*for (auto& collider : colliders) {
		if (CollisitonBoxToBox(collider->translation, collider->scaling, playerpos, radius)) {
			return true;
		}
	}*/
	return false;
}
