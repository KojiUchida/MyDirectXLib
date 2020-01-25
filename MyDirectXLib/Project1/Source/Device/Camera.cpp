#include "Camera.h"
#include "Def\Screen.h"
#include <DirectXMath.h>

Vec3 Camera::position = Vec3::zero();
DirectX::XMMATRIX Camera::viewMatrix2D;
DirectX::XMMATRIX Camera::viewMatrix3D;
DirectX::XMMATRIX Camera::projMatrix;
DirectX::XMMATRIX Camera::orthoMatrix;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::init()
{
	//2D�p
	//�J�����̍��W
	DirectX::XMVECTOR vEyePt = { position.x, position.y, -10.0f, 0 };
	//�J������������W
	DirectX::XMVECTOR vLookatPt = DirectX::XMVectorSet(position.x, position.y, 0.0f, 0.0f);
	//�J�����̉�]�̎�
	DirectX::XMVECTOR vUpVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//����s��
	viewMatrix2D = DirectX::XMMatrixLookAtLH(vEyePt, vLookatPt, vUpVec);

	//���s���e�̍s��
	orthoMatrix = DirectX::XMMatrixOrthographicLH(Screen::getWindowWidth(), Screen::getWindowHeight(), 1.0f / (100.0f - 0.3f), 100.0f);

	//3D�p
	//�J�����̍��W
	DirectX::XMVECTOR vEyePt = {position.x, position.y, position.z, 0};
	//�J������������W
	DirectX::XMVECTOR vLookatPt = { 0.0f, 0.0f, 0.0f, 0.0f };
	//�J�����̉�]�̎�
	DirectX::XMVECTOR vUpVec = { 0.0f, 0.0f, 0.0f, 0.0f };
	//����s��
	DirectX::XMMATRIX viewMatrix3D = DirectX::XMMatrixLookAtLH(vEyePt, vLookatPt, vUpVec);

	//����p
	const float fov = 60.0f;
	//�A�X�y�N�g��
	const float aspect = Screen::getWindowWidth() / Screen::getWindowHeight();

	//�������e�̍s��
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, 1.0f / (100.0f - 0.3f), 100.0f);
}

void Camera::update()
{
	//2D�p
	//�J�����̍��W
	DirectX::XMVECTOR vEyePt = { position.x, position.y, -10.0f, 0 };
	//�J������������W
	DirectX::XMVECTOR vLookatPt = DirectX::XMVectorSet(position.x, position.y, 0.0f, 0.0f);
	//�J�����̉�]�̎�
	DirectX::XMVECTOR vUpVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//����s��
	viewMatrix2D = DirectX::XMMatrixLookAtLH(vEyePt, vLookatPt, vUpVec);
}

void Camera::reset()
{
	position = Vec3::zero();
}

void Camera::setPosition(Vec3 position)
{
	position = position;
}

Vec3 Camera::getPosition()
{
	return position;
}

DirectX::XMMATRIX Camera::getViewProjMatrix2D()
{
	return viewMatrix2D * orthoMatrix;
}

DirectX::XMMATRIX Camera::getViewProjMatrix3D()
{
	return viewMatrix3D * projMatrix;
}
