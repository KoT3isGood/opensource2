#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "engine/structs.h"

#define DECLARE_CLASS_NOBASE( className ) \
	typedef className ThisClass;
#define DECLARE_CLASS( className, baseName ) \
	typedef baseName BaseClass; \
	typedef className ThisClass;

#define LINK_ENTITY_TO_CLASS( mapClassName, DLLClassName) \
	static CEntityFactory<DLLClassName> mapClassName( #mapClassName );

class CBaseEntity;

class IEntityFactory
{
public:
	virtual CBaseEntity *Create() = 0;
};

void InstallEntityFactory( IEntityFactory *pFactory, const char *szClassName );

template<class T>
class CEntityFactory : public IEntityFactory
{
	CEntityFactory( const char *szClassName )
	{
		InstallEntityFactory(this, szClassName);
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

	virtual ~CBaseEntity();

	virtual void SetAbsAngles( float fPitch, float fYaw, float fRoll );
	virtual void SetAbsOrigin( Vector origin );
	virtual void SetScale( float fScale );

	virtual QAngle GetAbsAngles( void );
	virtual Vector GetAbsOrigin( void );
	virtual float GetScale( void );

	virtual void SetThink( fnThink pfnThink );
private:
	struct ThinkFunction_t
	{
		struct ThinkFunction_t *m_pNext;
		fnThink m_pfnThink;
		float m_fNextTime;
	} *g_pThinkFunctions;
};

#endif
