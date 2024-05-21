#include "CLight.h"

CVector3D	CLight::m_lightPos[LIGHT_MAX]={CVector3D(0,0,0),};
CVector3D	CLight::m_lightDir[LIGHT_MAX]={CVector3D(0,0,1),};
CVector3D	CLight::m_AmbientColor[LIGHT_MAX]={CVector3D(1,1,1),};
CVector3D	CLight::m_DiffuseColor[LIGHT_MAX]={CVector3D(1,1,1),};
int			CLight::m_Type[LIGHT_MAX]={1,};
float		CLight::m_Attenuation[LIGHT_MAX];
float		CLight::m_RadiationAngle[LIGHT_MAX];
int			CLight::m_Lighting = true;
CLight::SFog CLight::m_fog = { CVector4D(1, 1, 1, 1), 500, 1200 };
CMatrix CLight::m_shadowMatrix;
