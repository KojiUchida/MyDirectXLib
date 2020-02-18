#include "FontManager.h"
#include "Utility\StringUtility.h"
#include <dwrite.h>

FontManager::FontManager()
{
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
}

FontManager::~FontManager()
{
	m_pDWriteFactory->Release();
}

IDWriteTextFormat * FontManager::createFont(std::wstring fontName, FONT_WEIGHT fontWeight, FONT_STYLE fontStyle)
{
	IDWriteTextFormat* pTextFormat = nullptr;

	DWRITE_FONT_STYLE style;
	switch (fontStyle)
	{
	case FONT_STYLE_NORMAL:
		style = DWRITE_FONT_STYLE_NORMAL;
		break;
	case FONT_STYLE_OBLIQUE:
		style = DWRITE_FONT_STYLE_OBLIQUE;
		break;
	case FONT_STYLE_ITALIC:
		style = DWRITE_FONT_STYLE_ITALIC;
		break;
	}

	m_pDWriteFactory->CreateTextFormat(
		fontName.c_str(),				//�t�H���g��
		NULL,
		(DWRITE_FONT_WEIGHT)fontWeight,	//�����̑���
		style,							//�Ἀ��̎w��
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"ja_jp",
		&pTextFormat
	);

	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	return pTextFormat;
}

IDWriteTextLayout * FontManager::createLayout(IDWriteTextFormat * pFont, std::wstring text, float fontSize)
{
	IDWriteTextLayout* pLayout;
	m_pDWriteFactory->CreateTextLayout(
		text.c_str(),			//wstring����WCHAR�̔z����擾
		text.size(),			//������̒���
		pFont,					//�g�p����TextLayout
		fontSize * text.size(),	//�����ꕶ�����̃T�C�Y * ������̒����ɂ���
		fontSize,				//�������ꕶ�����̃T�C�Y�ɂ���
		&pLayout				//TextLayout(����)
	);
	return pLayout;
}
