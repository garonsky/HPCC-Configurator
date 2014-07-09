#ifndef _SCHEMA_LENGTH_HPP_
#define _SCHEMA_LENGTH_HPP_

#include "SchemaCommon.hpp"

class CLength : public CXSDNode
{
public:

    static CLength* load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath);

protected:

private:

    CLength() : CXSDNode::CXSDNode(NULL, XSD_LENGTH)
    {
    }

};

#endif // _SCHEMA_LENGTH_HPP_
