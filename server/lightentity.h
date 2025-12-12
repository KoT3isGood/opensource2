#ifndef LIGHT_ENTITY_H
#define LIGHT_ENTITY_H

#include "engine/structs.h"
#include "pointentity.h"
#include "engine/engine2.h"

class CLightEntity : public CPointEntity
{
public:
	DECLARE_CLASS(CLightEntity, CPointEntity)
	DECLARE_DATADESC()

	CREATE_NATIVE(CSceneLightObject, m_pLightObject);

	virtual void Spawn() override;

	Vector4D m_lightColor = {0,0,0,0};
	int m_nShadows = 0;
	int m_nCascades = 0;
	HRenderTextureStrong *m_hLightCookie = NULL;
	int m_iBakeLightIndex = -1;
	float m_fShadowCascadeDistanceScale = 0;
	int m_iDirectLight = 2;
};

#endif
