#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include <DirectXMath.h>
#include "Useful.h"
#include "safe_delete.h"
#include "../Collider/Collision.h"

using namespace DirectX;


BatlleScene::BatlleScene()
{
}

BatlleScene::~BatlleScene()
{
	//�X�v���C�g���
	safe_delete(canvas);
	safe_delete(spriteBG);

	//3D�I�u�W�F�N�g���
	safe_delete(tenQ);
	safe_delete(ground);
	safe_delete(player);
	safe_delete(area);
	enemies.reset();
	safe_delete(battleCamera);
}

void BatlleScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;

	battleCamera = BattleCamera::Create();

	player = Player::Create(battleCamera, 2);
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	battleCamera->SetPlayer(player->object);

	ground = ObjectObj::Create(ModelManager::GetModel("battlegrund"));
	ground->SetScale({ 1000.0f,1.0f,1000.0f });
	ground->SetPosition({ 0.0f,-10.1f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	tenQ = ObjectObj::Create(ModelManager::GetModel("IntenQ"));
	tenQ->SetScale({ 10.0f,1.0f,10.0f });
	tenQ->SetPosition({ 0.0f,-10.0f,0.0f });
	tenQ->SetRotation({ 0.0f,0.0f,0.0f });
	tenQ->Update();

	area = ObjectObj::Create(ModelManager::GetModel("area"));
	area->SetScale({ 50.0f,1.0f,50.0f });
	area->SetPosition({ 0.0f,-10.0f,0.0f });
	//area->SetRotation({ 0.0f,0.0f,0.0f });		

	canvas = new Canvas();
	canvas->Initialize();
}

void BatlleScene::Update(int& sceneNo, GameScene* gameScene)
{
	EnemyZako::isAction = -1;

	ObjectObj::SetCamera(battleCamera);
	if (SceneManager::hitEnemyToPlayer || SceneManager::WinBattle) {
		return;
	}

	//�O�V�[�����璆�V�[���Ɉڍs��������̏���
	if (SceneManager::BattleInit == true) {
		for (std::unique_ptr<EnemyZako>& enemy : enemies->GetEnemies()) {
			enemy->NotDead();
		}
		SceneManager::BattleInit = false;
	}

	//�G�̏����O�V�[������擾�ł��Ă����珈��
	if (enemies != 0) {
		//���S���肪��������G�l�~�[������
		enemies->GetEnemies().remove_if([](std::unique_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });		
		for (std::unique_ptr<EnemyZako>& enemy : enemies->GetEnemies()) {
			//�G�ƃv���C���[�̃��[�����O�U���̓����蔻��
			if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
				if (player->attackFlag == true) {
					enemy->SetDead();
					XMVECTOR pos1 = XMLoadFloat3(&player->object->GetPosition());
					XMVECTOR pos2 = XMLoadFloat3(&enemy->object->GetPosition());
					XMVECTOR vec = pos1 - pos2;
					vec = XMVector3Normalize(vec);
					vec.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������
					player->Res(true, Use::LoadXMVECTOR(vec));
				}
				else if (enemy->GetAttack()) {
					player->Damage(1);
				}
			}
			enemy->SetPlayer(player);
			enemy->UpdateIn();

			if (player->GetRes()&&enemy->GetDead()==false) {
				if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
					if (Collision::CheckBox2Box(player->object->GetPosition(), { 5,5,5 }, enemy->object->GetPosition(), { 2.5,5,1 }) < 20) {
						player->HitCrowAttack(enemy->object->GetPosition());
					}
				}
			}
		}

		//�o�g���V�[������E�o����V�[��
		if (enemies->GetEnemies().size() == 0) {
			player->object->SetPosition(player->outPos);
			player->Stop();
			player->breakEnemy++;	//�G�̌��j���𑝂₷
			enemies.reset();
			//sceneNo = SceneManager::SCENE_GAME;
			SceneManager::WinBattle = true;
		}
		if (player->GetHp() <= 0) {
			player->Stop();
			player->Cure(5);
			sceneNo = SceneManager::SCENE_END;
		}
	}

	const int maxEnemy = 6;
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//player->Res(true);
	}

	battleCamera->Update();
	ground->Update();
	player->Res();
	player->MoveIn();
	//�v���C���[���G���A���Ɏ��߂�
	if (Collision::CheckDistance(player->object->GetPosition() + player->move, { 0,0,0 }) > 125.0f) {
		player->move = { 0,0,0 };
	}
	player->UpdateIn();
	player->Res();
	tenQ->Update();
	area->Update();

	canvas->SetHp(player->GetMaxHp(), player->GetHp());



}

void BatlleScene::Draw()
{

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	Sprite::PostDraw();


	ObjectObj::PreDraw(dxCommon->GetCmdList());
	tenQ->Draw();
	area->Draw();

	player->Draw();
	ground->Draw();
	if (enemies != 0) {
		for (std::unique_ptr<EnemyZako>& enemy : enemies->GetEnemies()) {
			enemy->Draw();
		}
	}
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	canvas->Draw();
	Sprite::PostDraw();
}