#include "GameScene.h"
DirectX::XMFLOAT3 initTarget = { 0,-10,20 };
DirectX::XMFLOAT3 initEye = { 0,20,-25 };
using namespace DirectX;

//�����̂̓����蔻��
bool CubeCollision(XMFLOAT3 object1, XMFLOAT3 radius1, XMFLOAT3 object2, XMFLOAT3 radius2) {
	float disX1 = object1.x + radius1.x;
	float disX2 = object2.x - radius2.x;
	float disX3 = object1.x - radius1.x;
	float disX4 = object2.x + radius2.x;
	float disY1 = object1.y + radius1.y;
	float disY2 = object2.y - radius2.y;
	float disY3 = object1.y - radius1.y;
	float disY4 = object2.y + radius2.y;
	float disZ1 = object1.z + radius1.z;
	float disZ2 = object2.z - radius2.z;
	float disZ3 = object1.z - radius1.z;
	float disZ4 = object2.z + radius2.z;

	//x��y�͂��ꂼ�ꒆ�S���W�Ƃ��Čv�Z����
	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//3D�I�u�W�F�N�g���
	delete fbxobject;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;

	//�J�����̏����ʒu�A�����_
	camera->SetTarget(initTarget);
	camera->SetEye(initEye);

	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/texture.png");
	Sprite::LoadTexture(1, L"Resources/torisetu.png");
	Sprite::LoadTexture(2, L"Resources/clear.png");

	//�X�v���C�g�̐���
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });

	Bullet::StaticInitialize();
	//���f���̓ǂݍ���
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	groundmodel = new Model();
	groundmodel->CreateFromOBJ("ground");
	enemymodel = new Model();
	enemymodel->CreateFromOBJ("enemy");
	//3D�I�u�W�F�N�g�̐���
	fbxobject = new FBXObject;
	fbxobject->Initialize();
	fbxobject->SetModel(fbxmodel);
	fbxobject->PlayAnimetion();
	fbxobject->SetPos({ 0,0,+80 });

	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	//�v���C���[�̐�������
	player = new Player();
	player->Initialize(input, mouse, camera);
	player->object->SetScale({ 1.0f,1.0f,1.0f });
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });
	player->object->SetRotation({ 0.0f,90.0f,0.0f });

	//�G�̐�������
	enemy1 = new Enemy;
	enemy1->Initialize("enemy");


	//�^���[�̐�������
	defenseTower = DefenseTower::Create();
	defenseTower->GetOBJObject()->SetPosition({ 20,0,20 });
	bullet = Bullet::Create();

}

void GameScene::Update(int& sceneNo)
{
	//player->Update();
	//�J��������
	if (input->PushKey(DIK_RIGHT)) {
		camera->matRot *= XMMatrixRotationY(0.1f);
	}
	else if (input->PushKey(DIK_LEFT)) {
		camera->matRot *= XMMatrixRotationY(-0.1f);
	}

	if (Input::GetInstance()->PushKey(DIK_H)) {
		int num = 0;
	}




	//3D�I�u�W�F�N�g�X�V
	fbxobject->Update();
	player->Update();
	ground->Update();
	defenseTower->Update(player);
	bullet->Update();
	enemy1->Update();

	camera->matRot *= XMMatrixRotationY(0.8f * mouse->MoveMouseVector('x') / 1000);

	//XMFLOAT3 rote = player->object->GetRotation();
	XMFLOAT3 pos = player->object->GetPosition();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	//matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	camera->SetEye({ player->object->GetPosition().x + movement.m128_f32[0] * 80, player->object->GetPosition().y + movement.m128_f32[1] * 80,
		 player->object->GetPosition().z + movement.m128_f32[2] * 80 });
	camera->eye.y = 20;
	camera->target = player->object->GetPosition();
	//�v���C���[���W�����v���������_������Ɍ����̂�h�~���邽�߂̏���
	//camera->target.y = player->graundheight;


	//�J�����̍X�V
	//camera->SetEye({ player->object->GetPosition().x + initEye.x,player->object->GetPosition().y + initEye.y,
	//	player->object->GetPosition().z + initEye.z });
	//camera->SetTarget({ player->object->GetPosition().x + initTarget.x,player->object->GetPosition().y + initTarget.y,
	//	player->object->GetPosition().z + initTarget.z });
	camera->SetWorldMatrix(player->object->GetWorldMatrix());
	camera->Update();
}

void GameScene::Draw()
{
	OBJobject::PreDraw(dxCommon->GetCmdList());
	player->object->Draw();
	ground->Draw();
	int num = 0;
	defenseTower->Draw();
	bullet->Draw();
	enemy1->Draw();

	//fbxobject->Draw(dxCommon->GetCmdList());
	OBJobject::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	if (num >= 6) {
		//clearsprite->Draw();
	}
	Sprite::PostDraw();

}