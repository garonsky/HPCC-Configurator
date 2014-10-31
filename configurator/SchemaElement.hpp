#ifndef _SCHEMA_ELEMENT_HPP_
#define _SCHEMA_ELEMENT_HPP_

#include "jiface.hpp"
#include "jstring.hpp"
#include "jlib.hpp"
#include "jarray.hpp"
#include "SchemaCommon.hpp"

class CAnnotation;
class CComplexTypeArray;
class IPropertyTree;
class CAttributeArray;
class CKeyArray;
class CKeyRefArray;
class CKeyRef;

static const char* DEFAULT_ELEMENT_ARRAY_XPATH(".");

class CElement : public CXSDNodeWithType
{
public:

    virtual ~CElement()
    {
        assert(false);
    }

    virtual const CXSDNodeBase* getNodeByTypeAndNameAscending(NODE_TYPES eNodeType, const char *pName) const;
    virtual const CXSDNodeBase* getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const;

    virtual const char* getXML(const char* /*pComponent*/);

    virtual void dump(std::ostream &cout, unsigned int offset = 0) const;

    virtual void getDocumentation(StringBuffer &strJS) const;

    virtual void getDojoJS(StringBuffer &strDoc) const;

    virtual void getQML(StringBuffer &strQML, int idx = -1) const;

    virtual void populateEnvXPath(StringBuffer strXPath, unsigned int index = 1);

    virtual void loadXMLFromEnvXml(const IPropertyTree *pEnvTree);

    virtual bool isTopLevelElement() const;

    void setTopLevelElement(bool b = true)
    {
        m_bTopLevelElement =  b;
    }

    void setParentIndex(int index)
    {
        m_nParentIndex = index;
    }

    int getParentIndex() const
    {
        return m_nParentIndex;
    }

    const CAnnotation* getAnnotation() const
    {
        return m_pAnnotation;
    }

    const CAttributeArray* getAttributeArray() const
    {
        return m_pAttributeArray;
    }

    const CComplexTypeArray* getComplexTypeArray() const
    {
        return m_pComplexTypeArray;
    }

    static const CXSDNodeBase* getAncestorElement(const CXSDNodeBase *pNode)
    {
         return pNode->getParentNodeByType(XSD_ELEMENT);
    }

    static const CElement* getTopMostElement(const CXSDNodeBase *pNode);

    static bool isAncestorTopElement(const CXSDNodeBase *pNode)
    {
        return (pNode != NULL && pNode->getParentNodeByType(XSD_ELEMENT) == getTopMostElement(pNode));
    }

    void setRefElementNode(CElement *pElement)
    {
        assert (pElement != NULL && this->getRefElementNode() != NULL);

        if (pElement != NULL)
        {
            this->m_pElementRefNode = pElement;
        }
    }

    CElement* getRefElementNode() const
    {
        return this->m_pElementRefNode;
    }

    //void addReverseKeyRef(const CKeyRef *pKeyRef);
    //void addReverseKey(const CKeyRef *pKey);

    bool isATab() const;
    bool isLastTab(const int idx) const;
    bool getIsInXSD() const
    {
        return m_bIsInXSD;
    }

    static CElement* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath, bool bIsInXSD = true);

    GETTERSETTER(Name)
    GETTERSETTER(MaxOccurs)
    GETTERSETTER(MinOccurs)
    GETTERSETTER(Title)
    GETTERSETTER(InstanceName)
    GETTERSETTER(Ref)

protected:

    CElement(CXSDNodeBase* pParentNode, const char* pName = "") : CXSDNodeWithType::CXSDNodeWithType(pParentNode, XSD_ELEMENT), m_strMinOccurs(""), m_strMaxOccurs(""), m_strName(pName), m_pAnnotation(NULL),
        m_pComplexTypeArray(NULL), m_pAttributeArray(NULL), m_pKeyArray(NULL), m_pKeyRefArray(NULL), m_pReverseKeyRefArray(NULL), m_pElementRefNode(NULL), m_bTopLevelElement(false), m_nParentIndex(-1), m_bIsInXSD(true)
    {
    }

    void setIsInXSD(bool b)
    {
        m_bIsInXSD = b;
    }

    CAnnotation * m_pAnnotation;
    CComplexTypeArray* m_pComplexTypeArray;
    CAttributeArray* m_pAttributeArray;
    CKeyArray *m_pKeyArray;
    CKeyRefArray *m_pKeyRefArray;
    CKeyRefArray *m_pReverseKeyRefArray;
    CElement *m_pElementRefNode;

    bool m_bTopLevelElement;
    int m_nParentIndex;
    bool m_bIsInXSD;


private:

    /*CElement() : CXSDNodeWithType::CXSDNodeWithType(NULL, XSD_ELEMENT), m_strMinOccurs(""), m_strMaxOccurs(""), m_strName(""), m_pAnnotation(NULL),
        m_pComplexTypeArray(NULL), m_pAttributeArray(NULL), m_pKeyArray(NULL), m_pKeyRefArray(NULL), m_bTopLevelElement(false)
    {
    }*/
};

class CElementArray : public CIArrayOf<CElement>, public InterfaceImpl, public CXSDNodeBase
{
    friend class CElement;
public:
    CElementArray(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot = NULL) : CXSDNodeBase::CXSDNodeBase(pParentNode, XSD_ELEMENT_ARRAY), m_pSchemaRoot(pSchemaRoot)
    {
    }

    virtual ~CElementArray()
    {
    }

    virtual const CXSDNodeBase* getNodeByTypeAndNameAscending(NODE_TYPES eNodeType, const char *pName) const;
    virtual const CXSDNodeBase* getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const;

    const CElement* getElementByNameAscending(const char *pName) const;
    const CElement* getElementByNameDescending(const char *pName) const;

    virtual void dump(std::ostream &cout, unsigned int offset = 0) const;

    virtual void getDocumentation(StringBuffer &strDoc) const;

    virtual void getDojoJS(StringBuffer &strJS) const;

    virtual void getQML(StringBuffer &strQML, int idx = -1) const;

    virtual void populateEnvXPath(StringBuffer strXPath, unsigned int index = 1);

    virtual void loadXMLFromEnvXml(const IPropertyTree *pEnvTree);

    virtual void traverseAndProcessNodes() const;

    virtual const char* getXML(const char* /*pComponent*/);

    virtual int getCountOfSiblingElements(const char *pXPath) const;

    virtual void setSchemaRoot(const IPropertyTree *pSchemaRoot)
    {
        assert(m_pSchemaRoot == NULL);
        assert(pSchemaRoot);

        m_pSchemaRoot = pSchemaRoot;
    }

    const IPropertyTree* getSchemaRoot() const
    {
        return m_pSchemaRoot;
    }

    static CElementArray* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath = DEFAULT_ELEMENT_ARRAY_XPATH);

    static CElementArray* load(const char *pSchemaFile);

protected:

    int getSiblingIndex(const char* pXSDXPath, const CElement* pElement);

    const IPropertyTree *m_pSchemaRoot;


private:

    CElementArray() : CXSDNodeBase::CXSDNodeBase(NULL, XSD_ELEMENT_ARRAY)
    {
    }
};

#endif // _SCHEMA_ELEMENT_HPP_
