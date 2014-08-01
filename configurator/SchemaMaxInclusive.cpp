#include "SchemaMaxInclusive.hpp"


#include "SchemaMinInclusive.hpp"

CMaxInclusive* CMaxInclusive::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CMaxInclusive *pMaxInclusive = new CMaxInclusive(pParentNode);

    pMaxInclusive->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pMaxInclusive;
        }

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pMaxInclusive->setMaxInclusive(pValue);
            pMaxInclusive->setValue(pValue);
        }
        else
        {
            delete pMaxInclusive;
            pMaxInclusive = NULL;

            assert(false);
            //throw MakeExceptionFromMap(EX_STR_MISSING_REQUIRED_ATTRIBUTE , EACTION_MAX_INCLUSIVE_NO_VALUE);
        }
    }

    return pMaxInclusive;
}

void CMaxInclusive::dump(std::ostream& cout, unsigned int offset) const
{
    offset += STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_MAX_INCLUSIVE_STR, offset);

    QUICK_OUT(cout, Value, offset);

    QuickOutFooter(cout, XSD_MAX_INCLUSIVE_STR, offset);
}
