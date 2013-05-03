#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class BoundingBox
{
public:
	BoundingBox(D3DXVECTOR3,D3DXVECTOR3);
	~BoundingBox();
	bool isPointInside(D3DXVECTOR3&);

private:
		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
};

#endif