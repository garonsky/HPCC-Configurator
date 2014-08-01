#include "jhash.hpp"
#include "jhash.ipp"
#include "jlog.hpp"
#include "SchemaCommon.hpp"

#ifndef _SCHEMA_MAP_MANAGER_HPP_
#define _SCHEMA_MAP_MANAGER_HPP_

class CSchema;
class CAttribute;
class CAttributeGroup;
class CSimpleType;
class CComplexType;
class CAttributeGroup;
class CAttribute;
class CRestriction;
class CElementArray;
class CElement;

class CSchemaMapManager
{
public:

    CSchemaMapManager();
    virtual ~CSchemaMapManager();

    CSchema* getSchemaForXSD(const char* pComponent);
    void setSchemaForXSD(const char* pComponent, CSchema *pSchema);

    CSimpleType* getSimpleTypeWithName(const char* pName);
    void setSimpleTypeWithName(const char* pName, CSimpleType *pSimpleType);

    CComplexType* getComplexTypeWithName(const char* pName);
    void setComplexTypeWithName(const char* pName, CComplexType *pComplexType);
    CComplexType* getComplexTypeFromXPath(const char *pXPath);

    CAttributeGroup *getAttributeGroup(const char* pName);
    void setAttributeGroupTypeWithName(const char* pName, CAttributeGroup *pAttributeGroup);
    CAttributeGroup* getAttributeGroupFromXPath(const char *pXPath);

    void addMapOfXPathToAttribute(const char* pXPath, CAttribute *pAttribute);
    void removeMapOfXPathToAttribute(const char* pXPath);
    CAttribute* getAttributeFromXPath(const char* pXPath);

    void addMapOfXPathToRestriction(const char*pXPath, CRestriction *pRestriction);
    void removeMapOfXPathToRestriction(const char*pXPath);
    CRestriction* getRestrictionFromXPath(const char* pXPath);

    void addMapOfXPathToElementArray(const char* pXPath, CElementArray *pElementArray);
    void removeMapOfXPathToElementArray(const char* pXPath);
    CElementArray* getElementArrayFromXPath(const char* pXPath);

    void addMapOfXPathToElement(const char* pXPath, CElement *pElement, bool bIsTopLevelElement = false);
    void removeMapOfXPathToElement(const char* pXPath);
    CElement* getElementFromXPath(const char *pXPath);

    int getNumberOfComponents() const;
    CElement* getComponent(int index);
    int getIndexOfElement(const CElement *pElem);

    enum NODE_TYPES getEnumFromTypeName(const char *pTypeName) const;
    const char* getTypeNameFromEnum(enum NODE_TYPES, bool bForDump = false) const;

protected:

    typedef MapStringTo<CSchema*> MapStringToCSchema;
    Owned<MapStringToCSchema> m_pSchemaPtrMap;

    typedef MapStringTo<CSimpleType*> MapStringToCSimpleType;
    Owned<MapStringToCSimpleType> m_pSimpleTypePtrMap;

    typedef MapStringTo<CComplexType*> MapStringToCComplexType;
    Owned<MapStringToCComplexType> m_pComplexTypePtrsMap;

    typedef MapStringTo<CAttributeGroup*> MapStringToCAttributeGroup;
    Owned<MapStringToCAttributeGroup> m_pAttributeGroupTypePtrsMap;

    typedef MapStringTo<CAttribute*> MapStringToCAttribute;
    Owned<MapStringToCAttribute> m_pAttributePtrsMap;

    typedef MapStringTo<CRestriction*> MapStringToCRestriction;
    Owned<MapStringToCRestriction> m_pRestrictionPtrsMap;

    typedef MapStringTo<CElement*> MapStringToCElement;
    Owned<MapStringToCElement> m_pElementPtrsMap;

    typedef MapStringTo<CElementArray*> MapStringToCElementArray;
    Owned<MapStringToCElementArray> m_pElementArrayPtrsMap;

    const char* m_enumArray[XSD_ERROR][2];

    typedef MapStringTo<enum NODE_TYPES> MapStringToNodeTypeEnum;
    Owned<MapStringToNodeTypeEnum> m_pStringToEnumMap;

    struct STypeStrings
    {
        const char *pXSDTypeString;
        const char *pDumpTypeString;
    };

    /*typedef MapBetween<enum NODE_TYPES, enum NODE_TYPES, STypeStrings, STypeStrings> MapEnumToTypeStringStruct;
    Owned<MapEnumToTypeStringStruct> m_pStringToEnumMap;*/

private:

    template<typename T1, typename T2>
    void clearArrays(T1 *a)
    {
        void *p = NULL;

//        while (true)
        //{
            HashIterator iter(*a);

            //iter.first();
            //if (iter.isValid() == true)
            ForEach(iter)
            {
                const char *pKey = static_cast<const char*>((&iter.query())->getKey());
                T2 *pT2 = *(a->getValue(pKey));

                assert(pT2->getLinkCount() == 1);
                assert(pT2 != p);
                p = NULL;

                PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
                PROGLOG("remove with key = %s pT2=%p pT2->xxcount->counter = %d", pKey, pT2, pT2->getLinkCount());

                //a->remove(pKey);
                //a->removeExact(&iter.query());
                //pT2->Release();

            }
/*            else
            {
                break;
            }
        }*/
            a->kill();
    }

    /*void clearArrays(StringMapOf<MAPPING> *pStringMap)
    {
        while (true)
        {
            HashIterator iter(*pStringMap);

            iter.first();
            if (iter.isValid() == true)
            {
                const char *pKey = static_cast<const char*>((&iter.query())->getKey());
                InterfaceImpl *p = *(pStringMap->getValue(pKey));

                p->Release();
                pStringMap->remove(pKey);
            }
            else
            {
                break;
            }
        }
    }*/
};

#endif // _SCHEMA_MAP_MANAGER_HPP_
