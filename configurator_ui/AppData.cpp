#include "AppData.hpp"
#include "jlog.hpp"
#include <cassert>

static const int TOP_LEVEL = 1;
static void* P_TOP_LEVEL = (void*)(&TOP_LEVEL);

TableDataModel::TableDataModel()
{
}

int TableDataModel::rowCount(const QModelIndex& /*parent*/) const
{
    int nCount = CONFIGURATOR_API::getNumberOfRows(this->m_pActiveTable);

    return nCount;
}

int TableDataModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant TableDataModel::data(const QModelIndex & index, int role) const
{
    QHash<int, QByteArray> Roles = roleNames();

    const char *pValue  = CONFIGURATOR_API::getTableValue(Roles.value(role), index.row()+1);

    assert(pValue != NULL);

    return QString(pValue);
}

QHash<int, QByteArray> TableDataModel::roleNames() const
{
    static QHash<int, QByteArray> Roles;

    int nColumns = CONFIGURATOR_API::getNumberOfUniqueColumns();

    for (int idx = 0; idx < nColumns; idx++)
    {
        const char *pRole =  CONFIGURATOR_API::getColumnName(idx);
        Roles[Qt::UserRole + idx+1] = pRole;
    }

    return Roles;
}

ComponentDataModel::ComponentDataModel( QObject *parent) : QAbstractItemModel(parent)
{
}

ComponentDataModel::~ComponentDataModel()
{
}

int ComponentDataModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}


QVariant ComponentDataModel::data(const QModelIndex &index, int role) const
{

    if (index.isValid() == false)
    {
        PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
        PROGLOG("\tindex.row() = %d index.column() = %d index.internalPointer() = %p", index.row(), index.column(), index.internalPointer());
        PROGLOG("index.isValid() == false");
        return QVariant();
    }

    if (Qt::DisplayRole != role)
    {
        return QVariant();
    }

    if (index.column() != 0)
    {
        assert(false);
    }

    const char *pName = CONFIGURATOR_API::getData(index.internalPointer());

    PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
    PROGLOG("\tindex.row() = %d index.column() = %d index.internalPointer() = %p", index.row(), index.column(), index.internalPointer());
    PROGLOG("\tpName = %s", pName);

    if (pName == NULL)
    {
        assert(false);
        return QVariant();
    }

    return QString(pName);
}

Qt::ItemFlags ComponentDataModel::flags(const QModelIndex &index) const
{
    if (index.isValid() == false)
    {
        return 0;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ComponentDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QString("Components");
    return QAbstractItemModel::headerData(section, orientation, role);
}



QModelIndex ComponentDataModel::index(int row, int column, const QModelIndex & parent) const
{
    //PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
    //PROGLOG("\tparent.row() = %d parent.column() = %d parent.internalPointer() = %p number_of_children = %d", parent.row(), parent.column(), parent.internalPointer(),\
                CONFIGURATOR_API::getNumberOfChildren(parent.internalPointer()));

    assert(column == 0);
    if (hasIndex(row, column, parent) == false || column > 0)
    {
        return QModelIndex();
    }

    void *pParentNode = NULL;

    if (parent.isValid() == false)
    {
        pParentNode = CONFIGURATOR_API::getRootNode();
    }
    else
    {
        pParentNode = parent.internalPointer();
    }

    void *pChildNode = CONFIGURATOR_API::getChild(parent.internalPointer(), row);

    if (pChildNode != NULL)
    {
        return createIndex(row, column, pChildNode);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ComponentDataModel::parent(const QModelIndex & index) const
{
   // PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);
  //  PROGLOG("\tindex.row() = %d index.column() = %d index.internalPointer() = %p number_of_children = %d", index.row(), index.column(), index.internalPointer(),\
                CONFIGURATOR_API::getNumberOfChildren(index.internalPointer()));

    if (!index.isValid())
    {
        return QModelIndex();
    }

    void *pChildNode = index.internalPointer();
    void *pParentNode = NULL;

    if (pParentNode == CONFIGURATOR_API::getRootNode())
    {
        return QModelIndex();
    }

    pParentNode = CONFIGURATOR_API::getParent(pChildNode);

    /*if (pParentNode == CONFIGURATOR_API::getRootNode())
    {
        return QModelIndex();
    }*/

    if (pParentNode == NULL)
    {
        return QModelIndex();
    }
    int nParentRow = CONFIGURATOR_API::getIndexFromParent(pParentNode);

   // PROGLOG("\tcalling createIndex(%d, 0, %p)", nParentRow, pParentNode);
    return createIndex(nParentRow, 0, pParentNode);
}

int ComponentDataModel::rowCount(const QModelIndex &parent) const
{
    //PROGLOG("Function: %s() at %s:%d", __func__, __FILE__, __LINE__);


    int nRetVal = 0;

    if (parent.column() > 0)
    {
        return 0;
    }

    void *pParentNode = NULL;

    if (parent.isValid() == false)
    {
        pParentNode = CONFIGURATOR_API::getRootNode();
    }
    else
    {
        pParentNode = parent.internalPointer();
    }

    nRetVal = CONFIGURATOR_API::getNumberOfChildren(pParentNode);

    //PROGLOG("\tparent.row() = %d parent.column() = %d pParentNode = %p number_of_children = %d", parent.row(), parent.column(), pParentNode,\
                CONFIGURATOR_API::getNumberOfChildren(pParentNode));

    return nRetVal;
}
