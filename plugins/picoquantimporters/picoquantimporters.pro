#
#    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg
#	
#	 
#
#    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).
#
#    This software is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

TEMPLATE = lib
CONFIG += plugin

TARGET = importers_picoquant
DEPENDPATH += ./

include(../../qf3plugins.pri)
DESTDIR = $$QFOUTPUT/plugins

# Input
HEADERS += qfpicoquantimporters.h \
           qftcspcreaderpicoquant.h \
    picoquant_t3r_tools.h \
    picoquant_pt3_tools.h \
    qftcspcreaderpicoquantpt3.h \
    picoquant_ptu_tools.h \
    qftcspcreaderpicoquantptu.h

SOURCES += qfpicoquantimporters.cpp\
           qftcspcreaderpicoquant.cpp \
    picoquant_t3r_tools.cpp \
    picoquant_pt3_tools.cpp \
    qftcspcreaderpicoquantpt3.cpp \
    picoquant_ptu_tools.cpp \
    qftcspcreaderpicoquantptu.cpp

FORMS =

RESOURCES +=

TRANSLATIONS= ./translations/de.importers_picoquant.ts


INCLUDEPATH += ../../lib/


include(../../quickfit3_configqt.pri)
