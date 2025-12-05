#ifndef LIGHT_ENTITY_H
#define LIGHT_ENTITY_H

#include "pointentity.h"
#include "engine/engine2.h"

class CLightEntity : public CPointEntity
{
public:
	DECLARE_CLASS(CLightEntity, CPointEntity)
	DECLARE_DATADESC()

	CREATE_NATIVE(CSceneLightObject, m_pLightObject);
};

#endif
