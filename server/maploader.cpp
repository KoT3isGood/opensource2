//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#include "maploader.h"
#include "baseentity.h"
#include "datamap.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/structs.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "entitysystem.h"
#include "stdio.h"
#include "string.h"

class CMapLoader : public IMapLoader
{
public:
	virtual void LoadMap( const char *szMap ) override;
	virtual ISceneWorld *GetMainWorld() override;
	
	void SpawnEntities();
private:
	void SetKeyField( CBaseEntity *pEntity, typedescription_t *pTypeDesc, const char *szValue);
	
	template <typename T> 
	T *GetVarWithOffset(CBaseEntity *pEntity, typedescription_t *pTypeDesc);

	CREATE_NATIVE(ISceneWorld, m_pMainWorld);
	CREATE_NATIVE(IWorldReference, m_pMainWorldRef);
	CREATE_NATIVE(IPVS, m_pMainPVS);

	uint32_t m_pWorldGroupId;
};

static CMapLoader s_mapLoader;

IMapLoader *MapLoader()
{
	return &s_mapLoader;
}

void CMapLoader::LoadMap( const char *szMap )
{
	CTransformUnaligned transform;
	
	m_pMainWorld = (ISceneWorld*)AcquireNextHandle(g_pSceneSystem->CreateWorld(szMap));
	
	m_pWorldGroupId = StringToken(m_pMainWorld->GetWorldDebugName());
	SET_NATIVE(
		m_pMainWorldRef, 
		g_pWorldRendererMgr->CreateWorld(
			szMap,
			GN(m_pMainWorld), 
			false, 
			false, 
			true, 
			false, 
			m_pWorldGroupId, 
			transform
		)
	);
	m_pMainWorldRef->PrecacheAllWorldNodes(0x0080);

	SET_NATIVE(m_pMainPVS, g_pEnginePVSManager->BuildPvs(GN(m_pMainWorldRef)));
	m_pMainWorld->SetPVS(GN(m_pMainPVS));

	SpawnEntities();
}

template <typename T>
T *CMapLoader::GetVarWithOffset(CBaseEntity *pEntity, typedescription_t *pTypeDesc)
{
	return (T*)(((char*)pEntity)+pTypeDesc->m_iFieldOffset);
}

void CMapLoader::SetKeyField( CBaseEntity *pEntity, typedescription_t *pTypeDesc, const char *szValue)
{
	union {
		Quaternion *pQuat;
		Vector *pVector;
		Vector4D *pColor;
	};
	union {
		struct
		{
			int r;
			int g;
			int b;
			int a;
		} colorData;
		QAngle angleData;
	};
	switch (pTypeDesc->m_eFieldType)
	{
	case FIELD_INTEGER:
		sscanf(szValue, "%i", GetVarWithOffset<int>(pEntity, pTypeDesc));
		break;
	case FIELD_QUATERNION_QANGLE:
		pQuat = GetVarWithOffset<Quaternion>(pEntity, pTypeDesc);
		sscanf(szValue, "%f %f %f", &angleData.x, &angleData.y, &angleData.z);
		pQuat->FromQAngle(angleData);
		break;
	case FIELD_VECTOR:
		pVector = GetVarWithOffset<Vector>(pEntity, pTypeDesc);
		sscanf(szValue, "%f %f %f", &pVector->x, &pVector->y, &pVector->z);
		break;
	case FIELD_COLOR255:
		pColor = GetVarWithOffset<Vector4D>(pEntity, pTypeDesc);
		sscanf(szValue, "%i %i %i %i", &colorData.r, &colorData.g, &colorData.b, &colorData.a);
		pColor->x = colorData.r/255.0;
		pColor->y = colorData.g/255.0;
		pColor->z = colorData.b/255.0;
		pColor->w = colorData.a/255.0;
		break;
	default:
		break;
	}
}

void CMapLoader::SpawnEntities()
{
	int iNumEntities;
	int i;
	int j;
	CREATE_NATIVE(CEntityKeyValues, pKeyValues);
	int iKeyCount;
	uint32_t nKey;
	CBaseEntity *pEntity;
	datamap_t *pDataMap;
	typedescription_t *pTypeDesc;
	const char *szValue;
	

	iNumEntities = m_pMainWorldRef->GetEntityCount("default_ents");

	for ( i = 0; i < iNumEntities; i++ )
	{
		SET_NATIVE(pKeyValues, m_pMainWorldRef->GetEntityKeyValues("default_ents", i));

		const char *szClassName = pKeyValues->GetValueString(StringToken("classname"),NULL);
	
		printf("classname = %s\n",szClassName);

		if ( !strcmp(szClassName, "worldspawn") )
			continue;
	
		pEntity = EntitySystem()->CreateByClassname(szClassName);
		if ( !pEntity )
			continue;

		for ( pDataMap = pEntity->GetDataMap(); pDataMap; pDataMap = pDataMap->m_pBase)
		{
			for ( j = 0; j < pDataMap->m_iNumFields; j++ )
			{
				pTypeDesc = &pDataMap->m_pData[j];
				if ( !(pTypeDesc->m_iFlags & FTYPEDESC_KEY) )
					continue;
				if ( !pTypeDesc->m_szEditorName )
					continue;
				
				szValue = pKeyValues->GetValueString(StringToken(pTypeDesc->m_szEditorName), NULL);
				SetKeyField(pEntity, pTypeDesc, szValue);
			}
		}

		pEntity->Spawn();

	}
}
	
ISceneWorld *CMapLoader::GetMainWorld()
{
	return m_pMainWorld;
}
