#ifndef _SCHEMA_TOTAL_DIGITS_HPP_
#define _SCHEMA_TOTAL_DIGITS_HPP_

#include "SchemaCommon.hpp"

class CTotalDigits : public CXSDNode
{
public:

    virtual ~CTotalDigits()
    {
    }

    static CTotalDigits* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath);

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

    CTotalDigits() : CXSDNode::CXSDNode(NULL, XSD_TotalDigits), m_TotalDigits(-1)
    {
    }

    int m_nTotalDigits;

};

#endif // _SCHEMA_TOTAL_DIGITS_HPP_
