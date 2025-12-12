#include "baseentity.h"
#include "datamap.h"
#include "engine/engine2.h"
#include "engine/structs.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "lightentity.h"
#include "maploader.h"

class CLightDirectionalEntity : public CLightEntity
{
public:
	DECLARE_CLASS(CLightDirectionalEntity, CLightEntity)
	DECLARE_DATADESC()

	virtual void Spawn() override;

};

LINK_ENTITY_TO_CLASS(light_environment, CLightDirectionalEntity)

void CLightDirectionalEntity::Spawn()
{
	m_transform.m_rotation.FromQAngle({0,0,180});
	Vector direction = m_transform.m_rotation.GetForwardAxis();

	m_pLightObject = (CSceneLightObject*)AcquireNextHandle(g_pSceneSystem->CreateDirectionalLight(GN(MapLoader()->GetMainWorld()), direction));
	m_pLightObject->AddTag(StringToken("light_directional"));

	BaseClass::Spawn();
}

BEGIN_DATADESC(CLightDirectionalEntity)
END_DATADESC()
