#include "SchemaMinExclusive.hpp"

CMinExclusive* CMinExclusive::load(CXSDNodeBase* pParentNode, const IPropertyTree *pSchemaRoot, const char* xpath)
{
    CMinExclusive *pMinExclusive = CXSDNodeWithRestrictions::load(pParentNode, pSchemaRoot, xpath);

    if (pMinExclusive == NULL)
    {
        return NULL;
    }

    pMinExclusive->setMinExclusive(pMinExclusive->getValue());

    return pMinExclusive;
}

/*void CMinExclusive::dump(std::ostream& cout, unsigned int offset) const
{
    assert(!"Not Implemented");
}*/
