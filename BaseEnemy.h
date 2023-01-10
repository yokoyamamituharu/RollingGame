//#pragma once
//#include "ObjectObj.h"
//#include "Input.h"
//#include "InputMouse.h"
//#include <DirectXMath.h>
//#include "Player.h"
//#include "Enemy.h"
//class BaseEnemy
//{
//private:
//	//DirectXを省略
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMVECTOR = DirectX::XMVECTOR;
//	using XMMATRIX = DirectX::XMMATRIX;
//
//public:	//サブクラス
//	enum FIELDLOCATION {
//		FIELD_NONE,
//		FIELD_OUT,
//		FIELD_IN
//	};
//
//public:	//静的メンバ関数
//	/// <summary>
//	/// インスタンスを生成
//	/// </summary>
//	/// <param name="filedFlag">生成フィールド</param>
//	/// <param name="pos">初期位置</param>
//	/// <param name="isTarget">目的地が設定されているかどうか</param>
//	/// <returns>敵クラスのインスタンス</returns>
//	static BaseEnemy* CreateIn(int filedFlag, XMFLOAT3 pos, bool isTarget);
//
//	/// <summary>
//	/// シーン内にいる敵の行動を全停止、もしくは動かす
//	/// </summary>
//	/// <param name="flag">行動するか否か</param>
//	static void Action(bool flag) {
//		isAction = flag;
//	}
//
//public:	//静的メンバ変数
//	//ざこてきの地面となるの座標
//	static const float groundOutPos;
//	static const float groundInPos;
//	//敵の行動させる/させないフラグ
//	static int isAction;
//
//public:	//メンバ関数	
//
//	//コンストラクタ
//	BaseEnemy();
//
//	//デストラクタ
//	~BaseEnemy();
//
//	/// <summary>
//	/// 外シーンの敵の初期化処理
//	/// </summary>
//	/// <param name="pos">初期位置</param>
//	/// <param name="isTarget">目的地が設定されているかどうか</param>
//	/// <param name="targetPos1">目的地2</param>
//	/// <param name="targetPos2">目的地1</param>
//	virtual void InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });
//
//	/// <summary>
//	/// 中シーンの敵の初期化処理
//	/// </summary>
//	virtual void InitializeIn();
//
//	/// <summary>
//	/// 外シーンでの更新処理
//	/// </summary>
//	virtual void UpdateOut();
//
//	/// <summary>
//	/// 中シーンでの更新処理
//	/// </summary>
//	virtual void UpdateIn();
//
//	/// <summary>
//	/// 敵の描画
//	/// </summary>
//	void Draw();
//
//protected:
//	/// <summary>
//	/// プレイヤーの方向を見る処理
//	/// </summary>
//	/// <param name="player"></param>
//	void ViewpointPlayer(Player* player);
//
//	/// <summary>
//	/// プレイヤーの方向へ進む処理
//	/// </summary>
//	void ApproachPlayer();
//
//	/// <summary>
//	///	攻撃前の予備動作
//	/// </summary>
//	void PreliminaryOperation();
//
//	/// <summary>
//	/// 死亡時のパーティクルをパーティクルクラスに追加
//	/// </summary>
//	void BreakParticle();
//
//
//public:	//セッター、ゲッター	
//
//	/// <summary>
//	/// ダメージを受ける処理
//	/// </summary>
//	/// <param name="attackPower">受けるダメージ値</param>
//	void Damege(int damage);
//
//	/// <summary>
//	/// 自身の行動を止める
//	/// </summary>
//	void Stop();
//
//	/// <summary>
//	/// 攻撃対象のプレイヤーをセットする
//	/// </summary>
//	/// <param name="player"></param>
//	void SetPlayer(Player* player)
//	{
//		this->player = player;
//	}
//
//	/// <summary>
//	/// 死亡判定を付ける
//	/// </summary>
//	void SetDead() { dead = true; }
//
//	/// <summary>
//	/// 死亡判定を外す
//	/// </summary>
//	void NotDead() { dead = false; }
//
//	/// <summary>
//	/// 敵の死亡判定をとる
//	/// </summary>
//	/// <returns>死んでたらtrue</returns>
//	bool GetDead() { return dead; }
//
//	/// <summary>
//	/// 攻撃判定を取る
//	/// </summary>
//	/// <returns>攻撃判定を持っているか否か</returns>
//	bool GetAttack() { return attackFlag; }
//
//	/// <summary>
//	/// ライフの値を取得
//	/// </summary>
//	/// <returns>ライフの値</returns>
//	float GetHp() { return hp; }
//
//	/// <summary>
//	/// 外シーン用の敵が持っている中シーン用の敵のリストを取得
//	/// </summary>
//	/// <returns>敵クラスのリスト</returns>
//	std::list<std::unique_ptr<BaseEnemy>>& GetEnemies() { return InEnemies; }
//
//	/// <summary>
//	/// パーティクルを生成
//	/// </summary>
//	void ParticleCreate();
//
//public:
//	ObjectObj* object = nullptr;	//オブジェクト
//	XMFLOAT3 oldPos = { 0,0,0 };	//1フレーム前のポジション
//
//protected:	//メンバ変数
//	//ポインター
//	Player* player;		//プレイヤーのポインター
//
//	//丸影
//	ObjectObj* shadowObj = nullptr;		//丸影
//
//	//フラグ
//	bool atattckFlag = false;	//攻撃中かどうか
//	bool dead = false;			//志望しているかどうか
//	bool nearFlag = false;		//プレイヤーに近いかどうか（周りを回る判定にはいっているかどうかのフラグ）
//
//	int isFiled = FIELD_NONE;	//1:外シーン、2:中シーン
//
//	bool stopFlag = false;		//攻撃の終わりに停止させる
//	bool animeEndFlag = false;
//
//	bool maeburiFlag = false;	//攻撃の前ぶりアニメショーン再生フラグ
//	bool tossinFlag = false;	//突進攻撃をする
//	bool attackFlag = false;	//攻撃中
//	bool isTarget = false;		//生成した時に目的地が設定されたか
//
//
//
//	//タイマー
//	int jumpTime = 0;
//	int maxStorpTime = 0;
//	int stopTime = maxStorpTime;
//	int waitTime = 0;
//	int time = 0;
//	int timer1 = 0;
//	int tossintime = 0;
//	int moveTime = 0;
//	int attackTime = 0;
//	int rotaTime = 0;
//
//	/*座標*/
//	//ターゲットに進む方向
//	XMVECTOR targetVec;
//	//ターゲットの場所
//	XMFLOAT3 targetPos1;
//	XMFLOAT3 targetPos2;
//	int targetIndex;//いま何個目のターゲットなのか
//
//	/*その他*/
//	XMFLOAT3 atodekesuROta = { 0,0,0 };
//	int num1 = 0;
//	int num2 = 0;
//	XMFLOAT3 move;
//
//
//	float m_Radius;		// 半径(描画用)
//	float m_Angle = 0;	// 角度
//	float m_Length = 50;// 半径の長さ
//	XMFLOAT3 rollPoint;
//
//	
//	XMVECTOR attackDirection;	//突進する方向
//
//
//	float maxHp = 100;
//	float hp = maxHp;
//
//	float scale = 1;
//	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };
//
//
//	std::list<std::unique_ptr<BaseEnemy>>InEnemies;
//};