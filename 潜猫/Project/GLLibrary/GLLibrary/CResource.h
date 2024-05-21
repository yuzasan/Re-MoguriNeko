#pragma once
#pragma once
/**
* @file		AsSet.h
* @brief	���\�[�X�Ǘ��N���X
*
* @author	shinya nagakawa
*/


#include <map>
#include <vector>
#include <string>

class CResource {
	char m_name[128];
public:
	CResource():m_name(""){}
	/**
	* @fn
	* �R���X�g���N
	* @brief �R���X�g���N�^
	* @param name [in]�@���O
	*/
	CResource(char *name) { memcpy(m_name, name, sizeof(m_name)); }
	/**
	* @fn
	* �f�X�g���N�^
	* @brief �f�X�g���N�^
	*/
	virtual ~CResource() {};
	/**
	* @fn
	* ���O�̎擾
	* @brief ���O�̎擾
	* @return ���O
	*/
	std::string GetName() const {
		return m_name;
	}
	/**
	* @fn
	* ���\�[�X�T�C�Y�擾
	* @brief ���\�[�X�T�C�Y�擾
	*/
	virtual unsigned int GetDataSize() { return 0; };

	/**
	* @fn
	* ���\�[�X�J������
	* @brief ���\�[�X�J������
	*/
	virtual void Release() {};
	friend class CResourceManager;

};
class CResourceManager {
private:
	std::map<int,std::map<std::string, CResource*>> m_list;
	static CResourceManager*	m_instance;
public:
	static CResourceManager* GetInstance();
	/**
	* @fn�@���\�[�X��S�Ĕj������
	* @brief ���\�[�X��S�Ĕj������
	*/
	static void ClearInstance();
	CResourceManager();
	~CResourceManager();
	/**
	* @fn
	* ���\�[�X�̒ǉ�
	* @brief ���\�[�X�̒ǉ�
	* @param res [in]�@�ǉ����郊�\�[�X
	* @param group [in]�@�O���[�v
	*/
	void Add(CResource* res,int group=0);
	/**
	* @fn
	* ���\�[�X�̒ǉ�
	* @brief ���\�[�X�̒ǉ�
	* @param name [in]�@���\�[�X�̖��O
	* @param res [in]�@�ǉ����郊�\�[�X
	* @param group [in]�@�O���[�v
	*/
	void Add(std::string name, CResource* res,int group=0);
	/**
	* @fn
	* ���\�[�X�̎擾
	* @brief ���\�[�X�̂̎擾
	* @param name [in]�@���\�[�X�̖��O
	* @param group [in]�@�O���[�v
	* @return ���\�[�X
	*/
	CResource* Get(std::string name, int group = 0);


	/**
	* @fn
	* ���\�[�X�����邩�`�F�b�N
	* @brief ���\�[�X�����邩�`�F�b�N
	* @param name [in]�@���\�[�X�̖��O
	* @param group [in]�@�O���[�v
	* @return true:���݂���,false:����
	*/
	bool IsSet(std::string name, int group = 0);
	/**
	* @fn
	* ���\�[�X�̍폜
	* @brief ���\�[�X�̂̍폜
	* @param group [in]�@�O���[�v
	* @param name [in]�@���\�[�X�̖��O
	*/
	void Delete(std::string name, int group = 0);
	/**
	* @fn
	* ���\�[�X�̍폜
	* @brief ���\�[�X�̂̍폜
	* @param name [in]�@���\�[�X�O���[�v
	*/
	void Delete(int group = 0);

	unsigned int GetDataSize();
};
/**
* @fn
* ���\�[�X�̒ǉ�
* @brief ���\�[�X�̒ǉ�
* @param name [in]�@���\�[�X�̖��O
* @param res [in]�@�ǉ����郊�\�[�X
*/
#define ADD_RESOURCE(name,obj) if(!CResourceManager::GetInstance()->IsSet(name)) CResourceManager::GetInstance()->Add(name,obj)
/**
* @fn
* ���f�����\�[�X�̒ǉ�
* @brief ���\�[�X�̒ǉ�
* @param name [in]�@���\�[�X�̖��O
* @param path [in]�@�t�@�C����
*/
#define CREATE_MODEL(name,path) ADD_RESOURCE(name,CModel::CreateModel(path))
/**
* @fn
* ���f�����\�[�X�̒ǉ��i�R���W�����܂ށj
* @brief ���\�[�X�̒ǉ�
* @param name	[in]�@���\�[�X�̖��O
* @param path	[in]�@�t�@�C����
* @param x		[in]�@������
* @param y		[in]�@������
* @param z		[in]�@������
*/
#define CREATE_MODEL_C(name,path,x,y,z) ADD_RESOURCE(name,CModel::CreateModel(path,x,y,z))
/**
* @fn
* �摜���\�[�X�̒ǉ�
* @brief ���\�[�X�̒ǉ�
* @param name	[in]�@���\�[�X�̖��O
* @param path	[in]�@�t�@�C����
*/
#define CREATE_IMAGE(name,path)  ADD_RESOURCE(name,CImage::CreateImage(path))
/**
* @fn
* �摜���\�[�X�̒ǉ��i�A�j���[�V�����t���j
* @brief ���\�[�X�̒ǉ�
* @param name	[in]�@���\�[�X�̖��O
* @param path	[in]�@�t�@�C����
* @param d		[in]�@�A�j���[�V�����f�[�^
* @param w		[in]�@�ꖇ�̕�
* @param h		[in]�@�ꖇ�̍���
*/
#define CREATE_ANIM_IMAGE(name,path,d,w,h) ADD_RESOURCE(name,CImage::CreateImage(path,d,w,h))

/**
* @fn
* ���\�[�X�̎擾
* @brief ���\�[�X�̎擾
* @param name	[in]�@���\�[�X�̖��O
* @param T		[in]�@�f�[�^�^
* @retrun		���\�[�X�̃A�h���X
*/
#define GET_RESOURCE(name,T) dynamic_cast<T*>(CResourceManager::GetInstance()->Get(name))

/**
* @fn
* ���\�[�X�̎擾�iCModelField�p�j
* @brief ���\�[�X�̎擾
* @param name	[in]�@���\�[�X�̖��O
* @retrun		���\�[�X�̃A�h���X
*/
#define GET_FIELD(name) dynamic_cast<CModelField*>(CResourceManager::GetInstance()->Get(name))
/**
* @fn
* ���f���̕����iCModelA3M�p�j
* @brief ���f���̕���
* @param name	[in]�@���\�[�X�̖��O
* @retrun		���\�[�X�̎���
*/
#define COPY_MODEL(name) *dynamic_cast<CModelA3M*>(CResourceManager::GetInstance()->Get(name))
/**
* @fn
* �摜�̕����iCImage�p�j
* @brief �摜�̕���
* @param name	[in]�@���\�[�X�̖��O
* @retrun		���\�[�X�̎���
*/
#define COPY_IMAGE(name) *dynamic_cast<CImage*>(CResourceManager::GetInstance()->Get(name))
/**
* @fn
* ���\�[�X�̕���
* @brief �摜�̕���
* @param name	[in]�@���\�[�X�̖��O
* @param T		[in]�@�f�[�^�^
* @retrun		���\�[�X�̎���
*/
#define COPY_RESOURCE(name,T) *dynamic_cast<T*>(CResourceManager::GetInstance()->Get(name))

/**
* @fn
* ���\�[�X�̕���(new)
* @brief �摜�̕���
* @param name	[in]�@���\�[�X�̖��O
* @param T		[in]�@�f�[�^�^
* @retrun		���\�[�X�̃A�h���X
*/
#define DUPLICATE_RESOURCE(name,T) new T(*dynamic_cast<T*>(CResourceManager::GetInstance()->Get(name)))

