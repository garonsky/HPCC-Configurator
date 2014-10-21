#include "SchemaMaxLength.hpp"

CMaxLength* CMaxLength::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    CMaxLength *pMaxLength = CXSDNodeWithRestrictions::load(pParentNode, pSchemaRoot, xpath);

    if (pMaxLength == NULL)
    {
        return NULL;
    }

    pMaxLength->setMaxLength(pMaxLength->getValue());

    return pMaxLength;
}
