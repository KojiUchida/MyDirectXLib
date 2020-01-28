#include "MeshRenderer.h"
#include <d3d11.h>
#include <DirectXMath.h>

#include "Actor\GameObject.h"

#include "Device\Resource\Mesh.h"
#include "Device\Resource\MeshManager.h"
#include "Device\Resource\Shader\VertexShader.h"
#include "Device\Resource\Shader\PixelShader.h"
#include "Device\Resource\Shader\ShaderManager.h"

#include "Device\Base\MeshVertex.h"

#include "Device\Buffer\VertexBuffer.h"
#include "Device\Buffer\IndexBuffer.h"
#include "Device\Buffer\ConstantBuffer.h"
#include "Device\Buffer\WVPConstantBuffer.h"
#include "Device\Buffer\MeshPSBuffer.h"

#include "Device\DirectXManager.h"
#include "Device\GameDevice.h"
#include "Device\Renderer.h"
#include "Device\Camera.h"

#include "Utility\Color.h"

#include <DirectXColors.h>

using namespace DirectX;

MeshRenderer::MeshRenderer(GameObject * pUser, int drawOrder)
	: AbstractComponent(pUser),
	m_DrawOrder(drawOrder),
	m_pMesh(nullptr),
	m_pColor(new Color(DirectX::Colors::White))
{
	//Renderer�ɓo�^
	GameDevice::getRenderer()->addMesh(this);

	m_pVertexShader = ShaderManager::GetVertexShader("MeshVS");
	m_pPixelShader = ShaderManager::GetPixelShader("MeshPS");
}

MeshRenderer::~MeshRenderer()
{
	//Renderer����o�^����
	GameDevice::getRenderer()->removeMesh(this);

	delete m_pColor;
}

void MeshRenderer::onStart()
{
}

void MeshRenderer::onUpdate()
{
}

void MeshRenderer::setMesh(std::string meshName)
{
	m_pMesh = GameDevice::getMeshManager()->getMesh(meshName);
}

int MeshRenderer::getDrawOrder()
{
	return m_DrawOrder;
}

void MeshRenderer::draw()
{
	if (m_pMesh == nullptr) return;
	if (!isActive()) return;

	auto pDevice = DirectXManager::getDevice();
	auto pDeviceContext = DirectXManager::getDeviceContext();

	//�ړ�
	XMMATRIX translate = XMMatrixTranslationFromVector(m_pUser->getPosition().toXMVector());
	//��]
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(m_pUser->getAngles().toXMVector());
	//�g��k��
	XMMATRIX scaling = XMMatrixScalingFromVector(m_pUser->getSize().toXMVector());
	//���[���h�s��쐬
	XMMATRIX world = scaling * rotation * translate;

	//�@���p�ϊ��s��
	XMMATRIX worldInv = XMMatrixInverse(nullptr, world);

	//�ϊ��s��̍쐬
	XMMATRIX wvp = XMMatrixTranspose(world * Camera::getViewProjMatrix3D());

	//���_�V�F�[�_�[�p�萔�o�b�t�@�쐬
	WVPConstantBuffer wvpCBuffer;
	XMStoreFloat4x4(&wvpCBuffer.wvpMatrix, wvp);
	XMStoreFloat4x4(&wvpCBuffer.world, worldInv);

	ConstantBuffer vsBuffer;
	vsBuffer.init(pDevice, sizeof(WVPConstantBuffer), &wvpCBuffer);
	auto pVSBuffer = vsBuffer.getBuffer();

	//�s�N�Z���V�F�[�_�[�p�萔�o�b�t�@�쐬
	MeshPSBuffer meshCBuffer;
	XMStoreFloat4(&meshCBuffer.color, m_pColor->toXMFLOAT4());

	ConstantBuffer psBuffer;
	psBuffer.init(pDevice, sizeof(MeshPSBuffer), &meshCBuffer);
	auto pPSBuffer = psBuffer.getBuffer();

	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;
	auto pVertexBuffer = m_pMesh->getVertexBuffer().getBuffer();
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pMesh->getIndexBuffer().getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->VSSetShader(m_pVertexShader->getShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->getShader(), NULL, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pVSBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pPSBuffer);

	//�`��
	pDeviceContext->DrawIndexed(m_pMesh->getVertexCount(), 0, 0);
}

void MeshRenderer::setColor(const Color & color)
{
	m_pColor->r = color.r;
	m_pColor->g = color.g;
	m_pColor->b = color.b;
	m_pColor->a = color.a;
}

const Color & MeshRenderer::getColor() const
{
	return *m_pColor;
}
