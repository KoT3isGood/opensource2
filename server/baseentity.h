//================= Copyright kotofyt, All rights reserved ==================//
//
// Purpose:
//
//===========================================================================//

#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "engine/structs.h"
#include "entitysystem.h"
#include "stdio.h"
#include "datamap.h"

#define DECLARE_CLASS_NOBASE( className ) \
	typedef className ThisClass;
#define DECLARE_CLASS( className, baseName ) \
	typedef baseName BaseClass; \
	typedef className ThisClass;

#define LINK_ENTITY_TO_CLASS( mapClassName, DLLClassName) \
	static CEntityFactory<DLLClassName> g_EntityFactory_##mapClassName( #mapClassName );

class CBaseEntity;

class IEntityFactory
{
public:
	virtual CBaseEntity *Create() = 0;
};


template<class T>
class CEntityFactory : public IEntityFactory
{
public:
	CEntityFactory( const char *szClassName )
	{
		EntitySystem()->RegisterEntityClass(this, szClassName);
	};
	virtual CBaseEntity *Create() {
		return new T;
	}
};



typedef void(*fnThink)();
class CBaseEntity
{
public: 
	DECLARE_CLASS_NOBASE(CBaseEntity);
	DECLARE_DATADESC_NOBASE()

	virtual ~CBaseEntity();
	virtual void Spawn();

	virtual void SetAbsAngles( float fPitch, float fYaw, float fRoll );
	virtual void SetAbsOrigin( Vector origin );
	virtual void SetScale( float fScale );

	virtual QAngle GetAbsAngles( void );
	virtual Vector GetAbsOrigin( void );
	virtual float GetScale( void );

	virtual void SetThink( fnThink pfnThink );
	virtual void SetNextThink( float fThink );
	
	fnThink m_pfnThink = NULL;
	CTransformUnaligned m_transform;

};

#endif
