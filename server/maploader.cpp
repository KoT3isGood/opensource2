

#include "maploader.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/structs.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"

class CMapLoader : public IMapLoader
{
public:
	virtual void LoadMap( const char *szMap ) override;
	virtual ISceneWorld *GetMainWorld() override;
private:
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
	typedef ISceneWorld *ISceneWorld2;

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
}
	
ISceneWorld *CMapLoader::GetMainWorld()
{
	return m_pMainWorld;
}
