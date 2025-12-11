
#include "baseentity.h"
#include "datamap.h"
#include "engine/structs.h"
#include "maploader.h"
#include "pointentity.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"

class CEnvCombinedLightProbeVolume : public CPointEntity
{
public:
	DECLARE_CLASS(CEnvCombinedLightProbeVolume, CPointEntity)
	DECLARE_DATADESC()


	virtual void Spawn() override;
	
	CSceneLightProbeVolumeObject *m_pProbeVolumeObject;

	Vector m_vBoxMins;
	Vector m_vBoxMaxs;
	int m_nHandShake;
	int m_indoorOutdoorLevel;

	HRenderTextureStrong *m_texture = NULL;
	HRenderTextureStrong *m_dliTexture = NULL;
	HRenderTextureStrong *m_dlsTexture = NULL;
	
	float m_fCubemapFeathering = 0;
	HRenderTextureStrong *m_pTexture = NULL;
	
	CEnvMapSceneObject *m_pSceneCubemap;
};

void CEnvCombinedLightProbeVolume::Spawn()
{
	m_pProbeVolumeObject = (CSceneLightProbeVolumeObject*)AcquireNextHandle(g_pSceneSystem->CreateLightProbeVolume(GN(MapLoader()->GetMainWorld())));
	if (m_texture)
		m_pProbeVolumeObject->m_hLightProbeTexture = m_texture;
	if (m_dliTexture)
		m_pProbeVolumeObject->m_hLightProbeDirectLightIndicesTexture = m_dliTexture;
	if (m_dlsTexture)
		m_pProbeVolumeObject->m_hLightProbeDirectLightScalarsTexture = m_dlsTexture;
	m_pProbeVolumeObject->m_vBoxMaxs = m_vBoxMaxs;
	m_pProbeVolumeObject->m_vBoxMaxs = m_vBoxMins;
	m_pProbeVolumeObject->m_nHandshake = m_nHandShake;
	m_pProbeVolumeObject->m_nRenderPriority = m_indoorOutdoorLevel;
	m_pProbeVolumeObject->SetRenderingEnabled(true);
	m_pProbeVolumeObject->CreateConstants();
	m_pProbeVolumeObject->SetTransform(m_transform);
	g_pSceneSystem->MarkLightProbeVolumeObjectUpdated(GN(m_pProbeVolumeObject));
	
	m_pSceneCubemap = (CEnvMapSceneObject*)AcquireNextHandle(g_pSceneSystem->CreateEnvMap(GN(MapLoader()->GetMainWorld()), 1));
	m_pSceneCubemap->m_nRenderPriority = 0;
	m_pSceneCubemap->m_vColor = {1,1,1};
	m_pSceneCubemap->m_flFeathering = m_fCubemapFeathering;
	m_pSceneCubemap->m_vBoxProjectMins = m_vBoxMins;
	m_pSceneCubemap->m_vBoxProjectMaxs = m_vBoxMaxs;
	m_pSceneCubemap->SetWorldDirection(m_transform.m_rotation);
	m_pSceneCubemap->SetWorldPosition(m_transform.m_position);


	if (m_pTexture)
	{
		m_pSceneCubemap->m_hEnvMapTexture = m_pTexture;
		m_pSceneCubemap->CalculateRadianceSH();
		m_pSceneCubemap->CalculateNormalizationSH();
		g_pSceneSystem->MarkEnvironmentMapObjectUpdated(GN(m_pSceneCubemap));
	}
}

LINK_ENTITY_TO_CLASS( env_combined_light_probe_volume, CEnvCombinedLightProbeVolume )

BEGIN_DATADESC(CEnvCombinedLightProbeVolume)
	DEFINE_KEYFIELD(m_texture, FIELD_TEXTURE, "lightprobetexture")
	DEFINE_KEYFIELD(m_dliTexture, FIELD_TEXTURE, "lightprobetexture_dli")
	DEFINE_KEYFIELD(m_dlsTexture, FIELD_TEXTURE, "lightprobetexture_dls")
	DEFINE_KEYFIELD(m_vBoxMins, FIELD_VECTOR, "box_mins")
	DEFINE_KEYFIELD(m_vBoxMaxs, FIELD_VECTOR, "box_maxs")
	DEFINE_KEYFIELD(m_nHandShake, FIELD_INTEGER, "handshake")
	DEFINE_KEYFIELD(m_indoorOutdoorLevel, FIELD_INTEGER, "indoor_outdoor_level")
	DEFINE_KEYFIELD(m_pTexture, FIELD_TEXTURE, "cubemaptexture")
	DEFINE_KEYFIELD(m_fCubemapFeathering, FIELD_FLOAT, "cubemap_feathering")
END_DATADESC()

