#include "basemodelentity.h"
#include "entitysystem.h"
#include "maploader.h"

void ServerInit()
{
	MapLoader()->LoadMap("maps/dev/preview_flat.vpk");
	CBaseModelEntity *pEntity;
	pEntity = (CBaseModelEntity*)EntitySystem()->CreateByClassname("basemodelentity");
	pEntity->Spawn();
}

void ServerFrame()
{
	EntitySystem()->Think();
}
