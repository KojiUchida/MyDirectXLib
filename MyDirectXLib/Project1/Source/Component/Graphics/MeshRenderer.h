#pragma once
#include "Component\AbstractComponent.h"

#include <string>

struct Mesh;
class VertexShader;
class PixelShader;

//���b�V���`��p�R���|�[�l���g
class MeshRenderer :
	public AbstractComponent
{
public:
	MeshRenderer(GameObject* pUser, int drawOrder = 100);
	~MeshRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	virtual void setMesh(std::string meshName);

	int getDrawOrder();
	void draw();

private:
	Mesh* m_pMesh;
	int m_DrawOrder;

	VertexShader* m_pVertexShader;
	PixelShader* m_pPixelShader;
};