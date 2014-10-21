#include "SchemaMaxInclusive.hpp"

CMaxInclusive* CMaxInclusive::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    CMaxInclusive *pMaxInclusive = CXSDNodeWithRestrictions::load(pParentNode, pSchemaRoot, xpath);

    if (pMaxInclusive == NULL)
    {
        return NULL;
    }

    pMaxInclusive->setMaxInclusive(pMaxInclusive->getValue());

    return pMaxInclusive;
}
