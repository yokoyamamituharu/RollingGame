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


	//���f���̓ǂݍ���
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	playermodel = new Model();
	playermodel->CreateFromOBJ("player");
	playerSpheremodel = new Model();
	playerSpheremodel->CreateFromOBJ("playerSphere");
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
	player = new Player();
	player->Initialize(input, mouse, camera);
	player->object->SetScale({ 1.0f,1.0f,1.0f });
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });
	player->object->SetRotation({ 0.0f,90.0f,0.0f });
	enemy = OBJobject::Create();
	enemy->SetModel(enemymodel);
	enemy->SetScale({ 10.0f,10.0f,10.0f });
	enemy->SetPosition({ 0.0f,10.0f,10.0f });
	enemy->SetRotation({ 0.0f,0.0f,0.0f });

	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });


	for (int i = 0; i < 6; i++) {
		enemys[i] = new Enemy();
		enemys[i]->Initialize(input, mouse, camera);
		enemys[i]->object->SetScale({ 4.0f,4.0f,4.0f });		
		enemys[i]->object->SetRotation({ 0.0f,90.0f,0.0f });
		alive[i] = true;
		enemys[i]->SetPlayer(player);
	}

	enemys[0]->object->SetScale({ 8.0f,8.0f,8.0f });
	enemys[0]->object->SetPosition({ 0.0f,3.0f,0.0f });
	enemys[1]->object->SetPosition({ 100.0f,-3.0f,50.0f });
	enemys[2]->object->SetPosition({ -50.0f,-3.0f,100.0f });
	enemys[3]->object->SetPosition({ 80.0f,-3.0f,200.0f });
	enemys[4]->object->SetPosition({ -200.0f,-3.0f,100.0f });
	enemys[5]->object->SetPosition({ 0.0f,-3.0f,50.0f });
}

void GameScene::Update(int& sceneNo)
{
	//�J��������
	if (input->PushKey(DIK_RIGHT)) {
		camera->matRot *= XMMatrixRotationY(0.1f);
	}
	else if (input->PushKey(DIK_LEFT)) {
		camera->matRot *= XMMatrixRotationY(-0.1f);
	}

	camera->matRot *= XMMatrixRotationY(0.8f * mouse->MoveMouseVector('x') / 1000);

	XMFLOAT3 rote = player->object->GetRotation();
	XMFLOAT3 pos = player->object->GetPosition();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matRot);
	movement *= XMVECTOR{ -1, -1, -1 };

	matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	camera->SetEye({ player->object->GetPosition().x + movement.m128_f32[0] * 80, player->object->GetPosition().y + movement.m128_f32[1] * 80,
		 player->object->GetPosition().z + movement.m128_f32[2] * 80 });
	camera->eye.y = 20;
	camera->target = player->object->GetPosition();
	//�v���C���[���W�����v���������_������Ɍ����̂�h�~���邽�߂̏���
	//camera->target.y = player->graundheight;


	//�J�����̍X�V
	/*camera->SetEye({ player->object->GetPosition().x + initEye.x,player->object->GetPosition().y + initEye.y,
		player->object->GetPosition().z + initEye.z });
	camera->SetTarget({ player->object->GetPosition().x + initTarget.x,player->object->GetPosition().y + initTarget.y,
		player->object->GetPosition().z + initTarget.z });*/
	camera->Update();
	
	flag1 = false;
	/*if (CubeCollision(enemys->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
		flag1 = true;
	}*/

	//�G�ƃv���C���[�̃��[�����O�U���̓����蔻��
	for (int i = 0; i < 6; i++) {
		if (CubeCollision(enemys[i]->object->GetPosition(), {2.5,5,1}, player->object->GetPosition(), {5,5,5})
			&&player->attackFlag==true) {
			alive[i] = false;
		}
	}

	//�V�[���̐؂�ւ�
	if (alive[0] == false)
	{
		sceneNo = 2;
	}


	//enemys[0]->Direction(player);
	//if(input->PushKey(DIK_1)){
	//	enemys[0]->GoTarget(player);
	//}
	//if (input->PushKey(DIK_2)) {

	//	enemys[0]->Mawarikomi(player);
	//}
	//else
	//{
	//	enemys[0]->num = 0;
	//}

		//�s���̍X�V
	//time++;
	//if (time < 100)
	//{
	//	XMFLOAT3 pos = enemys[0]->object->GetPosition();
	//	pos.z += 0.3;
	//	enemys[0]->object->SetPosition(pos);
	//}
	//else if (time < 200)
	//{
	//	enemys[0]->Mawarikomi(player);
	//}
	//else
	//{
	//	enemys[0]->GoTarget(player);
	//	if (time > 280)
	//	{
	//		time = 0;
	//	}
	//}


	//3D�I�u�W�F�N�g�X�V
	fbxobject->Update();
	enemy->Update();
	player->Update();
	ground->Update();
	for (int i = 0; i < 6; i++) {
		enemys[i]->Update();
	}

}

void GameScene::Draw()
{


	OBJobject::PreDraw(dxCommon->GetCmdList());
	player->object->Draw();
	if (flag1) {
		enemy->Draw();
	}
	ground->Draw();
	int num = 0;
	for (int i = 0; i < 6; i++) {
		if (alive[i] == true) {
			//enemys[i]->object->Draw();
		}
		else {
			num++;
		}
	}

	enemys[0]->object->Draw();
	//fbxobject->Draw(dxCommon->GetCmdList());

	OBJobject::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	if (num >= 6){
		//clearsprite->Draw();
	}
	Sprite::PostDraw();

}
