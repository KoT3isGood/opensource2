//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#include "entitysystem.h"
#include "baseentity.h"
#include "stddef.h"
#include "string.h"
#include "stdlib.h"


CEntitySystem *EntitySystem()
{
	static CEntitySystem s_entitySystem;
	return &s_entitySystem;
}

static struct EntityRegistry_t
{
	IEntityFactory *m_pFactory;
	const char *m_szClassName;
	struct EntityRegistry_t *m_pNext;
} *s_pEntitiesRegistry = NULL;

CEntitySystem::CEntitySystem()
{
	int i = 0;
	for ( i = 0; i < MAX_EDICTS; i++ )
	{
		m_pEntities[i] = NULL;
	}
	m_nEntityCount = 0;
}

void CEntitySystem::RegisterEntityClass( IEntityFactory *pEntityFactory, const char *szClassName )
{
	IEntityFactory *pFactory;
	EntityRegistry_t *pRegistry;

	pFactory = GetFactoryByClassname(szClassName);

	if ( pFactory != NULL )
	{
		// Already registered
		return;
	}
	
	pRegistry = new EntityRegistry_t;
	pRegistry->m_pFactory = pEntityFactory;
	pRegistry->m_pNext = s_pEntitiesRegistry;
	pRegistry->m_szClassName = szClassName;
	s_pEntitiesRegistry = pRegistry; 
}

CBaseEntity *CEntitySystem::CreateByClassname( const char *szName )
{
	IEntityFactory *pFactory;
	CBaseEntity *pEntity;
	int i;
	int iSelectedSlot;

	pFactory = GetFactoryByClassname(szName);
	if ( !pFactory )
		return NULL;

	// We do not want to have more than MAX_EDICT entities
	if ( m_nEntityCount >= MAX_EDICTS-1 )
		return NULL;

	// Search for space
	// Could be more efficient but nobody cares
	for ( i = 0; i < MAX_EDICTS; i++ )
	{
		if ( m_pEntities[i] == NULL )
		{
			iSelectedSlot = i;
			break;
		}
	}

	pEntity = pFactory->Create();
	m_pEntities[iSelectedSlot] = pEntity;
	m_nEntityCount++;
	return pEntity;
}

IEntityFactory *CEntitySystem::GetFactoryByClassname( const char *szName )
{
	EntityRegistry_t *pEntity;

	for ( pEntity = s_pEntitiesRegistry; pEntity; pEntity = pEntity->m_pNext )
	{
		if (!strcmp(szName, pEntity->m_szClassName))
		{
			return pEntity->m_pFactory;
		}
	}
	return NULL;
}


void CEntitySystem::Think()
{
	CBaseEntity *pEntity;
	int i;

	for ( i = 0; i < MAX_EDICTS; i++ )
	{
		pEntity = m_pEntities[i];
		if ( pEntity == NULL )
			continue;

		if ( !pEntity->m_pfnThink )
			continue;

		pEntity->m_pfnThink();
	}
}
