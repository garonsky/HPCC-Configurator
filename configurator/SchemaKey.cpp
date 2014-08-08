#include "SchemaKey.hpp"
#include "SchemaSelector.hpp"
#include "SchemaField.hpp"
#include "SchemaCommon.hpp"

CKey* CKey::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);
    assert(pParentNode != NULL);
    assert(pParentNode->getNodeType() == XSD_KEY_ARRAY);

    if (pSchemaRoot == NULL || pParentNode == NULL)
    {
        // TODO: Throw Exception
        return NULL;
    }

    CKey *pKey = NULL;

    if (xpath != NULL && *xpath != 0)
    {
        IPropertyTree* pTree = pSchemaRoot->queryPropTree(xpath);

        if (pTree == NULL)
        {
            return NULL; // no xs:key
        }

         const char* pName = pSchemaRoot->getPropTree(xpath)->queryProp(XML_ATTR_NAME);

         if (pName != NULL)
         {
             pKey = new CKey(pParentNode);
             pKey->setXSDXPath(xpath);
             pKey->setName(pName);
         }
         else
         {
             assert(!"value attribute can be empty!");
             // TODO: throw MakeExceptionFromMap(EX_STR_MISSING_VALUE_ATTRIBUTE_IN_LENGTH);
             return NULL;
         }

         const char *pID = pSchemaRoot->getPropTree(xpath)->queryProp(XML_ATTR_ID);

         if (pID != NULL)
         {
             pKey->setID(pID);
         }

         StringBuffer strXPathExt(xpath);
         strXPathExt.append("/").append(XSD_TAG_FIELD);
         m_pFieldArray = CFieldArray::load(pKey, pSchemaRoot, strXPathExt.str());

         strXPathExt.clear().set(xpath);
         strXPathExt.append("/").append(XSD_TAG_SELECTOR);
         m_pSelector = CSelector::load(pParentNode, pSchemaRoot, strXPathExt.str());

         assert(m_pFieldArray != NULL && m_pSelector != NULL);
    }

    return pKey;
}


void CKey::dump(std::ostream& cout, unsigned int offset) const
{
    offset += STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_KEY_STR, offset);

    QUICK_OUT(cout, Name, offset);
    QUICK_OUT(cout, ID, offset);
    QUICK_OUT(cout, XSDXPath,  offset);

    if (m_pFieldArray != NULL)
    {
        m_pFieldArray->dump(cout, offset);
    }

    if (m_pSelector != NULL)
    {
        m_pSelector->dump(cout, offset);
    }

    QuickOutFooter(cout, XSD_KEY_STR, offset);
}


CKeyArray* CKeyArray::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);
    assert(pParentNode->getNodeType() == XSD_ELEMENT);

    if (pSchemaRoot == NULL || xpath == NULL)
    {
        return NULL;
    }

    StringBuffer strXPathExt(xpath);

    CKeyArray *pKeyArray = new CKeyArray(pParentNode);
    pKeyArray->setXSDXPath(xpath);

    Owned<IPropertyTreeIterator> attributeIter = pSchemaRoot->getElements(xpath, ipt_ordered);

    int count = 1;
    ForEach(*attributeIter)
    {
        strXPathExt.clear().append(xpath).appendf("[%d]",count);

        CKey *pKey = CKey::load(pKeyArray, pSchemaRoot, strXPathExt.str());

        if (pKey != NULL)
        {
            pKeyArray->append(*pKey);
        }

        count++;
    }

    if (pKeyArray->length() == 0)
    {
        delete pKeyArray;
        pKeyArray = NULL;
    }

    return pKeyArray;
}

void CKeyArray::dump(std::ostream &cout, unsigned int offset) const
{
    offset+= STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_KEY_ARRAY_STR, offset);

    QUICK_OUT(cout, XSDXPath,  offset);
    QUICK_OUT(cout, EnvXPath,  offset);
    QUICK_OUT_ARRAY(cout, offset);

    QuickOutFooter(cout, XSD_KEY_ARRAY_STR, offset);
}
