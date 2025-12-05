//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#ifndef ENTITIES_H
#define ENTITIES_H

class IEntityFactory;
class CBaseEntity;

#define MAX_EDICTS 8192

class CEntitySystem
{
public:
	CEntitySystem();

	virtual void RegisterEntityClass( IEntityFactory *pEntityFactory, const char *szClassName );
	virtual CBaseEntity *CreateByClassname( const char *szName );

	virtual IEntityFactory *GetFactoryByClassname( const char *szName );

	virtual void Think();
private:
	CBaseEntity *m_pEntities[MAX_EDICTS];
	int m_nEntityCount;
};

CEntitySystem *EntitySystem();

#endif 
