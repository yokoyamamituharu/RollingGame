#pragma once
#include "OBJObject.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
#include "Enemy.h"
class EnemyZako
{
private:
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

	
public:	//静的メンバ関数
	//敵を行動させるかさせないか
	static void Action() {
		if (Input::GetInstance()->TriggerKey(DIK_E)) {
			isAction *= -1;
		}
	}
	static void EnemyCreateModel();


public:	//メンバ関数
#pragma region メンバ関数
	float Nitenkan(XMFLOAT3 pos1, XMFLOAT3 pos2);

	enum FIELDLOCATION {
		FIELD_NONE,
		FIELD_OUT,
		FIELD_IN
	};


	//コンストラクタ
	EnemyZako();

	//デストラクタ
	~EnemyZako();

	void Damege(int attackPower) { hp -= attackPower; }

	virtual void Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos = { 0,0,0 }, bool isTarget = false, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });
	virtual void Update();
	void Draw();

	void SetPlayer(Player* player)
	{
		this->player = player;
	}

	
#pragma endregion
public:
	void SetCamera(Camera* camera) { this->camera = camera; }

	//プレイヤーの方を向かせる処理
	void Direction(Player* player);


	std::list<std::unique_ptr<EnemyZako>>& GetEnemies() { return enemies; }

	float num = 0;

	XMFLOAT3 oldPos = { 0,0,0 };

public:
	OBJobject* object = nullptr;
	float GetHp() { return hp; }


	void Stop();

	void SetDead() { dead = true; }

	bool IsDead() { return dead; }

	bool GetAttack() { return attackFlag; }

protected:
	//プレイヤーの方に歩み寄る関数
	void DirectionWotch();
	//突進前の処理
	void Maeburi();

public:	//静的メンバ変数
	//ざこてきの地面となるの座標
	static const float groundOutPos;
	static const float groundInPos;
	static Model* enemyModel;
	//敵の行動させる/させないフラグ
	static int isAction;

protected:	//メンバ変数
	//ポインター
	Camera* camera;
	Player* player;



	//フラグ
	bool atattckFlag = false;
	bool dead = false;
	bool nearFlag = false;

	int isFiled = FIELD_NONE;	//1:外シーン、2:中シーン

	bool stopFlag = false;		//攻撃の終わりに停止させる
	bool animeEndFlag = false;
	
	bool maeburiFlag = false;	//攻撃の前ぶりアニメショーン再生フラグ
	bool tossinFlag = false;
	bool attackFlag = false;
	bool isTarget = false;		//生成した時に目的地が設定されたか

	

	//タイマー
	int jumpTime = 0;
	int maxStorpTime = 0;
	int stopTime = maxStorpTime;
	int waitTime = 0;
	int time = 0;
	int timer1 = 0;
	int tossintime = 0;
	int moveTime = 0;	
	int attackTime = 0;
	int rotaTime = 0;

	/*座標*/
	//ターゲットの座標
	XMVECTOR targetPos;
	//ターゲットに進む方向
	XMVECTOR targetVec;
	//ターゲットの場所
	XMFLOAT3 targetPos1;
	XMFLOAT3 targetPos2;
	int targetIndex;//いま何個目のターゲットなのか

	/*その他*/
	XMFLOAT3 atodekesuROta = {0,0,0};
	int num1 = 0;
	int num2 = 0;
	XMFLOAT3 move;
	
	float angleToPlayer = 0.0f;

	float m_Radius;		// 半径(描画用)
	float m_Angle = 0;	// 角度
	float m_Length = 50;// 半径の長さ
	XMFLOAT3 rollPoint;
	
	XMVECTOR attackDirection;


	float maxHp = 100;
	float hp = maxHp;

	float scale = 1;
	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };


	std::list<std::unique_ptr<EnemyZako>>enemies;
};