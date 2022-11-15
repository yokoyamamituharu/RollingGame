#include "StrongZakoEnemy.h"

Model* StrongZakoEnemy::strongEnemyModel = nullptr;

void StrongZakoEnemy::Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1, XMFLOAT3 targetPos2)
{
	this->isFiled = filedFlag;
	this->camera = camera;
	this->isTarget = isTarget;
	this->targetPos.m128_f32[0] = targetPos1.x;
	this->targetPos.m128_f32[1] = targetPos1.y;
	this->targetPos.m128_f32[2] = targetPos1.z;
	//オブジェクトの作成
	object = OBJobject::Create();
	object->SetModel(strongEnemyModel);

	//外用の敵としてインスタンスが生成された場合
	if (filedFlag == 1) {
		object->SetPosition(pos);
		object->SetScale({ 4.0f,4.0f, 4.0f });

		//この敵が中シーンに移動した時に持っている小敵の情報を追加
		int maxEnemyNum = rand() % 2 + 5;
		for (int i = 0; i < maxEnemyNum; i++)
		{
			//敵をリストに追加していく
			std::unique_ptr<StrongZakoEnemy> newEnemyZako = std::make_unique<StrongZakoEnemy>();
			newEnemyZako->Initialize(FIELD_IN, camera);
			//リストに登録
			enemies.push_back(std::move(newEnemyZako));
		}

		//目的地
		if (isTarget == true) {
			//移動する方向を計算する
			XMVECTOR pos1;
			pos1.m128_f32[0] = object->GetPosition().x;
			pos1.m128_f32[1] = object->GetPosition().y;
			pos1.m128_f32[2] = object->GetPosition().z;
			targetVec = pos1 - this->targetPos;
			targetVec = XMVector3Normalize(targetVec);
			targetVec.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる
		}
	}
	//中用の敵としてインスタンスが生成された場合
	else if (filedFlag == 2) {
		//座標のセット
		float x = rand() % 200 - 100;
		float z = rand() % 200 - 100;
		object->SetPosition({ x,groundInPos + 6.0f,z });
		//サイズのセット
		object->SetScale({ 4.0f,4.0f, 4.0f });
		object->SetScale({ 10,10,10 });
	}


}

void StrongZakoEnemy::Update()
{
	if (isFiled == FIELD_OUT && isAction > 0) {
		//HPによってサイズを変える
		scale = hp / maxHp;
		if (scale <= 0) {
			scale = 1;
			object->SetRotation({ 180,90,0 });
		}
		object->SetScale(maxScale * scale);

		//目的地へと行く処理
		if (isTarget == true) {
			XMVECTOR pos;
			pos.m128_f32[0] = object->GetPosition().x;
			pos.m128_f32[1] = object->GetPosition().y;
			pos.m128_f32[2] = object->GetPosition().z;
			pos -= targetVec * 1;
			object->SetPosition({ pos.m128_f32[0],pos.m128_f32[1] ,pos.m128_f32[2] });
		}
	}
	else if (isFiled == FIELD_IN/* && isAction > 0*/) {

		
		MoveMae();
	}
	//オブジェクトの更新
	object->Update();
}

void StrongZakoEnemy::CreateStrongEnemyModel()
{
	//敵用モデルを読み込み
	strongEnemyModel = Model::Create("strongEnemy");
}

void StrongZakoEnemy::Move()
{
}

void StrongZakoEnemy::MoveMae()
{


	if (Nitenkan(player->object->GetPosition(), object->GetPosition()) <= 10.0f) {
		maeTime++;
	}
	else {
		maeTime--;
		if (maeTime < 0) {
			maeTime = 0;
		}
	}

	if (maeTime > 20) {
		Dossun();
		maeTime = 0;
	}

	if (dossunFlag == false)
	{
		object->VecSetPosition({ 0,0,0.3 });
	}

}

void StrongZakoEnemy::Dossun()
{
	if (dosuntime1 < 60) {
		dosuntime1++;
		object->VecSetPosition({ 0,1.1f,0 });
	}
	else if (dosuntime2 < 60) {
		dosuntime2++;
		object->VecSetPosition({ 0,-1.1f,0 });
	}
	else {
		dosuntime1 = 0;
		dosuntime2 = 0;

	}




}
