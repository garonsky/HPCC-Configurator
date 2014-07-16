#include "SchemaMaxLength.hpp"

CMaxLength* CMaxLength::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CMaxLength *pMaxLength = new CMaxLength(pParentNode);

    pMaxLength->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pMaxLength;
        }

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pMaxLength->setMaxLength(atoi(pValue));
            pMaxLength->setValue(pValue);
        }

        if (pMaxLength->setMaxLength() < 0)  // not set or bad length value
        {
            delete pMaxLength;
            pMaxLength = NULL;

            throw MakeExceptionFromMap(EX_STR_LENGTH_VALUE_MUST_BE_GREATER_THAN_OR_EQUAL_TO_ZERO , EACTION_MAX_LENGTH_BAD_LENGTH);
        }
    }

    return pMaxLength;
}
