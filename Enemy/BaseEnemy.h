#pragma once
#include "ObjectObj.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
class BaseEnemy
{
protected:
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

public:	//静的メンバ変数
	//敵の地面となるの座標
	static const float groundOutPos;
	static const float groundInPos;
	//敵の行動させる/させないフラグ
	static int isAction;

public:	//メンバ関数	

	//コンストラクタ
	BaseEnemy();

	//デストラクタ
	~BaseEnemy();

	virtual void InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });

	virtual void InitializeIn();

	virtual void UpdateOut();

	virtual void UpdateIn();

	virtual void Draw();

protected:
	//プレイヤーの方を向かせる処理
	void ViewpointPlayer(Player* player);

	//プレイヤーの方に歩み寄る関数
	void ApproachPlayer();

	//突進前の予備動作
	void PreliminaryOperation();

	void BreakParticle();


public:	//セッター、ゲッター

	/// <summary>
	/// 外シーンでダメージを受けるときの処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	void DamageOut(int damage);

	/// <summary>
	/// 中シーンでダメージを受けるときの処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	void DamageIn(int damage);

	/// <summary>
	/// プレイヤーをセットする処理
	/// </summary>
	/// <param name="player">プレイヤーのアドレス</param>
	void SetPlayer(Player* player)
	{
		this->player = player;
	}

	/// <summary>
	/// 攻撃した後立ち止まる時間を計算する処理
	/// </summary>
	void Stop();

	/// <summary>
	/// 死亡フラグをセット
	/// </summary>
	void SetDead() { dead = true; }

	/// <summary>
	/// 死亡フラグを外す
	/// </summary>
	void NotDead() { dead = false; }

	/// <summary>
	/// 死亡判定を取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool GetDead() { return dead; }

	/// <summary>
	/// 攻撃判定を取得
	/// </summary>
	/// <returns>攻撃判定</returns>
	bool GetAttack() { return attackFlag; }

	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() { return outhp; }


	void ParticleCreate();



public:
	ObjectObj* object = nullptr;
	XMFLOAT3 oldPos = { 0,0,0 };
	std::list<std::unique_ptr<BaseEnemy>>& GetEnemies() { return enemies; }


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
	XMFLOAT3 atodekesuROta = { 0,0,0 };
	int num1 = 0;
	int num2 = 0;
	XMFLOAT3 move;


	float m_Radius;		// 半径(描画用)
	float m_Angle = 0;	// 角度
	float m_Length = 50;// 半径の長さ
	XMFLOAT3 rollPoint;

	XMVECTOR attackDirection;


	float outmaxHp = 1;
	float outhp = outmaxHp;

	float inmaxHp = 1;
	float inhp = inmaxHp;

	float scale = 1;
	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };


	std::list<std::unique_ptr<BaseEnemy>>enemies;

	//中用の敵が外シーンでマップ当たり判定にひっかかんないように
	XMFLOAT3 outPos = { 0,-100,0 };
	XMFLOAT3 inPos = { 0,-100,0 };

public:
	bool tikai = false;

};