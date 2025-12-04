#ifndef BASEMODELENTITY_H
#define BASEMODELENTITY_H

#include "baseentity.h"
#include "engine/engine2.h"
#include "engine/structs.h"

class CBaseModelEntity : public CBaseEntity
{
public:
	DECLARE_CLASS(CBaseModelEntity, CBaseEntity)
	
	virtual void SetModel( const char *szModelName );

private:
	HModelStrong *m_pModelhandle;

	CREATE_NATIVE(CSceneObject, m_pSceneObject);
};

#endif
