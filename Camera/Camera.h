#pragma once
#include <DirectXMath.h>

class Camera
{
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:	//メンバ関数
	Camera();
	~Camera();

	/// <summary>
	/// カメラの生成
	/// </summary>
	/// <returns>生成したカメラのポインター</returns>
	static Camera* Create();

	/// <summary>
	/// カメラクラスの初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	virtual void Update();

	void UpdateMat();

	void UpdateView();

	//上下の視点移動
	void VecUp(float scaler) {
		matRot *= DirectX::XMMatrixRotationY(0.8f * scaler);
	}

	//左右の視点移動
	void VecSide(float scaler) {
		matRot *= DirectX::XMMatrixRotationX(0.8f * scaler);

	}

protected: //メンバ関数
	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	void UpdateViewMatrix();

public:	//ゲッター
	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	XMMATRIX GetMatView() {
		return matView;
	}

	/// <summary>
	/// 射影行列を取得
	/// </summary>
	/// <returns>射影行列</returns>
	XMMATRIX GetMatProjection() {
		return matProjection;
	}

	/// <summary>
	/// ビュー射影行列の取得
	/// </summary>
	/// <returns>ビュー射影行列</returns>
	XMMATRIX GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// 視点座標を取得
	/// </summary>
	/// <returns>視点座標</returns>
	XMFLOAT3 GetEye() {
		return eye;
	}

	/// <summary>
	/// 注視点座標を取得
	/// </summary>
	/// <returns>注視点座標</returns>
	XMFLOAT3 GetTarget() {
		return target;
	}

	/// <summary>
	/// 上方向ベクトルを取得
	/// </summary>
	/// <returns>上方向ベクトル</returns>
	XMFLOAT3 GetUp() {
		return eye;
	}

public:	//セッター
	/// <summary>
	/// 視点座標をセット
	/// </summary>
	/// <param name="eye">新しい視点座標</param>
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	/// <summary>
	/// 中視点座標をセット
	/// </summary>
	/// <param name="target">新しい中視点座標</param>
	void SetTarget(XMFLOAT3 target) { this->target = target; }

	/// <summary>
	/// 上方向ベクトルをセット
	/// </summary>
	/// <param name="up">新しい上方向ベクトル</param>
	void SetUp(XMFLOAT3 up) { this->target = up; }

	/// <summary>
	/// 親となる行列をセット
	/// </summary>
	/// <param name="mat">親となる行列</param>
	void SetParent(XMMATRIX& mat) { parent = &mat; }
	//void ClearParent() { parent = nullptr; }
	//void SetMatView(XMMATRIX mat) { matView = mat; }
	//void SetMatProjection(XMMATRIX mat) { matProjection = mat; }

public:	//メンバ変数
	XMMATRIX matRot;
	// 視点座標
	XMFLOAT3 eye = { 0,0,-50 };
	// 注視点座標
	XMFLOAT3 target = { 0,0,0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0,1,0 };
protected: //メンバ変数
	// ビュー行列
	XMMATRIX matView;
	// 射影行列
	XMMATRIX matProjection;
	//ビュー射影行列
	XMMATRIX matViewProjection;




	int window_width;
	int window_height;

	XMMATRIX* parent = nullptr;
};

