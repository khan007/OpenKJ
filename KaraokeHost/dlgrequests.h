/*
 * Copyright (c) 2013-2016 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KHREQUESTSDIALOG_H
#define KHREQUESTSDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include "requeststablemodel.h"
#include "dbtablemodel.h"
#include "dbitemdelegate.h"
#include "rotationmodel.h"
#include "dlgcdgpreview.h"

namespace Ui {
class DlgRequests;
}

class DlgRequests : public QDialog
{
    Q_OBJECT

private:
    Ui::DlgRequests *ui;
    RequestsTableModel *requestsModel;
    //SongDBTableModel *songDbModel;
    DbTableModel *dbModel;
    DbItemDelegate *dbDelegate;
    RotationModel *rotModel;
    DlgCdgPreview *cdgPreviewDialog;

public:
    explicit DlgRequests(RotationModel *rotationModel, QWidget *parent = 0);
    ~DlgRequests();

signals:
    void addRequestSong(int songId, int singerId);

private slots:
    void on_pushButtonClose_clicked();
    void requestsModified();
    void on_pushButtonSearch_clicked();
    void on_lineEditSearch_returnPressed();
    void requestSelectionChanged(const QItemSelection & current, const QItemSelection & previous);
    void songSelectionChanged(const QItemSelection & current, const QItemSelection & previous);
    void on_radioButtonExistingSinger_toggled(bool checked);
    void on_pushButtonClearReqs_clicked();
    void on_treeViewRequests_clicked(const QModelIndex &index);
    void on_pushButtonAddSong_clicked();
    void on_tableViewSearch_customContextMenuRequested(const QPoint &pos);
    void updateReceived(QTime updateTime);
    void on_buttonRefresh_clicked();
    void authError();
    void sslError();
    void delayError(int seconds);
    void on_checkBoxAccepting_clicked(bool checked);

};

#endif // KHREQUESTSDIALOG_H
