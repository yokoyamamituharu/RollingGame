#pragma once

#include "CollisionPrimitive.h"

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
public:
	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR*inter = nullptr, DirectX::XMVECTOR * reject = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR*inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR * reject = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& lay, const Plane& plane, float*distance = nullptr, DirectX::XMVECTOR*inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float*distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float*distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 立方体と立方体の当たり判定
	/// </summary>
	/// <param name="pos1">立方体1</param>
	/// <param name="radius1">立方体1の各軸の半径</param>
	/// <param name="pos2">立方体2</param>
	/// <param name="radius2">立方体2の各軸の半径</param>
	/// <returns>当たっているか否か</returns>
	static bool CheckBox2Box(const DirectX::XMFLOAT3 pos1, const DirectX::XMFLOAT3 radius1, const DirectX::XMFLOAT3 pos2, const DirectX::XMFLOAT3 radius2);

	/// <summary>
	///	二点間の距離を計算
	/// </summary>
	/// <param name="pos1">点1</param>
	/// <param name="pos2">点2</param>
	/// <returns>二点の距離</returns>
	static float CheckDistance(const DirectX::XMFLOAT3 pos1, const DirectX::XMFLOAT3 pos2);

	static float CheckDistance(const DirectX::XMFLOAT2 pos1, const DirectX::XMFLOAT2 pos2);

	static bool CheckExceed(const DirectX::XMFLOAT3 pos1, const DirectX::XMFLOAT3 pos2,const DirectX::XMFLOAT3 pos3);
};