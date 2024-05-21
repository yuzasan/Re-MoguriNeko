#pragma once


#include "GL.h"
#include <map>
#include <string>

class CFragShaderCallback {
public:
	virtual void SendFragParam() = 0;
};
class CShader {
public:
	enum {
		eVertexLocation,
		eNormalLocation,
		eTexcoordlLocation,
		eWeightsLocation,
		eIndicesLocation,
		eVertexColorLocation,
	};

private:
	GLuint vertexShader;
	GLuint fragShader;

	GLuint program;
	GLint linked;
	static std::map<std::string, CShader*> m_instances;


	/*!
	@brief	�R�[�h����V�F�[�_�v���O�������쐬
	@param program	[in]	�V�F�[�_�[�v���O����
	@param shader	[in]	�V�F�[�_�[�I�u�W�F�N�g�@glCreateShader(GL_VERTEX_SHADER),glCreateShader(GL_FRAGMENT_SHADER)
	@param file		[in]	�V�F�[�_�[�t�@�C����
	@retval	����
	**/
	bool Load(GLuint shader, const char* file);
	/*!
	@brief	�R�[�h����V�F�[�_�v���O�������쐬
	@param program	[in]	�V�F�[�_�[�v���O����
	@param shader	[in]	�V�F�[�_�[�I�u�W�F�N�g�@glCreateShader(GL_VERTEX_SHADER),glCreateShader(GL_FRAGMENT_SHADER)
	@param code		[in]	�V�F�[�_�[�\�[�X�R�[�h
	@retval	����
	**/
	bool ReadCode(GLuint shader, const char* code);

public:

	/*!
		@brief	�g�ݍ��ݍς݃V�F�[�_�[�̎擾
		@param type [in] �g�ݍ��݂̎��
				"SkinMesh"�c�X�L�����b�V���p(fbx,x,a3m)
				"StaticMesh"�c�X�^�e�B�b�N�p(obj,field)
				"Image"�c�摜�p
				"Solid"�c��{�}�`�p
				"Trail"�c�O�՗p
		@retval	�V�F�[�_�[�C���X�^���X
	**/
	static CShader* GetInstance(std::string type);

	/*!
		@brief	�S�ẴV�F�[�_�[��j��
	**/
	static void ClearInstance();
	/*!
		@brief	�g�ݍ��ݍς݃V�F�[�_�[�̐���
		@param type [in] �g�ݍ��݂̎��
		@retval	�V�F�[�_�[�C���X�^���X
	**/
	static CShader* CreateInstance(std::string type);
	/*!
	@brief	�R���X�g���N�^
			�V�F�[�_�I�u�W�F�N�g�̐����̂�
	**/
	CShader();
	/*!
	@brief	�R���X�g���N�^
			�g�ݍ��݃V�F�[�_�[�Ő���
	@param type [in] �g�ݍ��݂̎��
	**/
	CShader(std::string type);
	/*!
	@brief	�R���X�g���N�^
			�V�F�[�_�[�t�@�C�����琶��
	@param vertexPath	[in]	���_�V�F�[�_�̃t�@�C����
	@param fragPath	[in]		�t���O�����g�V�F�[�_�̃t�@�C����
	**/
	CShader(const char* vertexPath,const char*  fragPath);
	/*!
	@brief	�f�X�g���N�^
	**/
	~CShader();

	/*!
	@brief	���_�V�F�[�_�[�ɑg�ݍ��݃V�F�[�_�[��ǂݍ���
	@param type [in] �g�ݍ��݂̎��
	**/
	void ReadVertexShader(std::string type);


	/*!
	@brief	�t�@�C������V�F�[�_�v���O�������쐬
	@param vertexPath	[in]	���_�V�F�[�_�̃t�@�C����
	@param fragPath	[in]		�t���O�����g�V�F�[�_�̃t�@�C����
	@retval	����
	**/
	bool Load(const char* vertexPath,const char*  fragPath);
	/*!
	@brief	�t�@�C������V�F�[�_�v���O�������X�ɍ쐬
	@param	path	[in]	�V�F�[�_�[�̃t�@�C����
	@param	type	[in]	�V�F�[�_�[�I�u�W�F�N�g�̎��(GL_VERTEX_SHADER,GL_FRAGMENT_SHADER)
	@retval	����
	**/
	bool Load(const char* path, GLuint type);
	/*!
	@brief	�\�[�X�R�[�h����V�F�[�_�v���O�������X�ɍ쐬
	@param	code	[in]	�V�F�[�_�[�̃\�[�X�R�[�h
	@param	type	[in]	�V�F�[�_�[�I�u�W�F�N�g�̎��(GL_VERTEX_SHADER,GL_FRAGMENT_SHADER)
	@retval	����
	**/
	bool ReadCode(const char* path,GLuint type);

	/*!
	@brief	�\�[�X�R�[�h����V�F�[�_�v���O�������쐬
	@param vertexcode	[in]	���_�V�F�[�_�̃\�[�X�R�[�h
	@param fragcode		[in]	�t���O�����g�V�F�[�_�̃\�[�X�R�[�h
	@retval	����
	**/
	bool ReadCode(const char* vertexcode, const char* fragcode);

	/*!
	@brief	�t���O�����g�V�F�[�_�[�ւ̃f�[�^�]��
	@param	s	[in]	CFragShaderCallback���p�������I�u�W�F�N�g
	@retval	����
	**/
	void SendFragParam(CFragShaderCallback* s) {
		s->SendFragParam();
	}
	/*!
	@brief	�V�F�[�_�[�L����
	@retval	����
	**/
	void Enable();
	/*!
	@brief	�V�F�[�_�[������
	@retval	����
	**/
	void Disable();
	/*!
	@brief	�V�F�[�_�[�v���O�����̎擾
	@retval	�V�F�[�_�[�v���O����
	**/
	GLuint GetProgram() const{
		return program;
	}


};