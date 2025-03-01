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

#include "qffitfunctionvalueinputdelegate.h"
#include "qffitfunctionvalueinputtable.h"
#include "qfevaluationitem.h"
#include "qfproject.h"
#include "qfrawdatarecord.h"
#include "qfpluginservices.h"
#include "qfdoubleedit.h"
#include "qfautooutputtimer.h"
#include "qffitfunction.h"
#include "qffitfunctionmanager.h"
#include "float.h"
#include <QScopedPointer>

QFFitFunctionValueInputDelegate::QFFitFunctionValueInputDelegate(QObject *parent) :
    QFHTMLDelegate(parent)
{
}

void QFFitFunctionValueInputDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFHTMLDelegate::paint(painter, option, index);
}

QSize QFFitFunctionValueInputDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QFHTMLDelegate::sizeHint(option, index);
}


QWidget *QFFitFunctionValueInputDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    QElapsedTimer t;
    t.start();
    QVariant widgetType=index.data(QFFitFunctionValueInputTable::widgetTypeRole);
    QVariant ffID=index.data(QFFitFunctionValueInputTable::fitFunctionIDRole);
    QVariant fpID=index.data(QFFitFunctionValueInputTable::fitParameterIDRole);
    QVariant fpMin=index.data(QFFitFunctionValueInputTable::fitParameterMinRole);
    QVariant fpMax=index.data(QFFitFunctionValueInputTable::fitParameterMaxRole);
    if (widgetType.isValid()) {
        QScopedPointer<QFFitFunction> ff(QFPluginServices::getInstance()->getFitFunctionManager()->createFunction(ffID.toString()));
        if (fpID.isValid() && (widgetType.toInt()==QFFitFunctionValueInputTable::wtValueDoubleEdit || widgetType.toInt()==QFFitFunctionValueInputTable::wtValueLogDoubleEdit || widgetType.toInt()==QFFitFunctionValueInputTable::wtRangeEditMin || widgetType.toInt()==QFFitFunctionValueInputTable::wtRangeEditMax || widgetType.toInt()==QFFitFunctionValueInputTable::wtValueIntEdit)) {
            QFDoubleEdit* edt=new QFDoubleEdit(parent);
            edt->setIntegerWidget(widgetType.toInt()==QFFitFunctionValueInputTable::wtValueIntEdit);
            if (widgetType.toInt()==QFFitFunctionValueInputTable::wtValueLogDoubleEdit) {
                edt->setLogScale(true, 10);
            }
            if (widgetType.toInt()==QFFitFunctionValueInputTable::wtRangeEditMin || widgetType.toInt()==QFFitFunctionValueInputTable::wtRangeEditMax) {
                edt->setCheckBounds(false, false);
            } else if (fpMin.isValid()||fpMax.isValid()){

                bool okmi=false, okma=false;
                edt->setRange(fpMin.toDouble(&okmi), fpMax.toDouble(&okma));
                edt->setCheckBounds(okmi, okma);
            } else if (ff && ff->hasParameter(fpID.toString())){
                edt->setRange(ff->getDescription(fpID.toString()).minValue, ff->getDescription(fpID.toString()).maxValue);
            } else {
                edt->setCheckBounds(false, false);
            }

            return edt;
        }
        if ( fpID.isValid() && widgetType.toInt()==QFFitFunctionValueInputTable::wtErrorEdit) {
            QFDoubleEdit* edt=new QFDoubleEdit(parent);
            return edt;
        }
        if ( fpID.isValid() && widgetType.toInt()==QFFitFunctionValueInputTable::wtFixCheck) {
            QCheckBox* edt=new QCheckBox(parent);
            return edt;
        }

        if (ff && fpID.isValid() && widgetType.toInt()==QFFitFunctionValueInputTable::wtValueComboBox) {
            QComboBox* edt=new QComboBox(parent);
            connect(edt, SIGNAL(currentIndexChanged(int)), this, SLOT(onEditorFinished()));
            QFFitFunction::ParameterDescription desc=ff->getDescription(fpID.toString());
            int cnt=0;
            //qDebug()<<fpMin<<"..."<<fpMax<<ff;
            if (fpMin.isValid()&&fpMax.isValid()){
                bool okmi=false, okma=false;
                int mi=fpMin.toInt(&okmi);
                int ma=fpMax.toInt(&okma);
                if (okmi&&okma&&ma>=mi) {
                    for (int i=mi; i<=ma; i++) {
                        edt->addItem(desc.comboItems.value(cnt, QString::number(i)), i);
                        cnt++;
                    }

                }
            } else if (ff && ff->hasParameter(fpID.toString())){
                int mi=ff->getDescription(fpID.toString()).minValue;
                int ma=ff->getDescription(fpID.toString()).maxValue;
                for (int i=mi; i<=ma; i++) {
                    edt->addItem(desc.comboItems.value(cnt, QString::number(i)), i);
                    cnt++;
                }
            }
            return edt;
        }
        return NULL;
    }
    return NULL;
}

void QFFitFunctionValueInputDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox* cmb=qobject_cast<QComboBox*>(editor);
    QCheckBox* chk=qobject_cast<QCheckBox*>(editor);
    QFDoubleEdit* dbl=qobject_cast<QFDoubleEdit*>(editor);
    QVariant widgetType=index.data(QFFitFunctionValueInputTable::widgetTypeRole);
    QVariant d=index.data(Qt::EditRole);
    if (!d.isValid()) d=index.data(Qt::DisplayRole);

    if (cmb && widgetType.isValid() && widgetType.toInt()==QFFitFunctionValueInputTable::wtValueComboBox) {
        cmb->setCurrentIndex(cmb->findData(d.toInt()));
    } else if (cmb ) {
        cmb->setCurrentIndex(cmb->findText(d.toString()));
    } else if (chk) {
        chk->setChecked(d.toBool());
    } else if (dbl) {
        dbl->setValue(d.toDouble());
    } else {
        QFHTMLDelegate::setEditorData(editor, index);
    }
}

void QFFitFunctionValueInputDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* cmb=qobject_cast<QComboBox*>(editor);
    QCheckBox* chk=qobject_cast<QCheckBox*>(editor);
    QFDoubleEdit* dbl=qobject_cast<QFDoubleEdit*>(editor);
    QVariant widgetType=index.data(QFFitFunctionValueInputTable::widgetTypeRole);

    if (cmb && widgetType.isValid() && widgetType.toInt()==QFFitFunctionValueInputTable::wtValueComboBox) {
        model->setData(index, cmb->itemData(cmb->currentIndex()).toDouble());
    } else if (cmb ) {
        model->setData(index, cmb->currentText());
    } else if (chk) {
        model->setData(index, chk->isChecked());
    } else if (dbl) {
        model->setData(index, dbl->value());
    } else {
        QFHTMLDelegate::setModelData(editor, model, index);
    }
}

void QFFitFunctionValueInputDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    QPoint offset(0,0);
    editor->setGeometry(QRect(offset+option.rect.topLeft(), QSize(option.rect.width()-offset.x(), option.rect.height())));
}

void QFFitFunctionValueInputDelegate::onEditorFinished()
{
    QWidget * widEditor = static_cast<QWidget *>(sender());
    if (widEditor) {
        emit commitData(widEditor);
        emit closeEditor(widEditor);
    }
}
