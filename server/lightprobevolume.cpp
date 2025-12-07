#include "baseentity.h"
#include "datamap.h"
#include "maploader.h"
#include "pointentity.h"
#include "engine/engine2.h"
#include "engine/engine2vars.h"
#include "engine/handle.h"

class CLightProbeEntity : public CPointEntity
{
public:
	DECLARE_CLASS(CLightProbeEntity, CPointEntity)
	DECLARE_DATADESC()


	virtual void Spawn() override;
	
	CSceneLightProbeVolumeObject *m_pProbeVolumeObject;

	const char *m_szProbeTextureName = NULL;
	const char *m_szDliProbeTextureName = NULL;
	const char *m_szDlsProbeTextureName = NULL;
	Vector m_vBoxMins;
	Vector m_vBoxMaxs;
	int m_nHandShake;
	int m_indoorOutdoorLevel;

	HRenderTextureStrong *m_texture;
	HRenderTextureStrong *m_dliTexture;
	HRenderTextureStrong *m_dlsTexture;
};

void CLightProbeEntity::Spawn()
{
	if (m_texture)
		m_texture = Glue::Resources::GetTexture(m_szProbeTextureName);
	if (m_dliTexture)
		m_dliTexture = Glue::Resources::GetTexture(m_szDliProbeTextureName);
	if (m_dlsTexture)
		m_dlsTexture = Glue::Resources::GetTexture(m_szDlsProbeTextureName);
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
	g_pSceneSystem->MarkLightProbeVolumeObjectUpdated(m_pProbeVolumeObject);
}

LINK_ENTITY_TO_CLASS( env_combined_light_probe_volume, CLightProbeEntity )
LINK_ENTITY_TO_CLASS( env_light_probe_volume, CLightProbeEntity )

BEGIN_DATADESC(CLightProbeEntity)
	DEFINE_KEYFIELD(m_szProbeTextureName, FIELD_STRING, "lightprobetexture")
	DEFINE_KEYFIELD(m_szDliProbeTextureName, FIELD_STRING, "lightprobetexture_dli")
	DEFINE_KEYFIELD(m_szDliProbeTextureName, FIELD_STRING, "lightprobetexture_dls")
	DEFINE_KEYFIELD(m_vBoxMins, FIELD_VECTOR, "box_mins")
	DEFINE_KEYFIELD(m_vBoxMaxs, FIELD_VECTOR, "box_maxs")
	DEFINE_KEYFIELD(m_nHandShake, FIELD_INTEGER, "handshake")
	DEFINE_KEYFIELD(m_indoorOutdoorLevel, FIELD_INTEGER, "indoor_outdoor_level")
END_DATADESC()
