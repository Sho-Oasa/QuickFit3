/*
Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate: 2014-06-22 12:12:54 +0200 (So, 22 Jun 2014) $  (revision $Rev: 3280 $)

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

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

#include "qfsimpleplotview.h"
#include "ui_qfsimpleplotview.h"

QFSimplePlotView::QFSimplePlotView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QFSimplePlotView)
{
    setWindowFlags(windowFlags()|Qt::WindowMinMaxButtonsHint);
    ui->setupUi(this);
}

QFSimplePlotView::~QFSimplePlotView()
{
    delete ui;
}


void QFSimplePlotView::readSettings(QSettings &/*settings*/, const QString &/*prefix*/)
{
    connectParameterWidgets(false);

    connectParameterWidgets(true);
}

void QFSimplePlotView::writeSettings(QSettings &/*settings*/, const QString &/*prefix*/)
{

}

void QFSimplePlotView::writeQFProperties(QFProperties */*current*/, const QString &/*prefix*/, const QString &/*egroup*/, const QString &/*param*/)
{

}

void QFSimplePlotView::readQFProperties(QFProperties */*current*/, const QString &/*prefix*/, const QString &/*egroup*/, const QString &/*param*/)
{
    connectParameterWidgets(false);

    connectParameterWidgets(true);
}

void QFSimplePlotView::connectParameterWidgets(bool /*connectTo*/)
{

}
