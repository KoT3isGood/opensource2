#ifndef ENV_CUBEMAP_H
#define ENV_CUBEMAP_H

#include "pointentity.h"
#include "datamap.h"
#include "engine/engine2.h"
class CEnvCubemap : public CPointEntity
{
public:
	DECLARE_CLASS( CPointEntity, CPointEntity )
	DECLARE_DATADESC()
	
	CEnvMapSceneObject *m_pSceneCubemap;
};

#endif
