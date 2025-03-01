/*
Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef QFBASICIMAGEIMPORTERSOPTIONSWIDGET_H
#define QFBASICIMAGEIMPORTERSOPTIONSWIDGET_H

#include <QWidget>
#include "qfpluginoptionsdialog.h"

namespace Ui {
    class QFBasicImageImportersOptionsWidget;
}

class QFBasicImageImportersOptionsWidget : public QFPluginOptionsWidget
{
        Q_OBJECT
        
    public:
        explicit QFBasicImageImportersOptionsWidget(QObject* pluginObject, QWidget *parent = 0);
        ~QFBasicImageImportersOptionsWidget();
        
        /** \brief read the widget contents from the program options */
        void readSettings(ProgramOptions* options);
        /** \brief write the widget contents to the program options */
        void writeSettings(ProgramOptions* options);
    protected slots:
        void on_btnClearLibTIFFLog_clicked();
        void on_btnClearTinyTIFFLog_clicked();
    private:
        Ui::QFBasicImageImportersOptionsWidget *ui;
};

#endif // QFBASICIMAGEIMPORTERSOPTIONSWIDGET_H
