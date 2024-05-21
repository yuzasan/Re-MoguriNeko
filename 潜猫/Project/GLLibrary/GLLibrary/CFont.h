#pragma once
#include "GL.h"
#include <map>
#include <string>
#include <FTGL/ftgl.h>

#ifdef _DEBUG
#pragma comment(lib,"ftgl_static_D.lib")
#pragma comment(lib,"freetyped.lib")
#else
#pragma comment(lib,"ftgl_static.lib")
#pragma comment(lib,"freetype.lib")
#endif
class CFont {
private:
	FTPixmapFont* m_font;
	int m_size;
	static std::map<std::string, CFont*> m_instances;
public:
	/*!
	@brief	�R���X�g���N�^
	@param	font			[in]	�t�H���g
	@param	size			[in]	�T�C�Y
	**/
	CFont(const char *font_path = "C:\\Windows\\Fonts\\msgothic.ttc", int size=24);
	/*!
	@brief	�����̕\��
	@param	x				[in]	���ʒu
	@param	y				[in]	�c�ʒu
	@param	r				[in]	�F�i�ԁj
	@param	g				[in]	�F�i�΁j
	@param	b				[in]	�F�i�j
	@param	format			[in]	������A����
	**/
	void Draw(int x,int y,float r,float g,float b, const char *format, ...);
	void Draw(int x, int y, float r, float g, float b, const wchar_t *format, ...);

	/*!
	@brief	���
	**/
	void Release();

	//�ǉ�
	//�t�H���g�T�C�Y�擾
	int GetFontSize() const;

	static CFont* GetInstance(std::string name);
	static void CreateInstance(std::string name, const char* font = "�l�r�S�V�b�N", int size = 24);
};

#define FONT_T()	CFont::GetInstance("default")
#define FONT(name)	CFont::GetInstance(name)
#define CREATE_FONT(name)	CFont::CreateInstance(name)
#define CREATE_FONT_F(name,font,size)	CFont::CreateInstance(name,font,size)