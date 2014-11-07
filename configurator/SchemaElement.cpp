#include <cassert>
#include "jptree.hpp"
#include "jstring.hpp"
#include "XMLTags.h"
#include "SchemaAnnotation.hpp"
#include "SchemaCommon.hpp"
#include "SchemaElement.hpp"
#include "SchemaComplexType.hpp"
#include "SchemaElement.hpp"
#include "SchemaAttributes.hpp"
#include "SchemaAppInfo.hpp"
#include "SchemaDocumentation.hpp"
#include "DocumentationMarkup.hpp"
#include "ConfigSchemaHelper.hpp"
#include "DojoJSMarkup.hpp"
#include "ConfigSchemaHelper.hpp"
#include "DojoHelper.hpp"
#include "QMLMarkup.hpp"
#include "SchemaMapManager.hpp"
#include "ConfiguratorMain.hpp"

const CXSDNodeBase* CElement::getNodeByTypeAndNameAscending(NODE_TYPES eNodeType, const char *pName) const
{
    const CXSDNodeBase* pMatchingNode = NULL;

    if (eNodeType == this->getNodeType() && (pName != NULL ? !strcmp(pName, this->getNodeTypeStr()) : true))
    {
        return this;
    }

    if (eNodeType == XSD_ELEMENT)
    {
        pMatchingNode = (dynamic_cast<CElement*>(this->getParentNode()))->getNodeByTypeAndNameAscending(XSD_ELEMENT, pName);
    }

    if (pMatchingNode == NULL)
    {
        pMatchingNode = (dynamic_cast<CElementArray*>(this->getParentNode()))->getNodeByTypeAndNameAscending(eNodeType, pName);
    }

    return pMatchingNode;

}

const CXSDNodeBase* CElement::getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const
{
    const CXSDNodeBase* pMatchingNode = NULL;

    if (eNodeType == this->getNodeType() && (pName != NULL ? !strcmp(pName, this->getNodeTypeStr()) : true))
    {
        return this;
    }

    if (m_pComplexTypeArray != NULL)
    {
        pMatchingNode = m_pComplexTypeArray->getNodeByTypeAndNameDescending(eNodeType, pName);
    }

    return pMatchingNode;
}

CElement* CElement::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath, bool bIsInXSD)
{
    assert(pSchemaRoot != NULL);
    assert(pParentNode != NULL);

    if (pSchemaRoot == NULL || pParentNode == NULL)
    {
        return NULL;
    }

    CElement *pElement = new CElement(pParentNode);

    pElement->setIsInXSD(bIsInXSD);
    pElement->setXSDXPath(xpath);

//  CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXSDXPathToElement(xpath, pElement);

    IPropertyTree *pTree = pSchemaRoot->queryPropTree(xpath);

    if (pElement != NULL && pTree != NULL)
    {
        pElement->setName(pTree->queryProp(XML_ATTR_NAME));
    }

    Owned<IAttributeIterator> iterAttrib = pTree->getAttributes(true);

    ForEach(*iterAttrib)
    {
        if (strcmp(iterAttrib->queryName(), XML_ATTR_NAME) == 0)
        {
            const char *pName = iterAttrib->queryValue();

            if (pName != NULL && *pName != 0)
            {
                pElement->setName(pName);
                //CConfigSchemaHelper::getInstance()->getSchemaMapManager()->setElementWithName(pName, pElement);
            }
        }
        else if (strcmp(iterAttrib->queryName(), XML_ATTR_MAXOCCURS) == 0)
        {
            pElement->setMaxOccurs(iterAttrib->queryValue());
        }
        else if (strcmp(iterAttrib->queryName(), XML_ATTR_MINOCCURS) == 0)
        {
            pElement->setMinOccurs(iterAttrib->queryValue());
        }
        else if (strcmp(iterAttrib->queryName(), XML_ATTR_TYPE) == 0)
        {
            const char *pType = iterAttrib->queryValue();

            assert(pType != NULL && *pType != 0);

            if (pType != NULL && *pType != 0)
            {
                pElement->setType(pType);
                CConfigSchemaHelper::getInstance()->addNodeForTypeProcessing(pElement);
            }
        }
        else if (strcmp(iterAttrib->queryName(), XML_ATTR_REF) == 0)
        {
            const char *pRef = iterAttrib->queryValue();

            assert (pRef != NULL && *pRef != 0 && pElement->getConstAncestorNode(2)->getNodeType() != XSD_SCHEMA);

            if (pRef != NULL && *pRef != 0 && pElement->getConstAncestorNode(2)->getNodeType() != XSD_SCHEMA)
            {
                pElement->setRef(pRef);
                CConfigSchemaHelper::getInstance()->addElementForRefProcessing(pElement);
            }
            else
            {
                // TODO:  throw exception
            }
        }

        assert(iterAttrib->queryValue() != NULL);
    }

    assert(strlen(pElement->getName()) > 0);

    StringBuffer strXPathExt(xpath);

    strXPathExt.append("/").append(XSD_TAG_ANNOTATION);
    pElement->m_pAnnotation = CAnnotation::load(pElement, pSchemaRoot, strXPathExt.str());

    strXPathExt.clear().append(xpath).append("/").append(XSD_TAG_COMPLEX_TYPE);
    pElement->m_pComplexTypeArray = CComplexTypeArray::load(pElement, pSchemaRoot, strXPathExt.str());

    strXPathExt.clear().append(xpath).append("/").append(XSD_TAG_ATTRIBUTE);
    pElement->m_pAttributeArray = CAttributeArray::load(pElement, pSchemaRoot, strXPathExt.str());

    if (pElement->m_pAnnotation != NULL && pElement->m_pAnnotation->getAppInfo() != NULL && strlen(pElement->m_pAnnotation->getAppInfo()->getTitle()) > 0)
    {
        /****  MUST FIX TO HAVE CORRECT UI TAB LABELS (but getName is expected to return the XPATH name *****/
        //pElement->setName(pElement->m_pAnnotation->getAppInfo()->getTitle());
        pElement->setTitle(pElement->m_pAnnotation->getAppInfo()->getTitle());
    }
    else
    {
        pElement->setTitle(pElement->getName());
    }

    strXPathExt.clear().append(xpath).append("/").append(XSD_TAG_KEY);
    pElement->m_pKeyArray = CKeyArray::load(pElement, pSchemaRoot, strXPathExt.str());

    strXPathExt.clear().append(xpath).append("/").append(XSD_TAG_KEYREF);
    pElement->m_pKeyRefArray = CKeyRefArray::load(pElement, pSchemaRoot, strXPathExt.str());

    SETPARENTNODE(pElement, pParentNode);

    return pElement;
}

const CElement* CElement::getTopMostElement(const CXSDNodeBase *pNode)
{
    if (pNode == NULL)
    {
        return NULL;
    }
    else if (pNode->getNodeType() == XSD_ELEMENT)
    {
        if (pNode->getParentNodeByType(XSD_ELEMENT) == NULL)
        {
            assert(dynamic_cast<const CElement*>(pNode) != NULL);

            return dynamic_cast<const CElement*>(pNode);
        }
    }

    return getTopMostElement(pNode->getParentNodeByType(XSD_ELEMENT));
}

const CSchema* CElement::getConstSchemaNode() const
{
    const CSchema *pSchema = dynamic_cast<const CSchema*>(CElement::getTopMostElement(this)->getParentNodeByType(XSD_SCHEMA));

    return pSchema;
}

/*const CXSDNodeBase* CElement::getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const
{
    for (int idx = 0; idx < m_pAttributeArray->length(); idx++)
    {
        const CAttribute *pAttribute  = m_pAttributeArray->item(idx);

        assert(pAttrib != NULL);

        if (pName == NULL || *pName == 0)
        {
            return pAttribute;
        }

        if (pAttribute->getName() == NULL || *(pAttribute->getName()) == 0)
        {
            assert(!"Attribute needs name");
            return NULL;
        }

        if (strcmp(pNamepAttribute->getName(), pName) == 0)
        {
            return pAttribute;
        }

        // not found yet.  descend into complextypes
        pAttribute = dynamic_cast<CAttribute*>(this->getNodeByTypeAndNameDescending(eNodeType, pName));

        return pAttribute;
    }
}*/

const char* CElement::getXML(const char* /*pComponent*/)
{
    if (m_strXML.length () == 0)
    {
        m_strXML.append("\n<").append(getName()).append(" ");

        if (m_pAnnotation != NULL)
        {
            m_strXML.append(m_pAnnotation->getXML(NULL));
        }
        if (m_pComplexTypeArray != NULL)
        {
            m_strXML.append(m_pComplexTypeArray->getXML(NULL));
        }
        if (m_pAttributeArray != NULL)
        {
            m_strXML.append(m_pAttributeArray->getXML(NULL));
        }
        if (m_pKeyArray != NULL)
        {
            m_strXML.append(m_pKeyArray->getXML(NULL));
        }
        if (m_pKeyRefArray != NULL)
        {
            m_strXML.append(m_pKeyRefArray->getXML(NULL));
        }

//        m_strXML.append("/>\n");
    }

    return m_strXML.str();
}

void CElement::dump(std::ostream &cout, unsigned int offset) const
{
    offset += STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_ELEMENT_STR, offset);

    QUICK_OUT(cout, Name, offset);
    QUICK_OUT(cout, Type, offset);
    QUICK_OUT(cout, MinOccurs, offset);
    QUICK_OUT(cout, MaxOccurs, offset);
    QUICK_OUT(cout, Title,  offset);
    QUICK_OUT(cout, XSDXPath,  offset);
    QUICK_OUT(cout, EnvXPath,  offset);
    QUICK_OUT(cout, EnvValueFromXML,  offset);
    QUICK_OUT(cout, Ref, offset);

    if (this->getTypeNode() != NULL)
    {
        this->getTypeNode()->dump(cout, offset);
    }

    if (m_pAnnotation != NULL)
    {
        m_pAnnotation->dump(cout, offset);
    }

    if (m_pComplexTypeArray != NULL)
    {
        m_pComplexTypeArray->dump(cout, offset);
    }

    if (m_pAttributeArray != NULL)
    {
        m_pAttributeArray->dump(cout, offset);
    }

    if (m_pKeyArray != NULL)
    {
        m_pKeyArray->dump(cout, offset);
    }

    if (m_pKeyRefArray != NULL)
    {
        m_pKeyRefArray->dump(cout, offset);
    }

    if (this->getRef() != NULL)
    {
        CElement *pElement = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getElementWithName(this->getRef());

        assert(pElement != NULL);

        if (pElement != NULL)
        {
            pElement->dump(cout, offset);
        }
    }

    QuickOutFooter(cout, XSD_ELEMENT_STR, offset);
}

void CElement::getDocumentation(StringBuffer &strDoc) const
{
    const CXSDNodeBase *pGrandParentNode = this->getConstParentNode()->getConstParentNode();

    assert(pGrandParentNode != NULL);

    if (pGrandParentNode == NULL)
    {
        return;
    }

    if (m_pAnnotation != NULL && m_pAnnotation->getAppInfo() != NULL && m_pAnnotation->getAppInfo()->getViewType() != NULL && stricmp(m_pAnnotation->getAppInfo()->getViewType(), "none") == 0)
    {
        return;
    }

    if (this->getName() != NULL && (stricmp(this->getName(), "Instance") == 0 || stricmp(this->getName(), "Note") == 0 || stricmp(this->getName(), "Notes") == 0 ||  stricmp(this->getName(), "Topology") == 0 ))
    {
        return; // don't document instance
    }

    assert(strlen(this->getName()) > 0);

    if (pGrandParentNode->getNodeType() == XSD_SCHEMA)
    {
        StringBuffer strName(this->getName());

        strName.replace(' ', '_');

        strDoc.append(DM_HEADING);

        // component name would be here
        strDoc.appendf("<%s %s=\"%s%s\">\n", DM_SECT2, DM_ID, strName.str(),"_mod");
        strDoc.appendf("<%s>%s</%s>\n", DM_TITLE_LITERAL, this->getName(), DM_TITLE_LITERAL);

        if (m_pAnnotation!= NULL)
        {
            m_pAnnotation->getDocumentation(strDoc);
            DEBUG_MARK_STRDOC;
        }

        strDoc.append(DM_SECT3_BEGIN);
        DEBUG_MARK_STRDOC;
        strDoc.append(DM_TITLE_BEGIN).append(DM_TITLE_END);

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getDocumentation(strDoc);
        }

        strDoc.append(DM_SECT3_END);
        return;
    }
    else if (m_pComplexTypeArray != NULL)
    {
        if (m_pAnnotation!= NULL)
        {
            m_pAnnotation->getDocumentation(strDoc);
            DEBUG_MARK_STRDOC;
        }

        if (pGrandParentNode->getNodeType() == XSD_CHOICE)
        {
            strDoc.appendf("%s%s%s", DM_PARA_BEGIN, this->getName(), DM_PARA_END);
        }
        else
        {
            strDoc.appendf("%s%s%s", DM_TITLE_BEGIN, this->getName(), DM_TITLE_END);
        }

        DEBUG_MARK_STRDOC;

        m_pComplexTypeArray->getDocumentation(strDoc);
    }
    else if (m_pComplexTypeArray == NULL)
    {
        if (m_pAnnotation!= NULL)
        {
            m_pAnnotation->getDocumentation(strDoc);
            DEBUG_MARK_STRDOC;
        }

        strDoc.appendf("%s%s%s", DM_PARA_BEGIN, this->getName(), DM_PARA_END);
        DEBUG_MARK_STRDOC;

        if (m_pAnnotation != NULL && m_pAnnotation->getDocumentation() != NULL)
        {
            m_pAnnotation->getDocumentation(strDoc);
            DEBUG_MARK_STRDOC;
        }

        if (m_pAttributeArray != NULL)
        {
            m_pAttributeArray->getDocumentation(strDoc);
        }
    }
}

void CElement::getDojoJS(StringBuffer &strJS) const
{
    //TODO: Handle tree view
    if (stricmp(this->getType(), "NodeType") == 0)
    {
        return;
    }

    const CXSDNodeBase *pGrandParentNode = this->getConstAncestorNode(2);

    assert(pGrandParentNode != NULL);

    if (pGrandParentNode == NULL)
    {
        return;
    }

    if (m_pAnnotation != NULL && m_pAnnotation->getAppInfo() != NULL && m_pAnnotation->getAppInfo()->getViewType() != NULL)
    {
        if (stricmp(m_pAnnotation->getAppInfo()->getViewType(), "none") == 0)
        {
            return;
        }
        /*else if (stricmp(m_pAnnotation->getAppInfo()->getViewType(), "list") == 0)
        {
            strJS.append("if (cp != null) dojo.place(\"<div><H1>").append(this->getName()).append("</H1></div>\", cp.containerNode, cp.containerNode.length);\n");
          //  return;
        }*/
    }

    assert(strlen(this->getName()) > 0);

    if (pGrandParentNode->getNodeType() == XSD_SCHEMA)
    {
        strJS.append(DJ_START);
        DEBUG_MARK_STRJS;

        if (m_pAnnotation != NULL)
        {
            m_pAnnotation->getDojoJS(strJS);
        }

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getDojoJS(strJS);
        }

        strJS.append(CConfigSchemaHelper::getInstance()->getToolTipJS());
        strJS.append(DJ_FINISH);

        return;
    }
    else if (CDojoHelper::IsElementATab(this) == false)
    {
        strJS.append(DJ_LAYOUT_BEGIN);
        DEBUG_MARK_STRJS;

        if (m_pAnnotation != NULL && m_pAnnotation->getAppInfo() != NULL && stricmp(m_pAnnotation->getAppInfo()->getViewType(), "list") == 0)
        {
            strJS.append(DJ_DIV_HEADING_BEGIN).append(this->getTitle()).append(DJ_DIV_HEADING_END);
            DEBUG_MARK_STRJS;
        }

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getDojoJS(strJS);
        }

        if (this->getConstAncestorNode(2)->getNodeType() == XSD_SEQUENCE)
        {
            strJS.append(DJ_LAYOUT_CONCAT_BEGIN);
            DEBUG_MARK_STRJS;
            strJS.append(createDojoColumnLayout(this->getTitle(), getRandomID()));
            DEBUG_MARK_STRJS;
            strJS.append(DJ_LAYOUT_CONCAT_END);
            DEBUG_MARK_STRJS;
        }

        strJS.append(DJ_LAYOUT_END);
        DEBUG_MARK_STRJS;
    }
    else if (m_pComplexTypeArray != NULL)
    {
        if (m_pAnnotation!= NULL)
        {
            m_pAnnotation->getDojoJS(strJS);
        }

        if (pGrandParentNode->getNodeType() == XSD_CHOICE)
        {
            //strJS.append(DJ_LAYOUT_END);
            //DEBUG_MARK_STRJS;
        }
        else
        {

        }

        m_pComplexTypeArray->getDojoJS(strJS);

        //strJS.append(DJ_LAYOUT_END);
        //DEBUG_MARK_STRJS;
    }
    else if (m_pComplexTypeArray == NULL)
    {
        if (m_pAnnotation!= NULL)
        {
            m_pAnnotation->getDojoJS(strJS);
        }

        if (CDojoHelper::IsElementATab(this) == true)
        {
            genTabDojoJS(strJS, this->getTitle());
        }

        if (m_pAnnotation != NULL && m_pAnnotation->getDocumentation() != NULL)
        {
            m_pAnnotation->getDojoJS(strJS);
        }

        if (m_pAttributeArray != NULL)
        {
            m_pAttributeArray->getDojoJS(strJS);
        }

        strJS.append(DJ_TABLE_PART_2);
    }
}

#ifdef _USE_OLD_GET_QML_
void CElement::getQML(StringBuffer &strQML, int idx) const
{
    //TODO: Handle tree view
    if (stricmp(this->getType(), "NodeType") == 0)
    {
        return;
    }

    const CXSDNodeBase *pGrandParentNode = this->getConstAncestorNode(2);

    assert(pGrandParentNode != NULL);

    if (pGrandParentNode == NULL)
    {
        return;
    }

    if (m_pAnnotation != NULL && m_pAnnotation->getAppInfo() != NULL && m_pAnnotation->getAppInfo()->getViewType() != NULL)
    {
        if (stricmp(m_pAnnotation->getAppInfo()->getViewType(), "none") == 0)
        {
            return;
        }
    }

    assert(strlen(this->getTitle()) > 0);

    if (pGrandParentNode->getNodeType() == XSD_SCHEMA)
    {
        strQML.append(QML_START);
        DEBUG_MARK_QML;
        strQML.append(QML_TAB_VIEW_BEGIN);
        DEBUG_MARK_QML;

        /*if (m_pAnnotation != NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getQML(strQML);
        }*/

        strQML.append(QML_TAB_VIEW_STYLE);
        DEBUG_MARK_QML;
        strQML.append(QML_TAB_VIEW_END);
        DEBUG_MARK_QML;
        strQML.append(QML_TAB_TEXT_STYLE);
        DEBUG_MARK_QML;
        strQML.append(QML_END);
        DEBUG_MARK_QML;

        return;
    }
    else if (CDojoHelper::IsElementATab(this) == false) // TODO: Fix this HACK
    {
        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getQML(strQML);
        }

        if (this->getConstAncestorNode(2)->getNodeType() == XSD_SEQUENCE  && this->m_pComplexTypeArray == NULL)
        {
            strQML.append(QML_ROW_BEGIN);
            DEBUG_MARK_QML;

            strQML.append(QML_TABLE_VIEW_BEGIN);
            DEBUG_MARK_QML;

            strQML.append(QML_MODEL).append(modelNames[CConfigSchemaHelper::getInstance(0)->getNumberOfTables()]).append(QML_STYLE_NEW_LINE);
            DEBUG_MARK_QML;

            const CElement *pElement = dynamic_cast<const CElement*>(this->getParentNodeByType(XSD_ELEMENT));
            assert(pElement != NULL);

            strQML.append(QML_PROPERTY_STRING_TABLE_BEGIN).append(modelNames[CConfigSchemaHelper::getInstance()->getNumberOfTables()]).append(QML_PROPERTY_STRING_TABLE_PART_1).append(pElement->getXSDXPath()).append(QML_PROPERTY_STRING_TABLE_END);
            DEBUG_MARK_QML;

            CQMLMarkupHelper::getTableViewColumn(strQML, this->getTitle(), this->getEnvXPath());
            DEBUG_MARK_QML;

            CConfigSchemaHelper::getInstance()->incTables();

            strQML.append(QML_TABLE_VIEW_END);
            DEBUG_MARK_QML;

            strQML.append(QML_ROW_END);
            DEBUG_MARK_QML;
        }
    }
    else if (m_pComplexTypeArray != NULL)
    {
        if (m_pAnnotation != NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (idx < 1)
        {
            CQMLMarkupHelper::getTabQML(strQML, this->getTitle());
            DEBUG_MARK_QML;
        }
            strQML.append(QML_GRID_LAYOUT_BEGIN_1);
            DEBUG_MARK_QML;
//        }

        m_pComplexTypeArray->getQML(strQML);

        strQML.append(QML_GRID_LAYOUT_END);
        DEBUG_MARK_QML;


        if (idx < 1)
        {
//            strQML.append(QML_GRID_LAYOUT_END);
//            DEBUG_MARK_QML;

            strQML.append(QML_TAB_END);
            DEBUG_MARK_QML;
        }
    }
    else if (m_pComplexTypeArray == NULL)
    {
        if (m_pAnnotation!= NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (m_pAnnotation != NULL && m_pAnnotation->getDocumentation() != NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (CDojoHelper::IsElementATab(this) == true)
        {
            CQMLMarkupHelper::getTabQML(strQML, this->getTitle());
            DEBUG_MARK_QML;

            strQML.append(QML_GRID_LAYOUT_BEGIN);
            DEBUG_MARK_QML;
        }

        if (m_pAttributeArray != NULL)
        {
            m_pAttributeArray->getQML(strQML);
        }

        if (CDojoHelper::IsElementATab(this) == true)
        {
            strQML.append(QML_GRID_LAYOUT_END);
            DEBUG_MARK_QML;

            strQML.append(QML_TAB_END);
            DEBUG_MARK_QML;
        }
    }
}
#else

void CElement::getQML(StringBuffer &strQML, int idx) const
{
    // Handle HPCC Specific tag
    if (m_pAnnotation != NULL && m_pAnnotation->getAppInfo() != NULL && m_pAnnotation->getAppInfo()->getViewType() != NULL)
    {
        if (stricmp(m_pAnnotation->getAppInfo()->getViewType(), "none") == 0)
        {
            return;
        }
    }

    if (this->isTopLevelElement())  // handle qml imports etc...
    {
        strQML.append(QML_TAB_VIEW_BEGIN);
        DEBUG_MARK_QML;

        if (m_pAnnotation != NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getQML(strQML);
        }

        strQML.append(QML_TAB_VIEW_STYLE);
        DEBUG_MARK_QML;
        strQML.append(QML_TAB_VIEW_END);
        DEBUG_MARK_QML;
        //strQML.append(QML_TAB_TEXT_STYLE);
        //DEBUG_MARK_QML

        return;
    }
    else if (this->isATab())  // Tabs will be made for all elements in a sequence
    {
        if (idx == 0)
        {
            strQML.append(QML_TAB_VIEW_BEGIN);
            DEBUG_MARK_QML;
        }
        CQMLMarkupHelper::getTabQML(strQML, this->getTitle());
        DEBUG_MARK_QML;


        if (m_pAnnotation != NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getQML(strQML,idx);
        }

        strQML.append(QML_FLICKABLE_HEIGHT).append(CQMLMarkupHelper::getImplicitHeight() * 1.5);
        DEBUG_MARK_QML;
        strQML.append(QML_FLICKABLE_END);
        DEBUG_MARK_QML;
        strQML.append(QML_TAB_END);
        DEBUG_MARK_QML;

        if (static_cast<CElementArray*>(this->getParentNode())->length()-1 == idx)
        {
            strQML.append(QML_TAB_VIEW_HEIGHT).append(CQMLMarkupHelper::getImplicitHeight());
            DEBUG_MARK_QML;
            strQML.append(QML_TAB_VIEW_STYLE);
            DEBUG_MARK_QML;
            strQML.append(QML_TAB_VIEW_END);
            DEBUG_MARK_QML;
            //strQML.append(QML_TAB_TEXT_STYLE);
            //DEBUG_MARK_QML;
            //strQML.append(QML_TAB_VIEW_END);
            //DEBUG_MARK_QML;
        }
    }
    /*else
    {
        strQML.append(QML_GRID_LAYOUT_BEGIN_1);
        DEBUG_MARK_QML;

        if (m_pAnnotation != NULL)
        {
            m_pAnnotation->getQML(strQML);
        }

        if (m_pComplexTypeArray != NULL)
        {
            m_pComplexTypeArray->getQML(strQML,idx);
        }

        strQML.append(QML_GRID_LAYOUT_END);
        DEBUG_MARK_QML;
    }*/

}

bool CElement::isATab() const
{
/*    if (stricmp(this->getMaxOccurs(), TAG_UNBOUNDED) == 0)
    {
        return false;
    }
    // Any element that is in sequence of complex type will be a tab
    else*/ if (this->getConstAncestorNode(2)->getNodeType() == XSD_SEQUENCE && this->getConstAncestorNode(3)->getNodeType() == XSD_COMPLEX_TYPE)
    {
        return true;
    }
    else if (/*this->getConstAncestorNode(2)->getNodeType == XSD_COMPLEX_TYPE &&*/ this->getConstAncestorNode(3)->getNodeType() == XSD_ELEMENT)
    {
        const CElement *pElement = dynamic_cast<const CElement*>(this->getConstAncestorNode(3));

        assert(pElement != NULL);

        if (pElement != NULL)
        {
            return pElement->isATab();
        }

    }
    return false;
}

bool CElement::isLastTab(const int idx) const
{
    assert(this->isATab() == true);

    const CElementArray *pElementArray = dynamic_cast<const CElementArray*>(this->getConstParentNode());

    if (pElementArray == NULL)
    {
        assert(!"Corrupt XSD??");
        return false;
    }

    if (pElementArray->length()-1 == idx)
    {
        return true;
    }
}

#endif // _USE_OLD_GET_QML_

void CElement::populateEnvXPath(StringBuffer strXPath, unsigned int index)
{
    assert(strXPath.length() > 0);

    strXPath.append("/").append(this->getName()).append("[").append(index).append("]");

    PROGLOG("Setting element to envpath of %s, previous path: %s", strXPath.str(), this->getEnvXPath());
    this->setEnvXPath(strXPath);


    //strXPath.setLength(strXPath.length()-3);  // remove [N] from XPath;
    //strXPath.appendf("[%d]", this->getParentIndex()+1);

    if (m_pComplexTypeArray != NULL)
    {
        m_pComplexTypeArray->populateEnvXPath(strXPath, index);
    }
    if (m_pAttributeArray != NULL)
    {
        m_pAttributeArray->populateEnvXPath(strXPath, index);
    }
}

void CElement::loadXMLFromEnvXml(const IPropertyTree *pEnvTree)
{
    //PROGLOG("Mapping element with XPATH of %s to %p", this->getEnvXPath(), this);
    CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXPathToElement(this->getEnvXPath(), this);


/*    if (this->getConstAncestorNode(2)->getNodeType() == XSD_SCHEMA)
    {
        m_bTopLevelElement = true;
    }*/

    if (m_pComplexTypeArray != NULL)
    {
        try
        {
            m_pComplexTypeArray->loadXMLFromEnvXml(pEnvTree);
        }
        catch (...)
        {
            // node described in XSD doesn't exist in XML
            // time to do validation?
        }
    }
    if (m_pAttributeArray != NULL)
    {
        try
        {
            m_pAttributeArray->loadXMLFromEnvXml(pEnvTree);
        }
        catch (...)
        {
        }
    }

    if (m_pAttributeArray == NULL && m_pComplexTypeArray == NULL)
    {
        const char* pValue =  pEnvTree->queryPropTree(this->getEnvXPath())->queryProp("");

        if (pValue != NULL)
        {
            this->setEnvValueFromXML(pValue);

//            CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXPathToElement(this->getEnvXPath(), this);
            CConfigSchemaHelper::getInstance()->appendElementXPath(this->getEnvXPath());
        }
    }

    const char* pInstanceName =  pEnvTree->queryPropTree(this->getEnvXPath())->queryProp(XML_ATTR_NAME);

    if (pInstanceName != NULL && *pInstanceName != 0)
    {
        this->setInstanceName(pInstanceName);
    }
}

bool CElement::isTopLevelElement() const
{
    //const CElementArray *pElemArray = dynamic_cast<const CElementArray*>(this->getConstParentNode());
    /*const CElementArray *pElemArray = static_cast<const CElementArray*>(this->getConstParentNode());

    assert(pElemArray != NULL);

    const CElement *pElem = static_cast<CElement*>(&(pElemArray->item(0)));
    if (pElem->m_bTopLevelElement == true && strcmp(pElem->getXSDXPath(), this->getXSDXPath()) == 0 && pElem->m_bTopLevelElement == true)
    {
        return true;
    }
    else
    {
        return false;
    }

    //*/return m_bTopLevelElement;
}

void CElementArray::dump(std::ostream &cout, unsigned int offset) const
{
    offset+= STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_ELEMENT_ARRAY_STR, offset);

    QUICK_OUT(cout, XSDXPath,  offset);
    QUICK_OUT(cout, EnvXPath,  offset);
    QUICK_OUT_ARRAY(cout, offset);

    QuickOutFooter(cout, XSD_ELEMENT_ARRAY_STR, offset);
}

void CElementArray::getDocumentation(StringBuffer &strDoc) const
{
    QUICK_DOC_ARRAY(strDoc);
}

void CElementArray::getDojoJS(StringBuffer &strDoc) const
{
    QUICK_DOJO_JS_ARRAY(strDoc);
}

void CElementArray::getQML(StringBuffer &strQML, int idx) const
{
    //if (idx == -1)
    {
        for (int idx=0; idx < this->length(); idx++)
        {
            bool bIsTab =  this->item(idx).isTopLevelElement();

            if (/*bIsTab ==*/ true)
            {
#ifdef _USE_OLD_GET_QML_
                (this->item(idx)).getQML(strQML, 0);
#else
                if ((this->item(idx)).getIsInXSD() == true)
                {
                    (this->item(idx)).getQML(strQML, idx);
                }
#endif
            }
            else
            {
                //(this->item(idx)).getQML(strQML, 1);
            }
        }
    }
    /*else
    {
        if (idx < this->length())
        {
            (this->item(idx)).getQML(strQML);
        }
    }*/
}

void CElementArray::populateEnvXPath(StringBuffer strXPath, unsigned int index)
{
    assert(index == 1);  // Only 1 array of elements per node

    //strXPath.append("[1]");
    this->setEnvXPath(strXPath);

    //StringBuffer mapKey(strXPath);

    int elemCount = 1;

    for (int idx=0; idx < this->length(); idx++)
    {
//        if ((this->item(idx)).getIsInXSD() == true)
        //if (idx == 0)
        {
  //          elemCount = 1;
//            mapKey.setf("%s[%d]", this->getXSDXPath(), elemCount);

//            CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXSDXPathToElementArray(mapKey.str(), this);
        }
        //(this->item(idx)).populateEnvXPath(strXPath, elemCount++);
        if (this->item(idx).getIsInXSD() == true)
        {
            elemCount == 1;
        }
        else
        {
            elemCount++;
        }

        this->item(idx).populateEnvXPath(strXPath, elemCount);

        //mapKey.setf("%s[%d]", this->getXSDXPath(), index);

    }
}

const char* CElementArray::getXML(const char* /*pComponent*/)
{
    if (m_strXML.length() == 0)
    {
        int length = this->length();

        for (int idx = 0; idx < length; idx++)
        {
            CElement &Element = this->item(idx);

            m_strXML.append(Element.getXML(NULL));

            if (idx+1 < length)
            {
                m_strXML.append("\n");
            }
        }
    }

    return m_strXML.str();
}

void CElementArray::loadXMLFromEnvXml(const IPropertyTree *pEnvTree)
{
    int nUniqueElements = this->length();

    for (int idx = 0; idx < nUniqueElements; idx++)
    {
        StringBuffer strXPath(this->item(idx).getEnvXPath());

        if (pEnvTree->hasProp(strXPath.str()) == false)
        {
            continue;
        }

        int subIndex = 1;

        do
        {
            StringBuffer strEnvXPath(this->item(idx).getEnvXPath());
            CConfigSchemaHelper::stripXPathIndex(strEnvXPath);

            strEnvXPath.appendf("[%d]",subIndex);

            if (pEnvTree->queryPropTree(strEnvXPath.str()) == NULL)
            {
                break;
            }

            CElement *pElement = NULL;
            if (subIndex > 1)
            {
                pElement = CElement::load(this, this->getSchemaRoot(), this->item(idx).getXSDXPath(), false);

                //int nIndexOfElement =  (static_cast<CElementArray*>(pElement->getParentNode()))->getSiblingIndex(pElement->getXSDXPath(), pElement)+1;
                int nIndexOfElement =  (static_cast<CElementArray*>(pElement->getParentNode()))->getCountOfSiblingElements(pElement->getXSDXPath())+1;
                pElement->populateEnvXPath(this->getEnvXPath(), subIndex);

                //StringBuffer strXSDXPath(pElement->getXSDXPath());
                //CConfigSchemaHelper::stripXPathIndex(strXSDXPath);

                //strXSDXPath.appendf("[%d]",subIndex);
                //pElement->setXSDXPath(strXSDXPath.str());

                PROGLOG("XSD Xpath to non-native element to %s", pElement->getXSDXPath());
                PROGLOG("XML Xpath to non-native element to %s", pElement->getEnvXPath());

                pElement->setTopLevelElement(false);

                //StringBuffer strMapKey;
                //strMapKey.setf("%s[%d]", this->getXSDXPath(), index);

                CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXPathToElement(pElement->getEnvXPath(), pElement);
                CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXSDXPathToElementArray(pElement/*->getConstParentNode()*/->getXSDXPath(), (static_cast<CElementArray*>(pElement->getParentNode())));

                this->append(*pElement);
                PROGLOG("Added element %p with xsd xpath=%s array is size=%d with xpath of %s", pElement, pElement->getXSDXPath(),this->length(), this->getXSDXPath());
            }
            else
            {
                pElement = &(this->item(idx));

                if (pElement->getConstAncestorNode(2)->getNodeType() == XSD_SCHEMA)
                {
                    pElement->setTopLevelElement(true);
                }

                CConfigSchemaHelper::getInstance()->getSchemaMapManager()->addMapOfXSDXPathToElementArray(pElement/*->getConstParentNode()*/->getXSDXPath(), (static_cast<CElementArray*>(pElement->getParentNode())));
                PROGLOG("Added element %p with xsd xpath=%s array is size=%d with xpath of %s", pElement, pElement->getXSDXPath(),this->length(), this->getXSDXPath());
                //this->append(*pElement);
            }
            pElement->loadXMLFromEnvXml(pEnvTree);

            subIndex++;
        }
        while (true);
    }
}

CElementArray* CElementArray::load(const char* pSchemaFile)
{
    assert(pSchemaFile != NULL);

    if (pSchemaFile == NULL)
    {
        return NULL;
    }

    Linked<IPropertyTree> pSchemaRoot;

    StringBuffer schemaPath;

    schemaPath.appendf("%s%s", DEFAULT_SCHEMA_DIRECTORY, pSchemaFile);

    pSchemaRoot.setown(createPTreeFromXMLFile(schemaPath.str()));

    CElementArray *pElemArray = CElementArray::load(NULL, pSchemaRoot, XSD_TAG_ELEMENT);

    PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
    PROGLOG("pElemArray = %p", pElemArray);

    return pElemArray;
}

CElementArray* CElementArray::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    CElementArray *pElemArray = new CElementArray(pParentNode);

    assert(pElemArray != NULL);

    pSchemaRoot->Link();
    pElemArray->setSchemaRoot(pSchemaRoot);

    StringBuffer strXPathExt(xpath);
    pElemArray->setXSDXPath(xpath);

    Owned<IPropertyTreeIterator> elemIter = pSchemaRoot->getElements(xpath);

    int count = 1;

    ForEach(*elemIter)
    {
        strXPathExt.set(xpath);
        strXPathExt.appendf("[%d]", count);

        CElement *pElem = CElement::load(pElemArray, pSchemaRoot, strXPathExt.str());

        assert(pElem);

        pElemArray->append(*pElem);

        count++;
    }

    if (pElemArray->length() == 0)
    {
        delete pElemArray;
        return NULL;
    }

    SETPARENTNODE(pElemArray, pParentNode);

    return pElemArray;
}

int CElementArray::getCountOfSiblingElements(const char *pXPath) const
{
    assert(pXPath != NULL && *pXPath != 0);

    int count = 0;

    for (int idx=0; idx < this->length(); idx++)
    {
        if (strcmp(this->item(idx).getXSDXPath(), pXPath) == 0)
        {
            count++;
        }
    }

    return count;
}

const CXSDNodeBase* CElementArray::getNodeByTypeAndNameAscending(NODE_TYPES eNodeType, const char *pName) const
{
    assert(pName != NULL);

    for (int idx = 1; idx < this->length() && eNodeType == XSD_ELEMENT; idx++)
    {
        if (strcmp ((static_cast<CElement>(this->item(idx))).getName(), pName) == 0)
        {
            return &(this->item(idx));
        }
    }

    return (this->getParentNode()->getNodeByTypeAndNameAscending(eNodeType, pName));
}

const CXSDNodeBase* CElementArray::getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const
{
    assert(pName != NULL);

    if (eNodeType == this->getNodeType())// && (pName != NULL ? !strcmp(pName, this->getNodeTypeStr()) : true))
    {
        return this;
    }

/*    for (int idx = 1; idx < this->length() && eNodeType == XSD_ELEMENT; idx++)
    {
        if (strcmp ((static_cast<CElement*>(this->item(idx)))->getName(), pName) == 0)
        {
            return this->item(idx);
        }
    }*/

    return (this->getParentNode()->getNodeByTypeAndNameDescending(eNodeType, pName));
}

const CElement* CElementArray::getElementByNameAscending(const char *pName) const
{
    for (int idx = 1; idx < this->length() ;idx++)
    {
        if (strcmp ((static_cast<CElement>(this->item(idx))).getName(), pName) == 0)
        {
            return &(this->item(idx));
        }
    }
}

const CElement* CElementArray::getElementByNameDescending(const char *pName) const
{
    for (int idx = 1; idx < this->length(); idx++)
    {
        if (strcmp ((static_cast<CElement>(this->item(idx))).getName(), pName) == 0)
        {
            return &(this->item(idx));
        }
    }

    return NULL;
}

int CElementArray::getSiblingIndex(const char* pXSDXPath, const CElement* pElement)
{
    assert(pXSDXPath != NULL && *pXSDXPath != 0 && pElement != NULL);

    int nSiblingIndex = 0;

    for (int idx=0; idx < this->length(); idx++)
    {
        if (strcmp(this->item(idx).getXSDXPath(), pXSDXPath) == 0)
        {
            if (&(this->item(idx)) == pElement)
            {
                break;
            }
            nSiblingIndex++;
        }
    }

    return nSiblingIndex;
}
