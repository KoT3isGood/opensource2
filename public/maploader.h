#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "engine/engine2.h"

class IMapLoader
{
public:
	virtual void LoadMap( const char *szMap ) = 0;
	virtual ISceneWorld *GetMainWorld() = 0;
};

IMapLoader *MapLoader();

#endif
