#include "baseentity.h"
#include "datamap.h"
#include "engine/structs.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"
#include "maploader.h"
#include "envcubemap.h"

class CEnvCubemapBox : public CEnvCubemap 
{
public:
	DECLARE_CLASS(CEnvCubemapBox, CEnvCubemap)
	DECLARE_DATADESC()

	virtual void Spawn() override;

	Vector m_vBoxMins;
	Vector m_vBoxMaxs;
	int m_indoorOutdoorLevel = 0;
	float m_fCubemapFeathering = 0;

	HRenderTextureStrong *m_pTexture = NULL;
};

LINK_ENTITY_TO_CLASS(env_cubemap_box, CEnvCubemapBox)

void CEnvCubemapBox::Spawn()
{
	m_pSceneCubemap = (CEnvMapSceneObject*)AcquireNextHandle(g_pSceneSystem->CreateEnvMap(GN(MapLoader()->GetMainWorld()), 1));
	m_pSceneCubemap->m_nRenderPriority = 0;
	m_pSceneCubemap->m_vColor = {1,1,1};
	m_pSceneCubemap->m_flFeathering = m_fCubemapFeathering;
	m_pSceneCubemap->m_vBoxProjectMins = m_vBoxMins;
	m_pSceneCubemap->m_vBoxProjectMaxs = m_vBoxMaxs;

	if (m_pTexture)
	{
		m_pSceneCubemap->m_hEnvMapTexture = m_pTexture;
		m_pSceneCubemap->CalculateRadianceSH();
		m_pSceneCubemap->CalculateNormalizationSH();
		g_pSceneSystem->MarkEnvironmentMapObjectUpdated(GN(m_pSceneCubemap));
	}
}

BEGIN_DATADESC(CEnvCubemapBox)
	DEFINE_KEYFIELD(m_pTexture, FIELD_TEXTURE, "cubemaptexture")
	DEFINE_KEYFIELD(m_vBoxMins, FIELD_VECTOR, "box_mins")
	DEFINE_KEYFIELD(m_vBoxMaxs, FIELD_VECTOR, "box_maxs")
	DEFINE_KEYFIELD(m_indoorOutdoorLevel, FIELD_INTEGER, "indoor_outdoor_level")
	DEFINE_KEYFIELD(m_fCubemapFeathering, FIELD_FLOAT, "cubemap_feathering")
END_DATADESC()
