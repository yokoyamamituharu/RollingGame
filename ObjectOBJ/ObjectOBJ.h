﻿#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include "Camera.h"
#include "CollisionInfo.h"
class BaseCollider;

using namespace DirectX;
//using DirectX::operator+;






/// <summary>
/// 3Dオブジェクト
/// </summary>
class ObjectObj
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		XMFLOAT3 specular;	//スペキュラー係数
		float blackOutAlpha;		//アルファ
	};



private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="camera">カメラ</param>	
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, Camera* camera);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ObjectObj* Create(Model* model = nullptr, XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	static void SetCamera(Camera* camera) { ObjectObj::camera = camera; }

protected: // 静的メンバ変数
#pragma region 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* commandList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//カメラ
	static Camera* camera;

#pragma endregion
protected:// 静的メンバ関数
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();


public: // メンバ関数
	ObjectObj();
	~ObjectObj();

	bool Initialize(Model* model = nullptr);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateWorldMatrix();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// スケールの取得
	/// </summary>
	///	<returns>スケール</returns>
	const XMFLOAT3& GetScale() { return scale; }


	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns>回転</returns>
	const XMFLOAT3& GetRotation() { return rotation; }


	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// </summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	void SetPosX(float x) { this->position.x = x; }
	void SetPosY(float y) { this->position.y = y; }
	void SetPosZ(float z) { this->position.z = z; }

	void SetName(std::string name) { objectName = name; }

	//指定した値分オブジェクトの座標を移動させる
	void VecSetPosition(XMFLOAT3 scalar)
	{
		this->position.x += scalar.x;
		this->position.y += scalar.y;
		this->position.z += scalar.z;
	}

	//指定した値分オブジェクトのサイズを変える
	void VecSetScale(XMFLOAT3 scalar)
	{
		this->scale.x += scalar.x;
		this->scale.y += scalar.y;
		this->scale.z += scalar.z;
	}

	//指定した値分オブジェクトの角度を傾ける
	void VecSetRotation(XMFLOAT3 scalar)
	{
		this->rotation.x += scalar.x;
		this->rotation.y += scalar.y;
		this->rotation.z += scalar.z;
	}

	//オブジェクトの回転行列の取得
	XMMATRIX GetMatRot();

	//オブジェクトの行列を取得
	XMMATRIX GetWorldMatrix();

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	//BaseCollider GetCollider() { return collider; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

protected: // メンバ変数
	//ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	ObjectObj* parent = nullptr;
	//モデルデータ
	Model* modelData;

	std::string objectName = "NONE";
public:
	// コライダー
	BaseCollider* collider = nullptr;

public:
	void SetModel(Model* model);
	Model* GetModel() { return modelData; }
};

