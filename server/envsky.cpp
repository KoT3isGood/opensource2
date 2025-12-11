#include "baseentity.h"
#include "datamap.h"
#include "engine/structs.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "maploader.h"
#include "pointentity.h"

class CEnvSky : public CPointEntity 
{
public:
	DECLARE_CLASS(CEnvSky, CPointEntity)
	DECLARE_DATADESC()

	virtual void Spawn() override;

	HMaterialStrong* m_hSkyMaterial;
	Vector4D m_vTintColor;

	CREATE_NATIVE(CSceneSkyBoxObject, m_pSceneSkyBox);
};

LINK_ENTITY_TO_CLASS(env_sky, CEnvSky)

void CEnvSky::Spawn()
{
	if (m_hSkyMaterial == NULL)
		m_hSkyMaterial = Glue::Resources::GetMaterial("materials/skybox/skybox_day_01.vmat");

	if (m_hSkyMaterial == NULL)
		printf("Good luck looking at the sky lol\n");

	m_pSceneSkyBox = (CSceneSkyBoxObject*)AcquireNextHandle(g_pSceneSystem->CreateSkyBox(m_hSkyMaterial, GN(MapLoader()->GetMainWorld())));
	m_pSceneSkyBox->SetSkyTint({m_vTintColor.x, m_vTintColor.y, m_vTintColor.z});
	m_pSceneSkyBox->SetMaterial(m_hSkyMaterial);
}

BEGIN_DATADESC(CEnvSky)
	DEFINE_KEYFIELD(m_hSkyMaterial, FIELD_MATERIAL, "skyname")
	DEFINE_KEYFIELD(m_vTintColor, FIELD_COLOR255, "tint_color")
END_DATADESC()
