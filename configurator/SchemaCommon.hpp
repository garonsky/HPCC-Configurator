#ifndef _SCHEMA_COMMON_HPP_
#define _SCHEMA_COMMON_HPP_


#include <iostream>
#include "jiface.hpp"
#include "jstring.hpp"
#include "jlib.hpp"
#include "jlog.hpp"
#include "ExceptionStrings.hpp"


#define MINIMUM_STRICTNESS  0
#define DEFAULT_STRICTNESS  5
#define MAXIMUM_STRICTNESS  10
#define STRICTNESS_LEVEL MINIMUM_STRICTNESS

#define QUICK_OUT(X,Y,Z) QuickOut(X,#Y,get##Y(),Z);
#define QUICK_OUT_2(Y) QuickOut(cout, #Y, get##Y(), offset);
#define QUICK_OUT_3(X) if (m_p##X != NULL) m_p##X->dump(cout, offset);
#define QUICK_OUT_ARRAY(X,Z) for (int idx=0; idx < this->length(); idx++)               \
                             {                                                          \
                                QuickOutPad(X,Z+STANDARD_OFFSET_1);                     \
                                X << idx+1 << "]" << std::endl;                         \
                                (this->item(idx)).dump(cout,Z);                         \
                             }

#define QUICK_DOC_ARRAY(X) for (int idx=0; idx < this->length(); idx++)                 \
                           {                                                            \
                                (this->item(idx)).getDocumentation(X);                  \
                           }

#define QUICK_DOJO_JS_ARRAY(X) for (int idx=0; idx < this->length(); idx++)             \
                           {                                                            \
                                (this->item(idx)).getDojoJS(X);                         \
                           }

#define QUICK_QML_ARRAY(X) for (int idx=0; idx < this->length(); idx++)                 \
                           {                                                            \
                                (this->item(idx)).getQML(X);                            \
                           }
#define QUICK_QML_ARRAY2(X)     for (int idx=0; idx < this->length(); idx++)            \
                                {                                                       \
                                    (this->item(idx)).getQML(X, idx);                   \
                                }

#define QUICK_ENV_XPATH(X) for (int idx=0; idx < this->length(); idx++)                 \
                            {                                                           \
                                 (this->item(idx)).populateEnvXPath(X, idx+1);          \
                            }

#define QUICK_LOAD_ENV_XML(X)   assert(X != NULL);                                        \
                                for (int idx=0; idx < this->length(); idx++)              \
                                {                                                         \
                                     (this->item(idx)).loadXMLFromEnvXml(X);              \
                                }

#define QUICK_TRAVERSE_AND_PROCESS  for (int idx=0; idx < this->length(); idx++)        \
{                                                                                       \
    CXSDNodeBase::processEntryHandlers(this);                                           \
    this->item(idx).traverseAndProcessNodes();                                          \
    CXSDNodeBase::processExitHandlers(this);                                            \
}


#define GETTER(X) virtual const char* get##X() const { return m_str##X.str(); }
#define SETTER(X) virtual void set##X(const char* p) { m_str##X.clear().append(p); }
#define GETTERSETTER(X) protected: StringBuffer m_str##X; public: GETTER(X) SETTER(X) public:

#define GETTERINT(X) virtual const long get##X() const { return m_n##X; }
#define SETTERINT(X) virtual void set##X(long p) { m_n##X = p; } virtual void set##X(const char *p) { assert(p != NULL); if (p != 0 && *p != 0) m_n##X = atol(p); }
#define GETTERSETTERINT(X) protected: long m_n##X; public: GETTERINT(X) SETTERINT(X) public:

#define SETPARENTNODE(X, Y) if (X!= NULL && Y != NULL) X->setParentNode(Y);
//#define DEBUG_MARK_STRDOC strDoc.append(__FILE__).append(":").append(__LINE__).append("\n");
#define DEBUG_MARK_STRDOC
#define DEBUG_MARK_COMMENT(X) X.append("//  ").append(__FILE__).append(":").append(__LINE__).append("\n");
#define DEBUG_MARK_STRJS DEBUG_MARK_COMMENT(strJS)
#define DEBUG_MARK_QML DEBUG_MARK_COMMENT(strQML)
//#define DEBUG_MARK_QML
//#define DEBUG_MARK_STRJS

#define GETTERTYPE(X) C##X* get##X() { return m_p##X; }
#define SETTERTYPE(X) void set##X( C##X *p ) { assert(p != NULL); if (p != NULL) m_p##X = p; }
#define GETTERSETTERTYPE(X) private: C##X *m_p##X; GETTERTYPE(X) SETTERTYPE(X)

enum NODE_TYPES
{
    XSD_ANNOTATION = 0x1,
    XSD_APP_INFO,
    XSD_ATTRIBUTE,
    XSD_ATTRIBUTE_ARRAY,
    XSD_ATTRIBUTE_GROUP,
    XSD_ATTRIBUTE_GROUP_ARRAY,
    XSD_CHOICE,
    XSD_COMPLEX_CONTENT,
    XSD_COMPLEX_TYPE,
    XSD_COMPLEX_TYPE_ARRAY,
    XSD_DOCUMENTATION,
    XSD_ELEMENT,
    XSD_ELEMENT_ARRAY,
    XSD_EXTENSION,
    XSD_INCLUDE,
    XSD_INCLUDE_ARRAY,
    XSD_RESTRICTION,
    XSD_SCHEMA,
    XSD_SEQUENCE,
    XSD_SIMPLE_TYPE,
    XSD_SIMPLE_TYPE_ARRAY,
    XSD_ENUMERATION,
    XSD_ENUMERATION_ARRAY,
    XSD_LENGTH,
    XSD_FRACTION_DIGITS,
    XSD_MAX_EXCLUSIVE,
    XSD_MAX_INCLUSIVE,
    XSD_MIN_EXCLUSIVE,
    XSD_MIN_INCLUSIVE,
    XSD_MIN_LENGTH,
    XSD_MAX_LENGTH,
    XSD_PATTERN,
    XSD_TOTAL_DIGITS,
    XSD_WHITE_SPACE,
    XSD_DT_NORMALIZED_STRING,
    XSD_DT_STRING,
    XSD_DT_TOKEN,
    XSD_DT_DATE,
    XSD_DT_TIME,
    XSD_DT_DATE_TIME,
    XSD_DT_INTEGER,
    XSD_DT_LONG,
    XSD_DT_NON_NEG_INTEGER,
    XSD_DT_NON_POS_INTEGER,
    XSD_DT_POS_INTEGER,
    XSD_DT_NEG_INTEGER,
    XSD_ERROR
};

static const char* DEFAULT_SCHEMA_DIRECTORY("/opt/HPCCSystems/componentfiles/configxml/");

static const char* XSD_TAG_ANNOTATION("xs:annotation");
static const char* XSD_TAG_APP_INFO("xs:appinfo");
//static const char* XSD_TAG_ATTRIBUTE("xs:attribute");
//static const char* XSD_TAG_ATTRIBUTE_GROUP("xs:attributeGroup");
//static const char* XSD_TAG_CHOICE("xs:choice");
//static const char* XSD_TAG_COMPLEX_CONTENT("xs:complexContent");
//static const char* XSD_TAG_COMPLEX_TYPE("xs:complexType");
static const char* XSD_TAG_DOCUMENTATION("xs:documentation");
//static const char* XSD_TAG_ELEMENT("xs:element");
static const char* XSD_TAG_EXTENSION("xs:extension");
static const char* XSD_TAG_INCLUDE("xs:include");
static const char* XSD_TAG_RESTRICTION("xs:restriction");
static const char* XSD_TAG_SCHEMA("");//xs:schema");
//static const char* XSD_TAG_SEQUENCE("xs:sequence");
static const char* XSD_TAG_SIMPLE_TYPE("xs:simpleType");
static const char* XSD_TAG_ENUMERATION("xs:enumeration");
static const char* XSD_TAG_FRACTION_DIGITS("xs:fractionDigits");
static const char* XSD_TAG_LENGTH("xs:length");
static const char* XSD_TAG_MAX_EXCLUSIVE("xs:maxExclusive");
static const char* XSD_TAG_MAX_INCLUSIVE("xs:maxInclusive");
static const char* XSD_TAG_MIN_EXCLUSIVE("xs:minExlusive");
static const char* XSD_TAG_MIN_INCLUSIVE("xs:minExclusive");
static const char* XSD_TAG_MAX_LENGTH("xs:maxLength");
static const char* XSD_TAG_MIN_LENGTH("xs:minLength");
static const char* XSD_TAG_PATTERN("xs:pattern");
static const char* XSD_TAG_TOTAL_DIGITS("xs:totalDigits");
static const char* XSD_TAG_WHITE_SPACE("xs:whiteSpace");

static const char* XSD_DATA_TYPE_NORMALIZED_STRING("xs:normalizedString");
static const char* XSD_DATA_TYPE_STRING("xs:string");
static const char* XSD_DATA_TYPE_TOKEN("xs:token");
static const char* XSD_DATA_TYPE_DATE("xs:date");
static const char* XSD_DATA_TYPE_TIME("xs:time");
static const char* XSD_DATA_TYPE_DATE_TIME("xs:dateTime");
static const char* XSD_DATA_TYPE_DECIMAL("xs:decimal"); // A decimal value
static const char* XSD_DATA_TYPE_INTEGER("xs:integer"); // An integer value
static const char* XSD_DATA_TYPE_INT("xs:int"); // A signed 32-bit integer
static const char* XSD_DATA_TYPE_LONG("xs:long"); // A signed 64-bit integer
static const char* XSD_DATA_TYPE_NON_NEGATIVE_INTEGER("xs:nonNegativeInteger");
static const char* XSD_DATA_TYPE_NON_POSITIVE_INTEGER("xs:nonPositiveInteger");
static const char* XSD_DATA_TYPE_NEGATIVE_INTEGER("xs:negativeInteger");
static const char* XSD_DATA_TYPE_POSITIVE_INTEGER("xs:positiveInteger");


static const char* XSD_ANNOTATION_STR("Annotation");
static const char* XSD_APP_INFO_STR("AppInfo");
static const char* XSD_ATTRIBUTE_STR("Attribute");
static const char* XSD_ATTRIBUTE_ARRAY_STR("AttributeArray");
static const char* XSD_ATTRIBUTE_GROUP_STR("AttributeGroup");
static const char* XSD_ATTRIBUTE_GROUP_ARRAY_STR("AttributeGroupArray");
static const char* XSD_CHOICE_STR("Choice");
static const char* XSD_COMPLEX_CONTENT_STR("ComplexContent");
static const char* XSD_COMPLEX_TYPE_STR("ComplexType");
static const char* XSD_COMPLEX_TYPE_ARRAY_STR("ComplexTypeArray");
static const char* XSD_DOCUMENTATION_STR("Documentation");
static const char* XSD_ELEMENT_STR("Element");
static const char* XSD_ELEMENT_ARRAY_STR("ElementArray");
static const char* XSD_ERROR_STR("ERROR");
static const char* XSD_ENUMERATION_STR("Enumeration");
static const char* XSD_ENUMERATION_ARRAY_STR("EnumerationArray");
static const char* XSD_EXTENSION_STR("Extension");
static const char* XSD_FRACTION_DIGITS_STR("FractionDigits");
static const char* XSD_INCLUDE_STR("Include");
static const char* XSD_INCLUDE_ARRAY_STR("IncludeArray");
static const char* XSD_LENGTH_STR("Length");
static const char* XSD_MIN_INCLUSIVE_STR("MinInclusive");
static const char* XSD_MAX_INCLUSIVE_STR("MaxInclusive");
static const char* XSD_MIN_EXCLUSIVE_STR("MinExclusive");
static const char* XSD_MAX_EXCLUSIVE_STR("MaxExclusive");
static const char* XSD_MIN_LENGTH_STR("MinLength");
static const char* XSD_MAX_LENGTH_STR("MaxLength");
static const char* XSD_PATTERN_STR("Pattern");
static const char* XSD_RESTRICTION_STR("Restriction");
static const char* XSD_SCHEMA_STR("Schema");
static const char* XSD_SEQUENCE_STR("Sequence");
static const char* XSD_SIMPLE_TYPE_STR("SimpleType");
static const char* XSD_SIMPLE_TYPE_ARRAY_STR("SimpleTypeArray");
static const char* XSD_TOTAL_DIGITS_STR("TotalDigits");
static const char* XSD_WHITE_SPACE_STR("WhiteSpace");
static const char* XSD_DT_NORMALIZED_STRING_STR("NormalizedString");
static const char* XSD_DT_STRING_STR("String");
static const char* XSD_DT_TOKEN_STR("Token");
static const char* XSD_DT_DATE_STR("Date");
static const char* XSD_DT_TIME_STR("Time");
static const char* XSD_DT_DATE_TIME_STR("DateTime");
static const char* XSD_DT_INTEGER_STR("Integer");
static const char* XSD_DT_LONG_STR("Long");
static const char* XSD_DT_NON_NEG_INTEGER_STR("NonNegativeInteger");
static const char* XSD_DT_NON_POS_INTEGER_STR("NonPositiveInteger");
static const char* XSD_DT_POS_INTEGER_STR("PositiveInteger");
static const char* XSD_DT_NEG_INTEGER_STR("NegativeInteger");

static const char* XML_ENV_VALUE_OPTIONAL("optional");
static const char* XML_ENV_VALUE_REQUIRED("required");

static const char* XML_ATTR_DEFAULT("@default");
static const char* XML_ATTR_USE("@use");
static const char* XML_ATTR_MINOCCURS("@minOccurs");
static const char* XML_ATTR_BASE("@base");

static const char* TAG_VIEWCHILDNODES("viewChildNodes");
static const char* TAG_VIEWTYPE("viewType");
static const char* TAG_TOOLTIP("tooltip");
static const char* TAG_COLINDEX("colIndex");
static const char* TAG_TITLE("title");
static const char* TAG_WIDTH("width");
//static const char* TAG_TOOLTIP("tooltip");
static const char* TAG_AUTOGENWIZARD("autogenforwizard");
static const char* TAG_AUTOGENDEFAULTVALUE("autogendefaultvalue");
static const char* TAG_AUTOGENDEFAULTVALUEFORMULTINODE("autogendefaultformultinode");
static const char* TAG_XPATH("xpath");
static const char* TAG_DOC_ID("docid");

#define TAG_OPTIONAL                    "optional"
#define TAG_REQUIRED                    "required"
#define XML_ATTR_ATTRIBUTEFORMDEFAULT  "@attributeFormDefault"
#define XML_ATTR_ELEMENTFORMDEFAULT    "@elementFormDefault"
#define XML_ATTR_ID                    "@id"
#define XML_ATTR_REF                   "@ref"
#define XML_ATTR_XMLNS_XS              "@xmlns:xs"
#define XML_ATTR_SCHEMA_LOCATION       "@schemaLocation"
#define XML_ATTR_VALUE                 "@value"
#define XML_ATTR_OVERIDE               "@overide"
#define XML_ATTR_DEPLOYABLE            "@deployable"

static unsigned int STANDARD_OFFSET_1 = 3;
static unsigned int STANDARD_OFFSET_2 = 6;

static void QuickOutPad(std::ostream& cout, unsigned int offset)
{
    while(offset > 0)
    {
        cout << " ";
        offset--;
    }
}

static void QuickOutHeader(std::ostream &cout, const char* pLabel, unsigned int offset = 0)
{
    QuickOutPad(cout,offset);
    cout << "\033[32m-- " << pLabel << " START" << " --" << "\033[0m" << std::endl;
}

static void QuickOutFooter(std::ostream &cout, const char* pLabel, unsigned int offset = 0)
{
    QuickOutPad(cout,offset);
    //cout << "<--- FINISH " << pLabel << std::endl;
    cout << "\033[31m" << "-- " << pLabel << " FINISH" << " --" << "\033[0m" << std::endl;
}

static void QuickOut(std::ostream &cout, const char* pLabel, const char* pValue, unsigned int offset = 0)
{
    if (pLabel && strlen(pValue) > 0)
    {
        QuickOutPad(cout,offset+STANDARD_OFFSET_2);
        cout << "\033[34m" << pLabel << ":\t\033[0m" << "\033[34m'\033[0m" << pValue << "\033[34m'" << "\033[0m" << std::endl;
    }
}

static void QuickOut(std::ostream &cout, const char* pLabel, int value, unsigned int offset = 0)
{
    QuickOutPad(cout,offset);
    cout << pLabel << ": " << value << std::endl;
}

class InterfaceImpl : public IInterface
{
public:

    InterfaceImpl()
    {
        atomic_set(&xxcount, 1);
    }

    virtual void Link() const
    {
        atomic_inc(&xxcount);
    }

    virtual bool Release() const
    {
        if (atomic_dec_and_test(&xxcount))
        {
           delete this;
           return true;
        }

        return false;
    }

    int getLinkCount() const
    {
        return xxcount.counter;
    }

private:

    mutable atomic_t xxcount;
};


class CXSDNodeBase;

class CXSDNodeHandler : public CInterface
{
public:

    virtual void onEventEntry(const CXSDNodeBase *pNode) const = 0;
    virtual void onEventExit(const CXSDNodeBase *pNode) const = 0;

};

class CXSDNodeBase
{
public:

    CXSDNodeBase(CXSDNodeBase* pParentNode = NULL, NODE_TYPES eNodeType = XSD_ERROR);

    virtual ~CXSDNodeBase();

    GETTERSETTER(XSDXPath)
    GETTERSETTER(EnvXPath)
    GETTERSETTER(EnvValueFromXML)


    void dumpStdOut() const;

    virtual CXSDNodeBase* getParentNode() const
    {
        return m_pParentNode;
    }

    virtual const CXSDNodeBase* getConstAncestorNode(unsigned iLevel) const;

    virtual const CXSDNodeBase* getConstParentNode() const
    {
        return m_pParentNode;
    }

    virtual const CXSDNodeBase* getParentNodeByType(NODE_TYPES eNodeType[], const CXSDNodeBase *pParent = NULL, int length = 1) const;

    virtual const CXSDNodeBase* getParentNodeByType(NODE_TYPES eNodeType, const CXSDNodeBase *pParent = NULL) const
    {
        return getParentNodeByType(eNodeType, pParent);
    }

    virtual const CXSDNodeBase* getNodeByTypeAndNameAscending(NODE_TYPES eNodeType[], const char *pName, int length = 1) const;
    virtual const CXSDNodeBase* getNodeByTypeAndNameAscending(NODE_TYPES eNodeType, const char *pName) const
    {
        return getNodeByTypeAndNameAscending(&eNodeType, pName);
    }

    virtual const CXSDNodeBase* getNodeByTypeAndNameDescending(NODE_TYPES eNodeType[], const char *pName, int length = 1) const;
    virtual const CXSDNodeBase* getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const
    {
        return getNodeByTypeAndNameDescending(&eNodeType, pName);
    }

    void setParentNode(CXSDNodeBase *pParentNode)
    {
        if (m_pParentNode == NULL)  // Should only be set once, otherwise it's an external schema and should have parent set
        {
            m_pParentNode = pParentNode;
        }
    }

    const char* getNodeTypeStr() const
    {
        return m_pNodeType;
    }

    virtual NODE_TYPES getNodeType() const
    {
        return m_eNodeType;
    }

    virtual void dump(std::ostream& cout, unsigned int offset = 0) const = 0;

    virtual void traverseAndProcessNodes() const = 0;

    virtual const char* getXML(const char* /*pComponent*/)
    {
        return NULL;
    }

    virtual void getDocumentation(StringBuffer &strDoc) const = 0;

    virtual void getDojoJS(StringBuffer &strJS) const = 0;

    virtual void getQML(StringBuffer &strQML, int idx = -1) const
    {
    }

    virtual void populateEnvXPath(StringBuffer strXPath, unsigned int index = 1)
    {
    }

    virtual void loadXMLFromEnvXml(const IPropertyTree *pEnvTree)
    {
    }

    static void addEntryHandler(CXSDNodeHandler &Handler)
    {
        s_callBackEntryHandlersArray.append(Handler);
    }

    static void addExitHandler(CXSDNodeHandler &Handler)
    {
        s_callBackExitHandlersArray.append(Handler);
    }

protected:

    static void processEntryHandlers(const CXSDNodeBase *pBase);

    static void processExitHandlers(const CXSDNodeBase *pBase);

    CXSDNodeBase*               m_pParentNode;
    StringBuffer                m_strXML;
    NODE_TYPES                  m_eNodeType;
    char                        m_pNodeType[1024];
    static CIArrayOf<CXSDNodeHandler>  s_callBackEntryHandlersArray;
    static CIArrayOf<CXSDNodeHandler>  s_callBackExitHandlersArray;

private:

} __attribute__((aligned (32)));


class CXSDNode : public CInterface, public CXSDNodeBase
{
public:

    IMPLEMENT_IINTERFACE

    CXSDNode(CXSDNodeBase *pParentNode, NODE_TYPES pNodeType = XSD_ERROR );

    virtual bool checkSelf(NODE_TYPES eNodeType, const char *pName, const char* pCompName) const;

    virtual const CXSDNodeBase* getParentNodeByType(NODE_TYPES eNodeType) const;

    virtual const CXSDNodeBase* getNodeByTypeAndNameAscending(NODE_TYPES eNodeType, const char *pName) const;

    virtual const CXSDNodeBase* getNodeByTypeAndNameDescending(NODE_TYPES eNodeType, const char *pName) const;

private:

};

class CXSDBuiltInDataType : public CXSDNode
{
public:

    //static CXSDBuiltInDataType* load(CXSDNodeBase *pParentNode, const IPropertyTree *pSchemaRoot, const char*  xpath);

    virtual ~CXSDBuiltInDataType();

    virtual void dump(std::ostream& cout, unsigned int offset = 0) const;
    virtual void traverseAndProcessNodes() const = 0;
    virtual void getDocumentation(StringBuffer &strDoc) const;
    virtual void getDojoJS(StringBuffer &strJS) const;

private:

    CXSDBuiltInDataType(CXSDNodeBase* pParentNode = NULL, enum NODE_TYPES eNodeType = XSD_ERROR);
};

#endif // _SCHEMA_COMMON_HPP_
