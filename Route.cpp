#include "Route.h"

DirectX::XMFLOAT2 Route::route[5][3];

Route* Route::GetIns()
{
	static Route ins;
	return &ins;
}

void Route::Set()
{
	for (int i = 0; i < 3; i++) {
		route[0][i] = {0,0};
	}

	route[1][0] = { 100,100 };
	route[1][1] = { 50,0 };
	route[1][2] = { 0,0 };

	route[2][0] = { -100,-100 };
	route[2][1] = { -100,0 };
	route[2][2] = { 0,0 };

	route[3][0] = { 350,-340 };
	route[3][1] = { 450,-390 };
	route[3][2] = { 480,-390 };

	route[4][0] = { -794,-64 };
	route[4][1] = { -764,-64 };
	route[4][2] = { 450,-390 };
}

DirectX::XMFLOAT2* Route::GetRoute(const int index)
{
	return route[index];
}
