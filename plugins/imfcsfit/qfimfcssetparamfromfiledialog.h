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

#ifndef QFIMFCSSETPARAMFROMFILEDIALOG_H
#define QFIMFCSSETPARAMFROMFILEDIALOG_H

#include "qfdialog.h"
#include "qfrawdatarecord.h"
#include "qfplotter.h"
#include "qfimfcsfitevaluation.h"
#include "qfimagetransform.h"

namespace Ui {
    class QFImFCSSetParamFromFileDialog;
}



class QFImFCSSetParamFromFileDialog : public QFDialog
{
        Q_OBJECT
        
    public:
        explicit QFImFCSSetParamFromFileDialog(int width, int heighth, QFImFCSFitEvaluation* eval, QStringList parameters, QStringList parameterIDs, QWidget *parent = 0);
        ~QFImFCSSetParamFromFileDialog();
        QFRawDataRecord* getRDR() const;
        QString getEvalID() const;
        QString getResultID() const;
        QString getParameter() const;
        QVector<double> getData(bool *ok=NULL, int* width=NULL, int* height=NULL);
    protected slots:
        void replotOvr();
        void on_cmbParamter_currentIndexChanged(int index);
        void updateDataFromRDR();
        void on_btnHelp_clicked();
        void on_btnLoadFile_clicked();
    private:
        Ui::QFImFCSSetParamFromFileDialog *ui;
        QStringList parameterIDs;
        JKQTPColumnMathImage* plt;
        int desiredwidth;
        int desiredheight;
        int datawidth;
        int dataheight;
        QVector<double> data;
};

#endif // QFIMFCSSETPARAMFROMFILEDIALOG_H
