//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#include "basemodelentity.h"
#include "engine/structs.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "maploader.h"

#define TEST_MODEL_NAME "models/dev/error.vmdl"

void CBaseModelEntity::Spawn()
{
	SetAbsOrigin({200,0,0});
	SetModel(TEST_MODEL_NAME);
}

void CBaseModelEntity::SetModel( const char *szModelName )
{
	m_pModelhandle = Glue::Resources::GetModel(szModelName);
	m_pSceneObject = (CSceneObject*)AcquireNextHandle(g_pMeshSystem->CreateSceneObject(m_pModelhandle, m_transform, NULL, 
				uint64_t(SceneObjectFlags::CastShadows) | 
				uint64_t(SceneObjectFlags::IsLoaded),
				0, GN(MapLoader()->GetMainWorld()), 1));
	m_pSceneObject->SetTintRGBA((Vector4D){1,1,1,1});
}
void CBaseModelEntity::SetAbsAngles( float fPitch, float fYaw, float fRoll )
{
	BaseClass::SetAbsAngles(fPitch, fYaw, fRoll);
}

void CBaseModelEntity::SetAbsOrigin( Vector origin )
{
	BaseClass::SetAbsOrigin(origin);
}

void CBaseModelEntity::SetScale( float fScale )
{
	BaseClass::SetScale(fScale);
}

void CBaseModelEntity::UpdateTransform()
{
	if (m_pSceneObject)
		m_pSceneObject->SetTransform(m_transform);
}

LINK_ENTITY_TO_CLASS(basemodelentity, CBaseModelEntity);
