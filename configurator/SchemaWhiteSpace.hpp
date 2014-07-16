#ifndef _SCHEMA_WHITE_SPACE_HPP_
#define _SCHEMA_WHITE_SPACE_HPP_

#include "SchemaCommon.hpp"

class CWhiteSpace : public CXSDNode
{
public:

    virtual ~CWhiteSpace()
    {
    }

    static CWhiteSpace* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath);

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

    virtual void loadXMLFromEnvXml(const IPropertyTree *pEnvTree)
    {
        assert(!"Not Implemented");
    }

    GETTERSETTER(Value)

private:

    CWhiteSpace() : CXSDNode::CXSDNode(NULL, XSD_WHITE_SPACE), m_pWhiteSpace(-1)
    {
    }

    int m_nWhiteSpace;

};

#endif // _SCHEMA_WHITE_SPACE_HPP_
