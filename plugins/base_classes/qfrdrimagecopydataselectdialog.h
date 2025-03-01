/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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



#ifndef QFRDRImageCOPYDATASELECTDIALOG_H
#define QFRDRImageCOPYDATASELECTDIALOG_H

#include "qfdialog.h"

namespace Ui {
    class QFRDRImageCopyDataSelectDialog;
}

class QFRDRImageCopyDataSelectDialog : public QFDialog
{
    Q_OBJECT

public:
    explicit QFRDRImageCopyDataSelectDialog(QWidget *parent = 0);
    ~QFRDRImageCopyDataSelectDialog();

    bool copyParam() const;
    bool copyOvf() const;
    bool copyParam2() const;
    bool copyMask() const;
    bool copyID() const;
    bool copyCoordinates() const;
private:
    Ui::QFRDRImageCopyDataSelectDialog *ui;
};

#endif // QFRDRImageCOPYDATASELECTDIALOG_H
