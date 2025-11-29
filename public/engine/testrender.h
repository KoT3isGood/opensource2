#include "structs.h"

abstract_class ITestRendering
{
public:
	virtual void Init() = 0;
	virtual void Frame(SwapChainHandle_t hSwapChain) = 0;
};

extern ITestRendering *pTestRendering;
