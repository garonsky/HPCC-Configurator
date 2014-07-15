#include "SchemaMinInclusive.hpp"

CMinInclusive* CMinInclusive::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CMinInclusive *pMinInclusive = new CMinInclusive(pParentNode);

    pMinInclusive->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pMinInclusive;
        }

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pMinInclusive->setMinInclusive(pValue);
            pMinInclusive->setValue(pValue);
        }
        else
        {
            delete pMinInclusive;
            pMinInclusive = NULL;

            throw MakeExceptionFromMap(EX_STR_MISSING_REQUIRED_ATTRIBUTE , EACTION_MIN_INCLUSIVE_NO_VALUE);
        }
    }

    return pMinInclusive;
}

void CMinInclusive::dump(std::ostream& cout, unsigned int offset = 0) const
{
    offset += STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_LENGTH_STR, offset);

    QUICK_OUT(cout, Value, offset);

    QuickOutFooter(cout, XSD_MIN_EXCLUSIVE_STR, offset);
}
