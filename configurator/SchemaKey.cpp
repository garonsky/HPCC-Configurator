#include "SchemaKey.hpp"
#include "SchemaSelector.hpp"
#include "SchemaField.hpp"
#include "SchemaAnnotation.hpp"
#include "SchemaCommon.hpp"
#include "ConfigSchemaHelper.hpp"
#include "SchemaMapManager.hpp"
#include "SchemaAttributes.hpp"

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

         strXPathExt.append("/").append(XSD_TAG_ANNOTATION);
         pElement->m_pAnnotation = CAnnotation::load(pElement, pSchemaRoot, strXPathExt.str());

    }

    return pKey;
}

bool CKey::checkConstraint(const char *pValue) const
{
    bool bRetVal = true;

    if (m_pSelector != NULL && m_pFieldArray->length() != 0)
    {
        for (int idx = 0; idx < m_pFieldArray->length(); idx++)
        {
            assert(!"Multiple fields not implemented");

            const CField *m_pField = m_pFieldArray->item(idx);

            assert(m_pField != NULL);

            if (m_pField == NULL)
            {
                return false;
            }

            StringBuffer strXSDPathForConstraint(this->getXSDXPath());

            strXSDPathForConstraint.appendf("/%s", this->m_pSelector->getXPath());

            const CElement *pElement = CConfigSchemaHelper::getSchemaMapManager()->getElementFromXSDXPath(strXSDXPath.str());

            if (pElement == NULL)
            {
                return false;
            }

            const CAttribute *pAttribute = pElement->getNodeByTypeAndNameDescending(XSD_ATTRIBUTE, m_pField->getXPath());  // needs to be first possible descendent

            if (pAttribute != NULL)
            {
                if (pAttribute->getParentNodeByType(XSD_ELEMENT) == pElement)
                {

                }
                else
                {
                    assert(!"Could not find match for key");
                }
            }
        }
    }

    return bRetVal;
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

virtual bool CKeyArray::checkConstraint(const char *pValue) const
{
    assert(pValue != NULL);

    if (pValue == NULL)
    {
        return false;
    }

    for (int idx = 0; idx < this->length(); idx++)
    {
        if ((this->item(idx)).checkConstraint(pValue) == false)
        {
            return false;
        }
    }

    return true;
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
