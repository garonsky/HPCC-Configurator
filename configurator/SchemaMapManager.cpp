#include "SchemaMapManager.hpp"
#include "SchemaAll.hpp"

CSchemaMapManager::CSchemaMapManager()
{
    m_pSchemaPtrMap.setown(new MapStringToCSchema());
    m_pSimpleTypePtrMap.setown(new MapStringToCSimpleType());
    m_pComplexTypePtrsMap.setown (new MapStringToCComplexType);
    m_pAttributeGroupTypePtrsMap.setown(new MapStringToCAttributeGroup);
    m_pAttributePtrsMap.setown(new MapStringToCAttribute);
    m_pRestrictionPtrsMap.setown(new MapStringToCRestriction);
    m_pElementPtrsMap.setown(new MapStringToCElement);
    m_pElementArrayPtrsMap.setown(new MapStringToCElementArray);
}

CSchemaMapManager::~CSchemaMapManager()
{
    //PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
}

CSchema* CSchemaMapManager::getSchemaForXSD(const char* pComponent)
{
    CSchema **pSchema = m_pSchemaPtrMap->getValue(pComponent);

    if (pSchema != NULL )
    {
        assert ((*pSchema)->getLinkCount() == 1);
        return *pSchema;
    }
    else
    {
        return NULL;
    }
}

void CSchemaMapManager::setSchemaForXSD(const char* pComponent, CSchema *pSchema)
{
    assert(pSchema != NULL);
    assert(pComponent != NULL);
    assert(*pComponent != 0);

    assert(pSchema->getLinkCount() == 1);
    //assert(m_pSchemaPtrMap->getValue(pComponent) == NULL);

    if (pSchema != NULL && pComponent != NULL && *pComponent != 0)// && m_pSchemaPtrMap->getValue(pComponent) == NULL)
    {
        if (m_pSchemaPtrMap->getValue(pComponent) == NULL)
        {
            m_pSchemaPtrMap->setValue(pComponent, (pSchema));
        }
        else
        {
            //pSchema->Link();
        }
        //m_pSchemaPtrMap->setValue(pComponent, (pSchema));
    }
}

CSimpleType* CSchemaMapManager::getSimpleTypeWithName(const char* pName)
{
    assert(pName != NULL);

    if (pName == NULL)
    {
        return NULL;
    }

    CSimpleType **ppSimpleType = NULL;

    ppSimpleType = m_pSimpleTypePtrMap->getValue(pName);

    if (ppSimpleType != NULL)
    {
        return *ppSimpleType;
    }
    else
    {
        return NULL;
    }
}

void CSchemaMapManager::setSimpleTypeWithName(const char* pName, CSimpleType *pSimpleType)
{
    assert (pSimpleType != NULL);

    if (pName == NULL || pSimpleType == NULL)
    {
        return;
    }

    if (m_pSimpleTypePtrMap->getValue(pName) != NULL)
    {
        throw MakeExceptionFromMap(EX_STR_SIMPLE_TYPE_ALREADY_DEFINED);
    }

    assert(pSimpleType->getLinkCount() == 1);
    m_pSimpleTypePtrMap->setValue(pName, pSimpleType);
}

CComplexType* CSchemaMapManager::getComplexTypeWithName(const char* pName)
{
    assert(pName != NULL);

    if (pName == NULL)
    {
        return NULL;
    }

    CComplexType *pComplexType = NULL;

    pComplexType = *(m_pComplexTypePtrsMap->getValue(pName));

    assert(pComplexType != NULL);

    return pComplexType;
}

void CSchemaMapManager::setComplexTypeWithName(const char* pName, CComplexType *pComplexType)
{
    assert (pComplexType != NULL);

    if (pName == NULL || pComplexType == NULL)
    {
        return;
    }

    if (m_pComplexTypePtrsMap->getValue(pName) != NULL)
    {
        throw MakeExceptionFromMap(EX_STR_COMPLEX_TYPE_ALREADY_DEFINED);
    }

    assert(pComplexType->getLinkCount() == 1);
    m_pComplexTypePtrsMap->setValue(pName, pComplexType);
}

CComplexType* CSchemaMapManager::getComplexTypeFromXPath(const char *pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    CComplexType** ppComplexType =  m_pComplexTypePtrsMap->getValue(pXPath);

    if (ppComplexType != NULL)
    {
        return *ppComplexType;
    }
    else
    {
        return NULL;
    }
}

CAttributeGroup* CSchemaMapManager::getAttributeGroup(const char* pName)
{
    assert(pName != NULL);

    if (pName == NULL)
    {
        return NULL;
    }

    CAttributeGroup *pAttributeGroup = NULL;

    pAttributeGroup = *(m_pAttributeGroupTypePtrsMap->getValue(pName));

    assert(pAttributeGroup != NULL);

    return pAttributeGroup;
}

void CSchemaMapManager::setAttributeGroupTypeWithName(const char* pName, CAttributeGroup *pAttributeGroup)
{
    assert (pAttributeGroup != NULL);

    if (pName == NULL || pAttributeGroup == NULL)
    {
        return;
    }

    if (m_pAttributeGroupTypePtrsMap->getValue(pName) != NULL)
    {
        m_pAttributeGroupTypePtrsMap->remove(pName);
        //throw MakeExceptionFromMap(EX_STR_ATTRIBUTE_GROUP_ALREADY_DEFINED);
    }

    assert(pAttributeGroup->getLinkCount() == 1);
    m_pAttributeGroupTypePtrsMap->setValue(pName, pAttributeGroup);
}

CAttributeGroup* CSchemaMapManager::getAttributeGroupFromXPath(const char *pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    if (pXPath == NULL || *pXPath == 0)
    {
        return NULL;
    }

    CAttributeGroup **ppAttributeGroup = m_pAttributeGroupTypePtrsMap->getValue(pXPath);

    assert(ppAttributeGroup != NULL);

    if (ppAttributeGroup != NULL)
    {
        return *ppAttributeGroup;
    }
    else
    {
        return NULL;
    }
}

void CSchemaMapManager::addMapOfXPathToAttribute(const char*pXPath, CAttribute *pAttribute)
{
    assert (pAttribute != NULL);
    assert(pXPath != NULL && *pXPath != 0);

    // TODO:: throw exception if problems here

    assert(m_pAttributePtrsMap->find(pXPath) == NULL);

    // should I remove automatically?

    assert(pAttribute->getLinkCount() == 1);
    m_pAttributePtrsMap->setValue(pXPath, pAttribute);
}

void CSchemaMapManager::removeMapOfXPathToAttribute(const char*pXPath)
{
    assert (m_pAttributePtrsMap->find(pXPath) != NULL);

    m_pAttributePtrsMap->remove(pXPath);
}

CAttribute* CSchemaMapManager::getAttributeFromXPath(const char* pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    CAttribute **pAttribute = m_pAttributePtrsMap->getValue(pXPath);

    if (STRICTNESS_LEVEL >= DEFAULT_STRICTNESS)
    {
        assert(pAttribute != NULL);
    }
    if (pAttribute == NULL)
    {
        return NULL;
    }

    return *pAttribute;
}

void CSchemaMapManager::addMapOfXPathToElementArray(const char*pXPath, CElementArray *pElementArray)
{
    assert (pElementArray != NULL);
    assert(pXPath != NULL && *pXPath != 0);
    assert(pElementArray->getLinkCount() == 1);

    if (m_pElementArrayPtrsMap->find(pXPath) != NULL)
    {
        return;  // already mapped, we must be dealing with live data
    }


    m_pElementArrayPtrsMap->setValue(pXPath, pElementArray);
}

void CSchemaMapManager::removeMapOfXPathToElementArray(const char*pXPath)
{
    assert (m_pElementArrayPtrsMap->find(pXPath) != NULL);
    m_pElementArrayPtrsMap->remove(pXPath);
}

CElementArray* CSchemaMapManager::getElementArrayFromXPath(const char* pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    if (pXPath == NULL)
    {
        return NULL;
    }

    CElementArray** ppElementArray = m_pElementArrayPtrsMap->getValue(pXPath);

    if (ppElementArray != NULL)
    {
        return *ppElementArray;
    }
    else
    {
        return NULL;
    }
}

void CSchemaMapManager::addMapOfXPathToElement(const char* pXPath, CElement *pElement,  bool bIsTopLevelElement)
{
    assert (pElement != NULL);
    assert(pXPath != NULL && *pXPath != 0);

    //assert(m_pElementPtrsMap->find(pXPath) == NULL);

    //PROGLOG("Mapping XPath %s to %p element", pXPath, pElement);

    assert(pElement->getLinkCount() == 1);
    m_pElementPtrsMap->setValue(pXPath, pElement);
}

void CSchemaMapManager::removeMapOfXPathToElement(const char*pXPath)
{
    assert (m_pElementPtrsMap->find(pXPath) != NULL);
    m_pElementPtrsMap->remove(pXPath);
}

/*void CSchemaMapManager::clearMapOfXPathToElement()
{
    m_pElementPtrsMap->clear();
}*/

CElement* CSchemaMapManager::getElementFromXPath(const char *pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    CElement **ppElement = m_pElementPtrsMap->getValue(pXPath);

    assert(ppElement != NULL);

    if (ppElement != NULL)
    {
        return *ppElement;
    }
    else
    {
        return NULL;
    }
}

void CSchemaMapManager::addMapOfXPathToRestriction(const char*pXPath, CRestriction *pRestriction)
{
    assert (pRestriction != NULL);
    assert(pXPath != NULL && *pXPath != 0);
    assert(m_pRestrictionPtrsMap->find(pXPath) == NULL);

    assert(pRestriction->getLinkCount() == 1);
    m_pRestrictionPtrsMap->setValue(pXPath, pRestriction);
}

void CSchemaMapManager::removeMapOfXPathToRestriction(const char*pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    m_pRestrictionPtrsMap->remove(pXPath);
}

CRestriction* CSchemaMapManager::getRestrictionFromXPath(const char* pXPath)
{
    assert(pXPath != NULL && *pXPath != 0);

    CRestriction **ppRestriction = m_pRestrictionPtrsMap->getValue(pXPath);

    assert(ppRestriction != NULL);

    if (ppRestriction != NULL)
    {
        return *ppRestriction;
    }
    else
    {
        return NULL;
    }
}

int CSchemaMapManager::getNumberOfComponents() const
{
    HashIterator iter(*(m_pElementPtrsMap.get()));

    int nCount = 0;

    ForEach(iter)
    {
        CElement *pElement = *(m_pElementPtrsMap->mapToValue(&iter.query()));

        if (pElement->isTopLevelElement() == true)
        {
            nCount++;
        }
    }

    return nCount;
}

CElement* CSchemaMapManager::getComponent(int index)
{
    assert(index >= 0 && index < getNumberOfComponents());

    HashIterator iter(*(m_pElementPtrsMap.get()));

    int nCount = 0;

    ForEach(iter)
    {
        CElement *pElement = *(m_pElementPtrsMap->mapToValue(&iter.query()));

        if (pElement->isTopLevelElement() == true)
        {
            if (nCount == index)
            {
                return pElement;
            }

            nCount++;
        }        
    }

    return NULL;
}

int CSchemaMapManager::getIndexOfElement(const CElement *pElem)
{
    int nCount = 0;

    HashIterator iter(*(m_pElementPtrsMap.get()));

    ForEach(iter)
    {
        CElement *pElement = *(m_pElementPtrsMap->mapToValue(&iter.query()));

        if (pElement == pElem)
        {
            return nCount;
        }

        if (pElement->isTopLevelElement() == true)
        {
            nCount++;
        }
    }

    assert(false);
    return -1;
}
