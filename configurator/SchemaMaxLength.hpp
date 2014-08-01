#ifndef _SCHEMA_MAX_LENGTH_HPP_
#define _SCHEMA_MAX_LENGTH_HPP_

#include "SchemaCommon.hpp"

class CMaxLength : public CXSDNode
{
public:

    virtual ~CMaxLength()
    {
    }

    static CMaxLength* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath);

    virtual void dump(std::ostream& cout, unsigned int offset = 0) const;

    virtual void getDocumentation(StringBuffer &strDoc) const
    {
        assert(!"Not Implemented");
    }

    virtual void getDojoJS(StringBuffer &strJS) const
    {
        assert(!"Not Implemented");
    }

    void getQML(StringBuffer &strQML, int idx = -1) const
    {
        assert(!"Not Implemented");
    }

    virtual const char* getXML(const char* /*pComponent*/)
    {
        assert(!"Not Implemented");
    }

    virtual void populateEnvXPath(StringBuffer strXPath, unsigned int index = 1)
    {
        assert(!"Not Implemented");
    }

    GETTERSETTER(Value)
    GETTERSETTERINT(MaxLength)

private:

    CMaxLength(CXSDNodeBase* pParentNode = NULL) : CXSDNode::CXSDNode(pParentNode, XSD_MAX_LENGTH), m_nMaxLength(-1)
    {
    }
};

#endif // _SCHEMA_MAX_LENGTH_HPP_
