//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#include "baseentity.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "engine/engine2.h"

#include "cglm/euler.h"


CBaseEntity::~CBaseEntity()
{

}

void CBaseEntity::Spawn()
{

}

void CBaseEntity::SetAbsAngles( float fPitch, float fYaw, float fRoll )
{
	mat4 m;
	versor q;
	glm_euler((vec3){fPitch, fYaw, fRoll}, m);
	glm_mat4_quat(m, *(versor*)&m_transform.m_rotation);
}

void CBaseEntity::SetAbsOrigin( Vector origin )
{
	m_transform.m_position = origin;
}

void CBaseEntity::SetScale( float fScale )
{
	m_transform.m_scale.x = fScale;
	m_transform.m_scale.y = fScale;
	m_transform.m_scale.z = fScale;
}


QAngle CBaseEntity::GetAbsAngles( void )
{

}

Vector CBaseEntity::GetAbsOrigin( void )
{

}

float CBaseEntity::GetScale( void )
{

}

void CBaseEntity::SetThink( fnThink pfnThink )
{
	m_pfnThink = pfnThink;
}

void CBaseEntity::SetNextThink( float fThink )
{

}

BEGIN_DATADESC_NOBASE(CBaseEntity)

END_DATADESC()
