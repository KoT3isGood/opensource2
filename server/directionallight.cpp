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
	Vector direction = m_transform.m_rotation.GetForwardAxis();
	printf("dir %f %f %f\n", direction.x, direction.y, direction.z);

	m_pLightObject = (CSceneLightObject*)AcquireNextHandle(g_pSceneSystem->CreateDirectionalLight(GN(MapLoader()->GetMainWorld()), direction));
	m_pLightObject->AddTag(StringToken("light_directional"));

	BaseClass::Spawn();
	printf("dir %f %f %f\n", m_pLightObject->GetWorldDirection().x, m_pLightObject->GetWorldDirection().y, m_pLightObject->GetWorldDirection().z);
}

BEGIN_DATADESC(CLightDirectionalEntity)
END_DATADESC()
