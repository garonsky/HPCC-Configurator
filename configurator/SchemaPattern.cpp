#include "SchemaMaxLength.hpp"

CPattern* CPattern::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CPattern *pPattern = NULL;

    pMaxLength->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pPattern;
        }

        pPattern = new CPattern(pParentNode);

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pPattern->setValue(pValue);
        }
        else
        {
            delete pPattern;
            pPattern = NULL;

            throw MakeExceptionFromMap(EX_STR_MISSING_REQUIRED_ATTRIBUTE , EACTION_PATTERN_MISSING_VALUE);
        }
    }

    return pPattern;
}
