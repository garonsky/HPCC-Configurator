#ifndef _SCHEMA_FRACTION_DIGITS_HPP_
#define _SCHEMA_FRACTION_DIGITS_HPP_

#include "SchemaCommon.hpp"

class CFractionDigits : public CXSDNode
{
public:

    virtual ~CFractionDigits()
    {
    }

    static CFractionDigits* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath);

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
        return NULL;
    }

    virtual void populateEnvXPath(StringBuffer strXPath, unsigned int index = 1)
    {
        assert(!"Not Implemented");
    }

    GETTERSETTER(Value)
    GETTERSETTERINT(MaxFractionDigits)

private:

    CFractionDigits() : CXSDNode::CXSDNode(NULL, XSD_FRACTION_DIGITS), m_pValue(NULL), m_nMaxFractionDigits(-1)
    {
    }
};

#endif
