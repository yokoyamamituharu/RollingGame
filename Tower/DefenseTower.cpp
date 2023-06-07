#include "DefenseTower.h"
#include "Input.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "Collision.h"

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

std::shared_ptr<DefenseTower> DefenseTower::CreateA()
{
	//インスタンスを生成
	std::shared_ptr<DefenseTower> defenseTower = std::make_shared<DefenseTower>();

	//初期化処理
	if (!defenseTower->Initialize()) {
		//delete defenseTower;
		assert(0);
		return nullptr;
	}

	defenseTower->object->SetScale({ 7, 7, 7 });
	defenseTower->object->SetPosition({ 0, 10, 0 });
	return std::move(defenseTower);
}

DefenseTower::DefenseTower()
{
}

DefenseTower::~DefenseTower()
{
	safe_delete(object);
}

void DefenseTower::Update(std::list<std::shared_ptr<BaseEnemy>>& enemies)
{
	//フラグが立った弾を消す
	bullets.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->GetDead(); });

	if (playerptr) {
		if (Collision::CheckDistance(object->GetPosition(), playerptr->object->GetPosition()) <= 40.0f)
			if (state == State::not && Input::GetInstance()->PushKey(DIK_E)) {
				object->SetModel(ModelManager::GetModel("defenseTower"));
				state = State::idle;
			}
	}

	object->Update();

	if (state == State::not)return;

	//検知範囲に敵が入ったら攻撃開始
	//ターゲットのエネミーが空（倒されている）だったら新たなターゲットを探す
	if (targetEnemy.expired()) {
		for (std::shared_ptr<BaseEnemy>& enemy : enemies) {
			if (Collision::CheckDistance(object->GetPosition(), enemy->object->GetPosition()) <= 100.0f &&
				enemy->GetHp() > 0) {
				attackFlag = true;
				targetEnemy = enemy;
				break;
			}
			else {
				attackFlag = false;
			}
		}
	}
	if (targetEnemy.lock() == nullptr) {
		attackFlag = false;
	}


	//こっちでやった方がいいのかどうかはわかんねぇや
	//std::shared_ptr<int> ptr = targetEnemy.lock();

	//弾の生成処理、弾を敵のいたとこに発射
	if (attackFlag == true) {
		if (interval <= 0) {
			//本来なら敵のマネージャーとかから対象の敵のを取得
			DirectX::XMFLOAT3 targetpos = targetEnemy.lock()->object->GetPosition();
			targetpos.y = 0;
			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet->Initialize({ object->GetPosition().x,8.0f,object->GetPosition().z }, targetpos, true);
			bullets.push_back(std::move(newBullet));
			interval = maxInterval;
		}
		else {
			interval--;
		}
	}

	//オブジェクトの更新
	object->Update();

	for (std::unique_ptr<Bullet>& bullet : bullets) {
		if (targetEnemy.expired() == false) {
			if (5.0f >= Collision::CheckDistance(bullet->object->GetPosition(), targetEnemy.lock()->object->GetPosition())) {
				targetEnemy.lock()->DamageOut(1);
				if (targetEnemy.lock()->GetHp() <= 0) {
					//Player::breakEnemy += 1;
				}
				bullet->Dead();
			}
		}
		bullet->Update();
	}

	//ターゲットの破棄の条件
	if (targetEnemy.expired() == false) {
		if (100.0f < Collision::CheckDistance(object->GetPosition(), targetEnemy.lock()->object->GetPosition()) ||
			targetEnemy.lock()->GetHp() <= 0) {
			targetEnemy.reset();
			attackFlag = false;
		}
		//if (targetEnemy.lock()->GetHp() <= 0) {
		//	Player::breakEnemy += 1;
		//}
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
	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("tower1"));
	state = State::not;
	return true;
}