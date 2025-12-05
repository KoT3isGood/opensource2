//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#ifndef BASEMODELENTITY_H
#define BASEMODELENTITY_H

#include "baseentity.h"
#include "engine/engine2.h"
#include "engine/structs.h"

class CBaseModelEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseModelEntity, CBaseEntity )

	virtual void Spawn() override;

	virtual void SetModel( const char *szModelName );
	virtual void SetAbsAngles( float fPitch, float fYaw, float fRoll ) override;
	virtual void SetAbsOrigin( Vector origin ) override;
	virtual void SetScale( float fScale ) override;

protected:
	virtual void UpdateTransform();

private:
	HModelStrong *m_pModelhandle;

	CREATE_NATIVE(CSceneObject, m_pSceneObject);
};

#endif
