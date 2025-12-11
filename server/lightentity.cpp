#include "lightentity.h"
#include "datamap.h"
#include "engine/stringtoken.h"
#include "engine/structs.h"

void CLightEntity::Spawn()
{
	if (!m_pLightObject)
		return;


	printf("%f %f %f\n", m_transform.m_position.x, m_transform.m_position.y, m_transform.m_position.z);
	printf("%f %f %f %f\n", m_transform.m_rotation.x, m_transform.m_rotation.y, m_transform.m_rotation.z, m_transform.m_rotation.w);
	printf("%f %f %f\n", m_transform.m_scale.x, m_transform.m_scale.y, m_transform.m_scale.z);

	m_pLightObject->AddTag(StringToken("light"));
	m_pLightObject->SetColor((Vector){1, 1, 1});
	m_pLightObject->SetBounceColor({1,1,1});
	m_pLightObject->SetRenderDiffuse(true);
	m_pLightObject->SetRenderSpecular(true);
	m_pLightObject->SetShadows(1);
	m_pLightObject->SetTransform(m_transform);
	m_pLightObject->SetLightFlags(m_pLightObject->GetLightFlags());
};

BEGIN_DATADESC(CLightEntity)
	
	DEFINE_KEYFIELD(m_lightColor, FIELD_COLOR255, "color")
	DEFINE_KEYFIELD(m_nShadows, FIELD_INTEGER, "cast")
	DEFINE_KEYFIELD(m_nCascades, FIELD_INTEGER, "numcascades")
	DEFINE_KEYFIELD(m_szLightCookie, FIELD_STRING, "lightcookie")
	//DEFINE_KEYFIELD(m_fShadowCascadeDistanceScale, FIELD_FLOAT, "shadowcascadedistancescale")
	DEFINE_KEYFIELD(m_iBakeLightIndex, FIELD_INTEGER, "bakelightindex")
END_DATADESC()
