#include "baseentity.h"
#include "datamap.h"
#include "engine/engine2.h"
#include "engine/structs.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "engine/stringtoken.h"
#include "lightentity.h"
#include "maploader.h"

class CDirectionalLight : public CLightEntity
{
public:
	DECLARE_CLASS(CDirectionalLight, CLightEntity)
	DECLARE_DATADESC()

	virtual void Spawn() override;

};

LINK_ENTITY_TO_CLASS(light_environment, CDirectionalLight)

void CDirectionalLight::Spawn()
{
	Vector direction = m_transform.m_rotation.GetForwardAxis();
	direction.x *= -1;


	m_pLightObject = (CSceneLightObject*)AcquireNextHandle(g_pSceneSystem->CreateDirectionalLight(GN(MapLoader()->GetMainWorld()), direction));
	m_pLightObject->SetColor((Vector){m_lightColor.x, m_lightColor.y, m_lightColor.z});
	m_pLightObject->AddTag(StringToken("light_directional"));

	BaseClass::Spawn();
}

BEGIN_DATADESC(CDirectionalLight)
END_DATADESC()
