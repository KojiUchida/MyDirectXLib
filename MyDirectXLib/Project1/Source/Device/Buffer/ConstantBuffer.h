#pragma once
struct ID3D11Device;
struct ID3D11Buffer;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void init(ID3D11Device* pDevice, unsigned int size, void* pInitData);

	ID3D11Buffer* getBuffer() const;

private:
	ID3D11Buffer* m_pBuffer;
};