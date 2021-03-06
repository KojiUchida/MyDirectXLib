#pragma once
#include "Math\Vec3.h"

namespace DirectX
{
	struct XMMATRIX;
}

class Camera
{
public:
	static void init();
	static void update();
	static void reset();

	static void setPosition(Vec3 position);
	static Vec3 getPosition();

	//���� * �ˉe�ϊ��̍s����擾(2D)
	static DirectX::XMMATRIX getViewProjMatrix2D();
	//���� * �ˉe�ϊ��̍s����擾(3D)
	static DirectX::XMMATRIX getViewProjMatrix3D();

private:
	static Vec3 position;

	static DirectX::XMMATRIX viewMatrix2D;
	static DirectX::XMMATRIX viewMatrix3D;

	static DirectX::XMMATRIX projMatrix;
	static DirectX::XMMATRIX orthoMatrix;

	Camera();
	~Camera();
};

