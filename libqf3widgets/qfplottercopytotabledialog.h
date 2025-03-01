/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef QFPLOTTERCOPYTOTABLEDIALOG_H
#define QFPLOTTERCOPYTOTABLEDIALOG_H

#include "qfdialog.h"
#include "qfrdrtableinterface.h"
#include "qfrdrcolumngraphsinterface.h"
#include "qfrawdatarecord.h"
#include "libwid_imexport.h"
#include "qfrdrtablecombobox.h"

namespace Ui {
    class QFPlotterCopyToTableDialog;
}

class QFWIDLIB_EXPORT QFPlotterCopyToTableDialog : public QFDialog
{
        Q_OBJECT
        
    public:
        explicit QFPlotterCopyToTableDialog(QWidget *parent = 0);
        ~QFPlotterCopyToTableDialog();

        QFRDRTableInterface* getTable() const;
        QFRawDataRecord* getRDR() const;
        QFRDRColumnGraphsInterface* getGraph() const;
        int getGraphID() const;
        bool getNewGraph(QString& name) const;
        bool getNewTable(QString& name) const;
        bool getAddGraphs() const;
        bool getShowEditor() const;
        void setDescription(const QString& text);
        bool copyOnlyPlotData() const;
        QString getPrefix();
        void addWidget(const QString& label, QWidget* wid);

    protected slots:
        void on_cmbRDR_refilled(bool full);
        void on_cmbGraph_refilled(bool full);
        void on_chkNewTable_toggled(bool checked);
        void on_chCreateNewGraph_toggled(bool checked);
    private:
        Ui::QFPlotterCopyToTableDialog *ui;
};

#endif // QFPLOTTERCOPYTOTABLEDIALOG_H
