#include "DefenseTower.h"
#include "Input.h"

DefenseTower* DefenseTower::Create()
{
	//インスタンスを生成
	DefenseTower* defenseTower = new DefenseTower;

	//初期化処理
	if (!defenseTower->Initialize()) {
		delete defenseTower;
		assert(0);
		return nullptr;
	}

	defenseTower->object->SetScale({ 7, 7, 7 });
	defenseTower->object->SetPosition({ 0, 10, 0 });
	return defenseTower;
}

void DefenseTower::Update(const Player* player)
{
	//フラグが立った弾を消す
	bullets.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->IsDead(); });


	//検知範囲に敵が入ったら攻撃開始
	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		attackFlag = true;
	}

	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		attackFlag = false;
	}
	//弾の生成処理、弾を敵のいたとこに発射
	if (attackFlag == true) {
		if (interval <= 0) {
			//本来なら敵のマネージャーとかから対象の敵のを取得
			DirectX::XMFLOAT3 target = player->object->GetPosition();
			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet->Initialize(object->GetPosition(), player->object->GetPosition(), true);
			bullets.push_back(std::move(newBullet));
			interval = maxInterval;
		}
		else {
			interval--;
		}
	}

	//とりあえずプレイヤーにむけて撃つ

	object->Update();

	for (std::unique_ptr<Bullet>& bullet : bullets) {
		bullet->Update();
	}
}

void DefenseTower::Draw()
{
	object->Draw();
	for (std::unique_ptr<Bullet>& bullet : bullets) {
		bullet->Draw();
	}
}

bool DefenseTower::Initialize()
{
	//タワー用モデルを読み込み
	model = new Model();
	model->CreateFromOBJ("defenseTower");
	//オブジェクトの作成
	object = OBJobject::Create();
	object->SetModel(model);

	return true;
}