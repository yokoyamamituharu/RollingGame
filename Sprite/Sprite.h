#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "SpriteManager.h"

class Sprite
{
protected://エエイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//新しい頂点構造体の定義
	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT4 color;		//色(RGBA)
		XMMATRIX mat;		//3D変換行列
		float value;
	};

protected://性的メンバ変数
	//テクスチャの最大枚数
	static const int srvCount = 512;
	//頂点数
	static const int vertNum = 4;
	//デバイス
	static ID3D12Device* device;
	//デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootSignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelineState;
	//射影行列
	static XMMATRIX matProjection;
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap>descHeap;
	//テクスチャバッファ
	static ComPtr<ID3D12Resource>texBuffer[srvCount];


public://静的メンバ関数
	//静的初期化
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//描画後処理
	static void PostDraw();

	//生成されたスプライト
	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 });

public://メンバ関数

	//コンストラクタ	
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color);

	//初期化	
	bool Initialize();

	//スプライト単体描画
	void Draw();

	//色を設定
	void SetColor(XMFLOAT3 color) {
		this->color.x = color.x;
		this->color.y = color.y;
		this->color.z = color.z;
	}

	//アルファ値を設定
	void SetAlpha(float blackOutAlpha) { color.w = blackOutAlpha; }

	//サイズを変更
	void SetSize(XMFLOAT2 size);

	//スケールを変更
	void SetScale(XMFLOAT2 scale);

	//場所を変更
	void SetPosition(XMFLOAT2 pos) { position = pos; }

	//回転
	void SetRotation(float rota) { rotation = rota; }

	XMFLOAT2 GetSize() { return size; }
	XMFLOAT2 GetPosition() { return position; }

	void SetAnchorPoint(XMFLOAT2 anchorpoint) { this->anchorpoint = anchorpoint; }

protected:
	//頂点データ転送
	void TransferVertices();

protected://メンバ変数
	ComPtr<ID3D12Resource>vertBuff;		//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//頂点バッファビュー
	ComPtr<ID3D12Resource> constBuff;//定数バッファ
	//Z軸回りの回転角
	float rotation = 0.0f;
	//座標
	XMFLOAT2 position = { 0,0 };
	//大きさ
	XMFLOAT2 size = { 100,100 };
	//大きさの倍率
	XMFLOAT2 scale = { 1.0f,1.0f };
	//ワールド行列
	XMMATRIX matWorld;
	//色(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };
	//テクスチャ番号
	UINT texNumber = 0;

	//アンカーポイント
	XMFLOAT2 anchorpoint = { 0.0f,0.0f };
};