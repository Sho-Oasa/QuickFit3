/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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

#include "qfpluginrawdata.h"
#include <QDebug>

QFPluginRawDataRecordBase::QFPluginRawDataRecordBase() {
    //qDebug()<<"QFPluginRawDataRecordBase::QFPluginRawDataRecordBase()";
    services=NULL;
    project=NULL;
    settings=NULL;
    parentWidget=NULL;

}

QFPluginRawDataRecordBase::~QFPluginRawDataRecordBase() {

}

void QFPluginRawDataRecordBase::getQFLibVersion(int& major, int& minor) const { \
        major=QF3LIB_APIVERSION_MAJOR; \
        minor=QF3LIB_APIVERSION_MINOR;\
    }


void QFPluginRawDataRecordBase::setProject(QFProject* project) {
    this->project=project;
}

void QFPluginRawDataRecordBase::setServices(QFPluginServices* services) {
    this->services=services;
}

void QFPluginRawDataRecordBase::setSettings(ProgramOptions* settings) {
    this->settings=settings;
}

void QFPluginRawDataRecordBase::setParentWidget(QWidget* parentWidget) {
    this->parentWidget=parentWidget;
}
