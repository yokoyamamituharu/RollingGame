#pragma once
#include "ObjectObj.h"
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

	enum FIELDLOCATION {
		FIELD_NONE,
		FIELD_OUT,
		FIELD_IN
	};

	static EnemyZako* CreateIn(int filedFlag, XMFLOAT3 pos,bool isTarget);

public:	//静的メンバ変数
//ざこてきの地面となるの座標
	static const float groundOutPos;
	static const float groundInPos;
	//敵の行動させる/させないフラグ
	static int isAction;

public:	//メンバ関数	

	//コンストラクタ
	EnemyZako();

	//デストラクタ
	~EnemyZako();	

	virtual void InitializeOut( XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });

	virtual void InitializeIn();

	virtual void UpdateOut();

	virtual void UpdateIn();

	void Draw();
	
protected:
	//プレイヤーの方を向かせる処理
	void ViewpointPlayer(Player* player);

	//プレイヤーの方に歩み寄る関数
	void ApproachPlayer();

	//突進前の予備動作
	void PreliminaryOperation();
	
	void BreakParticle();


public:	//セッター、ゲッター	
	void Damege(int attackPower);
	
	void SetPlayer(Player* player)
	{
		this->player = player;
	}
	void Stop();

	void SetDead() { dead = true; }

	void NotDead() { dead = false; }

	/// <summary>
	/// 敵の死亡判定をとる
	/// </summary>
	/// <returns>死んでたらtrue</returns>
	bool GetDead() { return dead; }

	bool GetAttack() { return attackFlag; }

	float GetHp() { return hp; }

	std::list<std::unique_ptr<EnemyZako>>& GetEnemies() { return enemies; }

	void ParticleCreate();

public:
	ObjectObj* object = nullptr;
	XMFLOAT3 oldPos = { 0,0,0 };

protected:	//メンバ変数
	//ポインター
	Player* player;

	//丸影
	ObjectObj* shadowObj = nullptr;

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