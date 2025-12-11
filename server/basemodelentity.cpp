//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#include "basemodelentity.h"
#include "engine/structs.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "maploader.h"

#define TEST_MODEL_NAME "models/dev/box.vmdl"

void CBaseModelEntity::Spawn()
{
	
	SetAbsOrigin({200,0,25});
	SetScale(1);
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
	m_pSceneObject->SetFlags((uint64_t)SceneObjectFlags::CastShadows);

	HMaterialStrong* pMaterialHandle = Glue::Resources::GetMaterial("materials/dev/reflectivity_30.vmat");
	m_pSceneObject->SetMaterialOverrideForMeshInstances(pMaterialHandle);
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
