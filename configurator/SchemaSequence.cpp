#include <cassert>
#include "jptree.hpp"
#include "XMLTags.h"
#include "SchemaSequence.hpp"
#include "SchemaElement.hpp"
#include "QMLMarkup.hpp"


const CXSDNodeBase* CSequence::getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const
{
    const CXSDNodeBase* pMatchingNode = NULL;

    if (eNodeType == this->getNodeType() && (pName != NULL ? !strcmp(pName, this->getNodeTypeStr()) : true))
    {
        return this;
    }

    if (p_mElementArray != NULL)
    {
        pMatchingNode = p_mElementArray->getNodeByTypeAndNameDescending(eNodeType, pName);
    }

    return pMatchingNode;
}

CSequence* CSequence::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    assert(pSchemaRoot != NULL);

    CSequence *pSequence = NULL;

    if (pSchemaRoot == NULL)
    {
        return NULL;
    }

    if (pSchemaRoot->queryPropTree(xpath) == NULL)
    {
        return NULL;  // no sequence node
    }

    StringBuffer strXPath(xpath);
    strXPath.append("/").append(XSD_TAG_ELEMENT);

    CElementArray *pElemArray = CElementArray::load(NULL, pSchemaRoot, strXPath.str());

    //PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
    //PROGLOG("pElementArray = %p", pElemArray);

    if (pElemArray != NULL)
    {
        pSequence = new CSequence(pParentNode, pElemArray);

        pSequence->setXSDXPath(xpath);
    }

    SETPARENTNODE(pElemArray, pSequence)

    return pSequence;
}

void CSequence::dump(std::ostream& cout, unsigned int offset) const
{
    offset+= STANDARD_OFFSET_1;

    QuickOutHeader(cout, XSD_SEQUENCE_STR, offset);

    QUICK_OUT(cout, XSDXPath,  offset);

    if (p_mElementArray != NULL)
    {
        p_mElementArray->dump(cout, offset);
    }

    QuickOutFooter(cout, XSD_SEQUENCE_STR, offset);
}

void CSequence::getDocumentation(StringBuffer &strDoc) const
{
    if (p_mElementArray != NULL)
    {
        p_mElementArray->getDocumentation(strDoc);
    }
}

void CSequence::getDojoJS(StringBuffer &strJS) const
{
    if (p_mElementArray != NULL)
    {
        p_mElementArray->getDojoJS(strJS);
    }
}

void CSequence::getQML(StringBuffer &strQML, int idx) const
{
    if (p_mElementArray != NULL)
    {
        p_mElementArray->getQML(strQML);
    }
}

void CSequence::getQML2(StringBuffer &strQML, int idx) const
{
    if (p_mElementArray != NULL)
    {
        this->setUIType(this->getParentNode()->getUIType());
        p_mElementArray->getQML2(strQML);
    }
}

void CSequence::populateEnvXPath(StringBuffer strXPath, unsigned int index)
{
    if (p_mElementArray != NULL)
    {
        p_mElementArray->populateEnvXPath(strXPath);
    }

    this->setEnvXPath(strXPath);
}

void CSequence::loadXMLFromEnvXml(const IPropertyTree *pEnvTree)
{
    if (p_mElementArray != NULL)
    {
        p_mElementArray->loadXMLFromEnvXml(pEnvTree);
    }
}

bool CSequence::hasChildElements() const
{
    if (this->p_mElementArray->length() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
