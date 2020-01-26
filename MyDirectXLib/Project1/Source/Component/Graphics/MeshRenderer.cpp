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

#include "Device\DirectXManager.h"
#include "Device\GameDevice.h"
#include "Device\Renderer.h"
#include "Device\Camera.h"

using namespace DirectX;

MeshRenderer::MeshRenderer(GameObject * pUser, int drawOrder)
	: AbstractComponent(pUser),
	m_DrawOrder(drawOrder),
	m_pMesh(nullptr)
{
	//Rendererに登録
	GameDevice::getRenderer()->addMesh(this);

	m_pVertexShader = ShaderManager::GetVertexShader("MeshVS");
	m_pPixelShader = ShaderManager::GetPixelShader("MeshPS");
}

MeshRenderer::~MeshRenderer()
{
	//Rendererから登録解除
	GameDevice::getRenderer()->removeMesh(this);
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

	//移動
	XMMATRIX translate = XMMatrixTranslationFromVector(m_pUser->getPosition().toXMVector());
	//回転
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(m_pUser->getAngles().toXMVector());
	//拡大縮小
	XMMATRIX scaling = XMMatrixScalingFromVector(m_pUser->getSize().toXMVector());
	//ワールド行列作成
	XMMATRIX world = scaling * rotation * translate;

	//変換行列の作成
	XMMATRIX wvp = XMMatrixTranspose(world * Camera::getViewProjMatrix3D());

	//定数バッファ作成
	WVPConstantBuffer wvpCBuffer;
	XMStoreFloat4x4(&wvpCBuffer.wvpMatrix, wvp);

	ConstantBuffer cBuffer;
	cBuffer.init(pDevice, sizeof(WVPConstantBuffer), &wvpCBuffer);

	auto pWVPCBuffer = cBuffer.getBuffer();

	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;
	auto pVertexBuffer = m_pMesh->getVertexBuffer().getBuffer();
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pMesh->getIndexBuffer().getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->VSSetShader(m_pVertexShader->getShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->getShader(), NULL, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pWVPCBuffer);

	//描画
	pDeviceContext->DrawIndexed(m_pMesh->getVertexCount(), 0, 0);
}
