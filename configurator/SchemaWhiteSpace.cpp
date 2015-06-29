#include "SchemaWhiteSpace.hpp"

CWhiteSpace* CWhiteSpace::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    CWhiteSpace *pWhiteSpace = CXSDNodeWithRestrictions<CWhiteSpace>::load(pParentNode, pSchemaRoot, xpath);

    if (pWhiteSpace == NULL)
    {
        return NULL;
    }

    return pWhiteSpace;
}
