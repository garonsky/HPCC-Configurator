#include "SchemaMinLength.hpp"

CMinLength* CMinLength::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    CMinLength *pMinLength = CXSDNodeWithRestrictions::load(pParentNode, pSchemaRoot, xpath);

    if (pMinLength == NULL)
    {
        return NULL;
    }

    pMinLength->setMinLength(pMinLength->getValue());

    return pMinLength;
}
