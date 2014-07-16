#include "SchemaMaxExclusive.hpp"

CMaxExclusive* CMaxExclusive::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CMaxExclusive *pMaxExclusive = new CMaxExclusive(pParentNode);

    pMaxExclusive->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pMaxExclusive;
        }

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pMaxExclusive->setMinExclusive(pValue);
            pMaxExclusive->setValue(pValue);
        }
        else
        {
            delete pMaxExclusive;
            pMaxExclusive = NULL;

            throw MakeExceptionFromMap(EX_STR_MISSING_REQUIRED_ATTRIBUTE , EACTION_MIN_EXCLUSIVE_NO_VALUE);
        }
    }

    return pMaxExclusive;
}

void CMaxExclusive::dump(std::ostream& cout, unsigned int offset = 0) const
{
    offset += STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_MIN_EXCLUSIVE_STR, offset);

    QUICK_OUT(cout, Value, offset);

    QuickOutFooter(cout, XSD_MIN_EXCLUSIVE_STR, offset);
}
