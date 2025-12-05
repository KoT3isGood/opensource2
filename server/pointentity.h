#ifndef POINT_ENTITY_H
#define POINT_ENTITY_H

#include "baseentity.h"
#include "datamap.h"
class CPointEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CPointEntity, CBaseEntity )
	DECLARE_DATADESC()
};

#endif
