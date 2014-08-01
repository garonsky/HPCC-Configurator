#include "SchemaWhiteSpace.hpp"

CWhiteSpace* CWhiteSpace::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CWhiteSpace *pWhiteSpace = NULL;

    pWhiteSpace->setXSDXPath(xpath);

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return pWhiteSpace;
        }

        const char* pValue = pTree->queryProp(XML_ATTR_VALUE);

        if (pValue != NULL && *pValue != 0 && (stricmp(pValue, "preserve") == 0 || stricmp(pValue, "replace") == 0 || stricmp(pValue, "collapse") == 0))
        {
            pWhiteSpace = new CWhiteSpace(pParentNode);
            pWhiteSpace->setValue(pValue);
        }
        else
        {
            assert(false);
            //throw MakeExceptionFromMap(EX_STR_WHITE_SPACE_HAS_INVALID_VALUE , EACTION_WHITE_SPACE_BAD_VALUE);
        }
    }

    return pWhiteSpace;
}

void CWhiteSpace::dump(std::ostream& cout, unsigned int offset) const
{
    offset+= STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_WHITE_SPACE_STR, offset);

    QUICK_OUT(cout, XSDXPath,  offset);
    QUICK_OUT(cout, EnvXPath,  offset);
    QUICK_OUT(cout, Value, offset);

    QuickOutFooter(cout, XSD_WHITE_SPACE_STR, offset);
}
