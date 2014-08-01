#include "SchemaPattern.hpp"

CPattern* CPattern::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CPattern *pPattern = NULL;

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pPattern;
        }

        pPattern = new CPattern(pParentNode);
        pPattern->setXSDXPath(xpath);

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0)
        {
            pPattern->setValue(pValue);
        }
        else
        {
            delete pPattern;
            pPattern = NULL;

            assert(false);
//            throw MakeExceptionFromMap(EX_STR_MISSING_REQUIRED_ATTRIBUTE , EACTION_PATTERN_MISSING_VALUE);
        }
    }

    return pPattern;
}


void CPattern::dump(std::ostream& cout, unsigned int offset) const
{
    offset+= STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_FRACTION_DIGITS_STR, offset);

    QUICK_OUT(cout, XSDXPath,  offset);
    QUICK_OUT(cout, EnvXPath,  offset);
    QUICK_OUT(cout, Value, offset);
    QUICK_OUT(cout, Pattern, offset);

    QuickOutFooter(cout, XSD_FRACTION_DIGITS_STR, offset);
}
