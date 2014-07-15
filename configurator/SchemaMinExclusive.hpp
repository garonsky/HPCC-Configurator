#ifndef _SCHEMA_MIN_EXCLUSIVE_HPP_
#define _SCHEMA_MIN_EXCLUSIVE_HPP_

#include "SchemaCommon.hpp"

class CMinExclusive : public CXSDNode
{
public:

    virtual ~CMinExclusive()
    {
    }

    static CMinExclusive* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath);

    //GETTERSETTER(V)

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


protected:

private:

    CMinExclusive() : CXSDNode::CXSDNode(NULL, XSD_MinExclusive), m_MinExclusive(-1)
    {
    }

    int m_nMinExclusive;

};

#endif // _SCHEMA_MIN_EXCLUSIVE_HPP_
