#include "lightentity.h"

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
	GetDataMap()->m_iNumFields;
}

BEGIN_DATADESC(CDirectionalLight)
END_DATADESC()
