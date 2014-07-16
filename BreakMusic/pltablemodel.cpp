#include "pltablemodel.h"
#include <QSqlQuery>

PlTableModel::PlTableModel(QObject *parent, QSqlDatabase db) :
    QSqlRelationalTableModel(parent, db)
{
    m_playlistId = -1;
    setTable("plsongs");
    setRelation(1, QSqlRelation("playlists", "playlistid", "title"));
    setRelation(3, QSqlRelation("songs", "songid", "artist"));
    setRelation(4, QSqlRelation("songs", "songid", "title"));
    setRelation(5, QSqlRelation("songs", "songid", "filename"));
    setRelation(6, QSqlRelation("songs", "songid", "duration"));
    setRelation(7, QSqlRelation("songs", "songid", "path"));
    setHeaderData(2, Qt::Horizontal, "");
    setHeaderData(7, Qt::Horizontal, "");
    setSort(2, Qt::AscendingOrder);
}

void PlTableModel::moveSong(int oldPosition, int newPosition)
{
    QSqlQuery query;
    int plSongId = index(oldPosition,0).data().toInt();
    query.exec("BEGIN TRANSACTION");
    if (newPosition > oldPosition)
    {
        // Moving down
        QString sql = "UPDATE plsongs SET position = position - 1 WHERE position > " + QString::number(oldPosition) + " AND position <= " + QString::number(newPosition) + " AND plsongid != " + QString::number(plSongId);
        query.exec(sql);
        sql = "UPDATE plsongs SET position = " + QString::number(newPosition) + " WHERE plsongid == " + QString::number(plSongId);
        query.exec(sql);
    }
    else if (newPosition < oldPosition)
    {
        // Moving up
        QString sql = "UPDATE plsongs SET position = position + 1 WHERE position >= " + QString::number(newPosition) + " AND position < " + QString::number(oldPosition) + " AND plsongid != " + QString::number(plSongId);
        query.exec(sql);
        sql = "UPDATE plsongs SET position = " + QString::number(newPosition) + " WHERE plsongid == " + QString::number(plSongId);
        query.exec(sql);
    }
    query.exec("COMMIT TRANSACTION");
    select();
}

void PlTableModel::addSong(int songId)
{
    if (insertRow(rowCount())) {
        int newRow = rowCount() - 1;
        setData(index(newRow, 1), m_playlistId);
        setData(index(newRow, 2), newRow);
        setData(index(newRow, 3), songId);
        setData(index(newRow, 4), songId);
        setData(index(newRow, 5), songId);
        setData(index(newRow, 6), songId);
        setData(index(newRow, 7), songId);
        submitAll();
        select();
    }
}

void PlTableModel::insertSong(int songId, int position)
{
    addSong(songId);
    moveSong(rowCount() - 1, position);
}

void PlTableModel::deleteSong(int position)
{
    int plSongId = index(position,0).data().toInt();
    QSqlQuery query("DELETE FROM plsongs WHERE plsongid == " + QString::number(plSongId));
    query.exec("UPDATE plsongs SET position = position - 1 WHERE position > " + QString::number(position));
    select();
}

void PlTableModel::setCurrentPlaylist(int playlistId)
{
    m_playlistId = playlistId;
    setFilter("playlist=" + QString::number(m_playlistId));
    select();
}

int PlTableModel::currentPlaylist()
{
    return m_playlistId;
}

int PlTableModel::getSongIdByFilePath(QString filePath)
{
    QSqlQuery query("SELECT songid FROM songs WHERE path == \"" + filePath + "\" LIMIT 1");
    if (query.first())
        return query.value(0).toInt();

    return -1;
}

bool PlTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(column);

    if (data->hasFormat("integer/queuepos"))
    {
        int droprow;
        if (parent.row() >= 0)
            droprow = parent.row();
        else if (row >= 0)
            droprow = row;
        else
            droprow = rowCount() - 1;
        int oldPosition;
        QByteArray bytedata = data->data("integer/queuepos");
        oldPosition =  QString(bytedata.data()).toInt();
        if ((oldPosition < droprow) && (droprow != rowCount() - 1))
            moveSong(oldPosition, droprow - 1);
        else
            moveSong(oldPosition, droprow);
        return true;
    }
    if (data->hasFormat("integer/songid"))
    {
                unsigned int droprow;
                if (parent.row() >= 0)
                    droprow = parent.row();
                else if (row >= 0)
                    droprow = row;
                else
                    droprow = rowCount();
                int songid;
                QByteArray bytedata = data->data("integer/songid");
                songid = QString(bytedata.data()).toInt();
                insertSong(songid, droprow);
    }
    return false;
}

QStringList PlTableModel::mimeTypes() const
{
    QStringList types;
    types << "integer/songid";
    types << "integer/queuepos";
    return types;
}

bool PlTableModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);
    return true;
}

Qt::DropActions PlTableModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QMimeData *PlTableModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("integer/queuepos", indexes.at(0).sibling(indexes.at(0).row(), 2).data().toByteArray().data());
    return mimeData;
}


Qt::ItemFlags PlTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
}