#include "basemodelentity.h"
#include "entitysystem.h"
#include "maploader.h"
#include "icvar.h"

void ServerInit()
{
	MapLoader()->LoadMap("maps/dev/preview_flat.vpk");
	CBaseModelEntity *pEntity;
	pEntity = (CBaseModelEntity*)EntitySystem()->CreateByClassname("basemodelentity");
	pEntity->Spawn();
}

void ServerFrame()
{
	Console()->AddCommand("r_light_probe_volume_debug_grid 1");
	Console()->Execute();
	EntitySystem()->Think();
}
