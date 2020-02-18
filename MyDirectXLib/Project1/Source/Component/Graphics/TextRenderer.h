#pragma once
#include <string>
#include "Component\AbstractComponent.h"
#include "Device\Resource\FontOption.h"

struct ID2D1RenderTarget;
struct ID2D1SolidColorBrush;
struct IDWriteTextFormat;
struct IDWriteTextLayout;

struct Color;

class TextRenderer
	: public AbstractComponent
{
public:
	TextRenderer(GameObject* pUser, int drawOrder = 100);
	~TextRenderer();

	virtual void onStart() override;
	virtual void onUpdate() override;

	int getDrawOrder() { return m_DrawOrder; }
	void draw(ID2D1RenderTarget* pRenderTarget);

	void setFont(std::wstring fontName, FONT_WEIGHT fontWeight, FONT_STYLE fontStyle);	//�t�H���g���̐ݒ�
	std::wstring getFont() const;		//�t�H���g���̎擾

	void setText(std::wstring text);	//������̐ݒ�
	std::wstring getText() const;		//������̎擾

	void setColor(const Color& color);	//�����F�̐ݒ�
	const Color& getColor() const;		//�����F�̎擾

	void setTextSize(float size);		//�����T�C�Y�̐ݒ�
	float getTextSize();				//�����T�C�Y�̎擾

private:
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextLayout* m_pTextLayout;
	ID2D1SolidColorBrush* m_pBrush;

	std::wstring m_FontName;
	std::wstring m_Text;
	float m_TextSize;
	int m_DrawOrder;
};