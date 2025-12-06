//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#include "baseentity.h"
#include "datamap.h"
#include "engine/engine2.h"
#include "engine/structs.h"


CBaseEntity::~CBaseEntity()
{

}

void CBaseEntity::Spawn()
{

}

void CBaseEntity::SetAbsAngles( float fPitch, float fYaw, float fRoll )
{
	m_transform.m_rotation.FromQAngle((QAngle){fPitch, fYaw, fRoll});
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
typedescription_t *CBaseEntity::FindDataByName( const char *szName )
{
	datamap_t *pDataMap;
	int i;

	pDataMap = GetDataMap();

lookforname:
	for ( i = 0; i < pDataMap->m_iNumFields; i++ )
	{
		if (!strcmp(pDataMap->m_pData[i].m_szFieldName, szName))
			return &pDataMap->m_pData[i];
	}
	pDataMap = pDataMap->m_pBase;

	if (pDataMap)
		goto lookforname;

	return NULL;
};

typedescription_t *CBaseEntity::FindDataByMapName( const char *szName )
{
	datamap_t *pDataMap;
	int i;

	pDataMap = GetDataMap();

lookforname:
	for ( i = 0; i < pDataMap->m_iNumFields; i++ )
	{
		if (!strcmp(pDataMap->m_pData[i].m_szEditorName, szName))
			return &pDataMap->m_pData[i];
	}
	pDataMap = pDataMap->m_pBase;

	if (pDataMap)
		goto lookforname;

	return NULL;
};


BEGIN_DATADESC_NOBASE(CBaseEntity)
	DEFINE_KEYFIELD(m_transform.m_position, FIELD_VECTOR, "origin")
	DEFINE_KEYFIELD(m_transform.m_rotation, FIELD_QUATERNION_QANGLE, "angles")
	DEFINE_KEYFIELD(m_transform.m_scale, FIELD_VECTOR, "scales")
END_DATADESC()
