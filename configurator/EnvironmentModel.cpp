#include "EnvironmentModel.hpp"
#include "SchemaCommon.hpp"
#include "ConfigSchemaHelper.hpp"
#include "SchemaMapManager.hpp"
#include "SchemaElement.hpp"
#include "jlib.hpp"
#include "jlog.hpp"
#include <cassert>

#define LOG_CONSTRUCTOR

CEnvironmentModelNode::CEnvironmentModelNode(const CEnvironmentModelNode *pParent, int index,  CXSDNodeBase *pNode) : m_pParent(NULL), m_pArrChildNodes(NULL)
{
    //assert(index == 0); // index will be 0 for now. No support for multi-environments

    if (pParent == NULL && index == 0)  // if this is the 'Environment' Node
    {
        this->m_pXSDNode = NULL;
        this->m_pArrChildNodes = new PointerArray();  // array of ptrs to each component

        int nComps = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getNumberOfComponents();

        for (int idx = 0; idx < nComps; idx++)
        {
            CXSDNodeBase *pNode = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getComponent(idx);

            if (pNode == NULL)
            {
                continue;
            }

            assert(pNode != NULL);

            CEnvironmentModelNode *pModelNode = new CEnvironmentModelNode(this, 0, pNode);
            m_pArrChildNodes->append(pModelNode);
        }
    }
    else if (pParent != NULL && pParent->getParent() == NULL)  // component tier
    {
        assert(pParent != NULL);
        assert(pNode != NULL);

        this->m_pParent = pParent;
        this->m_pXSDNode = pNode;
        this->m_pArrChildNodes = new PointerArray();

        assert(m_pXSDNode->getNodeType() == XSD_ELEMENT);
        assert(m_pXSDNode->getConstParentNode()->getNodeType() == XSD_ELEMENT_ARRAY);

        //const CElementArray *pElementArray = static_cast<const CElementArray*>(m_pXSDNode->getConstParentNode());

        const CElement *pElement =  static_cast<const CElement*>(m_pXSDNode->getNodeByTypeAndNameDescending(XSD_ELEMENT, NULL));
        const CElementArray *pElementArray = pElement != NULL ? static_cast<const CElementArray*>(pElement->getParentNode()) : NULL;

        for (int idx = 0; (pElementArray != NULL && idx < pElementArray->length()); idx++)
        {
            CEnvironmentModelNode *pModelNode = new CEnvironmentModelNode(this, idx, &(pElementArray->item(idx)));
            m_pArrChildNodes->append(pModelNode);
        }
    }
    else// if (pParent != NULL && pParent->getParent() == NULL && pParent->getParent()->getParent() != NULL) // instance tier
    {
        assert(pParent != NULL);
        assert(pNode != NULL);

        this->m_pParent = pParent;
        this->m_pXSDNode = pNode;
        this->m_pArrChildNodes = new PointerArray();
    }
    /*else if (pParent != NULL)  // not 'environment' node
    {
        assert(pNode != NULL);
        assert(pNode->getNodeType() == XSD_ELEMENT);

        this->m_pParent = pParent;
        this->m_pArrChildNodes = new PointerArray();

        CElement *pElement = static_cast<CElement*>(pNode);

        CElementArray *pElementArray = static_cast<CElementArray*>(pElement->getParentNode());

        assert(pElementArray->length() > index);

        m_pXSDNode = &(pElementArray->item(index));

        if (pElementArray->length() > index+1)
        {
            const CXSDNodeBase *pNodeDescendent = m_pXSDNode->getNodeByTypeAndNameDescending(XSD_ELEMENT, NULL);

            CEnvironmentModelNode *pModelNode = NULL;
            pModelNode = new CEnvironmentModelNode(this, index+1, const_cast<CXSDNodeBase*>(pNodeDescendent));

            m_pParent->m_pArrChildNodes->append(pModelNode);
        }
    }
    else
    {
        assert(false);
    }*/
}

/*CEnvironmentModelNode::CEnvironmentModelNode(const CEnvironmentModelNode *pParent, int index) : m_pParent(NULL), m_pArrChildNodes(NULL)
{
    if (pParent == NULL)
    {

        m_pArrChildNodes = new PointerArray();

        int nComps = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getNumberOfComponents();
        this->m_pXSDNode = NULL;

#ifdef LOG_CONSTRUCTOR
        PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
        PROGLOG("pParent == NULL nComs = %d", nComps);
#endif // LOG_CONSTRUCTOR

        for (int idx = 0; idx < nComps; idx++)
        {
            CXSDNodeBase *pNode = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getComponent(idx);

            assert(pNode != NULL);
            this->m_pXSDNode = pNode;

            CEnvironmentModelNode *pModelNode = new CEnvironmentModelNode(this, idx);

            pModelNode->m_pXSDNode = pNode;

            m_pArrChildNodes->append(pModelNode);
        }
    }
    /*else if (pParent == CEnvironmentModel::getInstance())
    {
        CXSDNodeBase *pNode = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getComponent(idx);

        this->m_pXSDNode = pNode;

        assert(pNode != NULL);
    }*//*
    else
    {
        m_pParent = const_cast<CEnvironmentModelNode*>(pParent);

        CElement *pElement = static_cast<CElement*>(m_pParent->m_pXSDNode);

#ifdef LOG_CONSTRUCTOR
        PROGLOG("\tpElement != %p index = %d", pElement, index);
#endif // LOG_CONSTRUCTOR

        *//*if (pElement == NULL)
        {
            CXSDNodeBase *pNode = CConfigSchemaHelper::getInstance()->getSchemaMapManager()->getComponent(index);
            m_pXSDNode = pNode;
        }
        else
        {*//*
            assert(pElement->getNodeType() == XSD_ELEMENT);

            CElementArray *pElementArray = static_cast<CElementArray*>(pElement->getParentNode());

            assert(pElementArray->length() > index);

            m_pXSDNode = &(pElementArray->item(index));

#ifdef LOG_CONSTRUCTOR
            PROGLOG("m_pXSDNode = %p of type %s", m_pXSDNode, m_pXSDNode->getNodeTypeStr());
#endif // LOG_CONSTRUCTOR
        //}
    }
}*/

const CEnvironmentModelNode* CEnvironmentModelNode::getChild(int index) const
{
   assert(m_pArrChildNodes == NULL || m_pArrChildNodes->length() > index);

   if (m_pArrChildNodes != NULL)
   {
        CEnvironmentModelNode *pNode = static_cast<CEnvironmentModelNode*>(m_pArrChildNodes->item(index));

        return pNode;
   }

   return NULL;
}

CEnvironmentModelNode::~CEnvironmentModelNode()
{
    delete m_pArrChildNodes;
}

int CEnvironmentModelNode::getNumberOfChildren() const
{
    int nRetVal = 0;

    if (m_pArrChildNodes != NULL)
    {
        nRetVal = m_pArrChildNodes->length();
    }
    else if (m_pArrChildNodes == NULL && m_pXSDNode != NULL && m_pParent != (CEnvironmentModel::getInstance()->getRoot()))
    {
        return 0;
    }
    else
    {
        assert(this->getXSDNode()->getNodeType() == XSD_ELEMENT);

        const CElement *pElement = static_cast<const CElement*>(this->getXSDNode());

        const CElementArray *pElementArray = static_cast<const CElementArray*>(pElement->getConstParentNode());

        assert(pElementArray->getNodeType() == XSD_ELEMENT_ARRAY);

        nRetVal = pElementArray->getCountOfSiblingElements(pElement->getXSDXPath());
    }

    return nRetVal;
}

CEnvironmentModel* CEnvironmentModel::getInstance()
{
    static CEnvironmentModel *s_pCEnvModel = NULL;

    if (s_pCEnvModel == NULL)
    {
        s_pCEnvModel = new CEnvironmentModel();
    }

    return s_pCEnvModel;
}

CEnvironmentModel::CEnvironmentModel()
{
    m_pRootNode = new CEnvironmentModelNode(NULL);
}

CEnvironmentModel::~CEnvironmentModel()
{
    delete m_pRootNode;
}

const CEnvironmentModelNode* CEnvironmentModel::getParent(CEnvironmentModelNode *pChild)
{
    if (pChild != NULL)
    {
        return pChild->getParent();
    }

    assert(false);
    return NULL;
}

const CEnvironmentModelNode* CEnvironmentModel::getChild(CEnvironmentModelNode *pParent, int index)
{
    assert(index >= 0);
    assert(pParent != NULL);

    if (pParent == NULL)
    {
        return m_pRootNode;
    }

    assert(pParent->getNumberOfChildren() > index);

    return pParent->getChild(index);
}

const char* CEnvironmentModel::getData(const CEnvironmentModelNode *pChild) const
{
    assert(pChild != NULL);

    if (pChild == reinterpret_cast<const CEnvironmentModelNode*>(this))
    {
        return NULL;
    }

    const CElement *pElement = static_cast<const CElement*>(pChild->getXSDNode());

    if (pElement != NULL)
    {
        const char *pInstanceName = pElement->getInstanceName();

        if (pChild->getParent() != CEnvironmentModel::getInstance()->getRoot())
        {
            return pInstanceName;
        }
        else
        {
            return pElement->getName();
        }
    }
    else
    {
        return "Environment";
    }
}

const char* getInstanceName(const CEnvironmentModelNode *pChild)
{
    assert(pChild != NULL);

    const CElement *pElement = static_cast<const CElement*>(pChild->getXSDNode());

    if (pElement != NULL && pElement->isTopLevelElement() == true)
    {
        return pElement->getInstanceName();
    }
    else
    {
        return pElement->getName();
    }
}


int CEnvironmentModel::getNumberOfRootNodes() const
{
    return 1;
}

CEnvironmentModelNode* CEnvironmentModel::getRoot(int index)
{
    assert(m_pRootNode != 0);
    assert(index == 0);

    if (index != 0)
    {
        return NULL;
    }
    else
    {
        return m_pRootNode;
    }
}
