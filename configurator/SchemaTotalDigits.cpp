#include "SchemaTotalDigits.hpp"

CTotalDigits* CTotalDigits::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CTotalDigits *pTotalDigits = new CTotalDigits(pParentNode);

    pTotalDigits->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pTotalDigits;
        }

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pTotalDigits->setTotalDigits(atoi(pValue));
            pTotalDigits->setValue(pValue);
        }

        if (pTotalDigits->getTotalDigits() < 0)  // not set or bad length value
        {
            delete pTotalDigits;
            pTotalDigits = NULL;

            throw MakeExceptionFromMap(EX_STR_LENGTH_VALUE_MUST_BE_GREATER_THAN_OR_EQUAL_TO_ZERO , EACTION_TOTAL_DIGITS_BAD_LENGTH);
        }
    }

    return pTotalDigits;
}
