#pragma once

#include "GL.h"
#include "CVector.h"
#include "AL/alc.h"
#include "AL/al.h"
#include <map>
#include <string>
class CCamera;
struct SSoundParam {
	float		vol;
	float		pan;
	float		pitch;
	bool		is3D;
	float		dist;
	float		rolloff_factor;
	float		reference_distance;
	SSoundParam() {
		vol = 1.0f;
		pan = 0.0f;
		pitch = 1.0f;
		is3D = false;
		dist = FLT_MAX;
		rolloff_factor = 1.0f;
		reference_distance = 1.0f;
	}
};
class CSoundBase {
public:
	ALuint *m_source;
	ALuint m_buffer;
	ALuint m_current;
	int m_layer_max;
	int m_channels;
	int m_sample_rate;
	SSoundParam m_param;
public:
	CSoundBase();
	~CSoundBase();
	/*!
	@brief	�T�E���h�ǂݍ���(wav)(3D�Ή��̏ꍇ�̓��m��������)
	@param	path	[in]	�t�@�C����
	@param	layer	[in]	�����Đ����i�S�����ōő�128�܂Łj
	@param	is3D	[in]	3D�Ή�(�v�F���X�i�[�X�V)
	**/
	bool Load(const char *path, int layer=1,bool is3D=false);

	bool Create(const void* data, ALsizei size, ALenum  format, ALsizei freq);
	/*!
	@brief	�T�E���h�Đ�
	@param	boLoop	[in]	���[�v�t���O
	@return	�n���h��ID
	**/
	int Play(bool boLoop=false);
	/*!
	@brief	�T�E���h�ĊJ
	@param	boLoop	[in]	���[�v�t���O
	@param	layer	[in]	�n���h��ID
	**/
	void Resume(bool boLoop=false,int layer=0);

	/*!
	@brief	�T�E���h�ĊJ
	@param	pos			[in]	�������W	
	@param	velocity	[in]	���x
	@param	boLoop		[in]	���[�v�t���O
	**/
	int Play3D(const CVector3D& pos,const CVector3D& velocity, bool boLoop = false);
	/*!
	@brief	�T�E���h���W�Đݒ�
	@param	pos			[in]	�������W
	@param	layer		[in]	�n���h��ID
	**/
	void SetPos(CVector3D pos, int layer = -1);
	/*!
	@brief	�T�E���h�����ݒ�
	@param	dist			[in]	����
	**/
	void SetMaxDistance(float dist) { m_param.dist = dist; }
	/*!
	@brief	�T�E���h��~
	@param	layer			[in]	�n���h��ID(-1�őS���C���[��~)
	**/
	void Stop(int layer=-1);
	/*!
	@brief	�T�E���h�ꎞ��~
	@param	layer			[in]	�n���h��ID(-1�őS���C���[��~)
	**/
	void Pause(int layer=-1);
	/*!
	@brief	�T�E���h�j��
	**/	void Release();

	/*!
	@brief	���ʂ̐ݒ�
	@param	volumes			[in] ����(�y�ŏ��z0.0�`1.0�y�f�t�H���g�z) 
	**/
	void Volume(float volumes, int layer=-1);
	/*!
	@brief	�Đ����x�̐ݒ�
	@param	rato			[in] ���x(�y�x���z0.0�`1.0�y�f�t�H���g�z�`�y�����z)
	**/
	void Pitch(float ratio, int layer=-1);
	/*!
	@brief	�p���̐ݒ�
	@param	pan			[in] �p��(�y���z-1.0�`0.0�y���S�z�`1.0�y�E�z)
	**/
	void Pan(float pan, int layer = -1);
	/*!
	@brief	���������̐ݒ�
	@param	max_distance		[in] �����l
	@param	reference_distance	[in] �����l
	@param	rolloff_factor		[in] �����l
	**/
	void SetDistance(float max_distance, float reference_distance, float rolloff_factor, int layer = -1);

	/*!
	@brief	�Đ��I������
	@param	layer			[in]	�n���h��ID
	@return	true�Đ��I��
	**/
	bool CheckEnd(int layer = 0);

	/*!
	@brief	�Đ��ʒu���擾
	@retval	�Đ��ʒu�i�b�j
	**/
	float GetOffset(int layer = 0);
};
class CSound {
	std::map<std::string, CSoundBase*> m_list;
	static CSound *m_instance;
	friend class CSoundBase;
public:
	CSound();
	~CSound();
	/*!
	@brief	�T�E���h�}�l�[�W���[�̎擾
	**/
	static CSound* GetInstance();
	/*!
	@brief	�T�E���h�}�l�[�W���[�̔j��
	**/
	static void ClearInstance();
	/*!
	@brief	�T�E���h�I�u�W�F�N�g�̎擾
	@param	name			[in]���ʖ�
	**/
	CSoundBase* GetSound(const char* name);
	/*!
	@brief	�T�E���h�I�u�W�F�N�g�̔j��
	@param	name			[in]���ʖ�
	**/
	void ReleaseSound(const char *name);
	/*!
	@brief	���X�i�[�̍X�V
	**/
	void UpdateListener();

	
	void StopAll();
	
};

#define SOUND(name)	CSound::GetInstance()->GetSound(name)
