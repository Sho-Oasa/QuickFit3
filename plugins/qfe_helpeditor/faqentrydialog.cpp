/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>),
    German Cancer Research Center/University Heidelberg



    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "faqentrydialog.h"
#include "ui_faqentrydialog.h"
#include "programoptions.h"
#include "qftools.h"

FAQEntryDialog::FAQEntryDialog(QWidget *parent) :
    QFDialog(parent),
    ui(new Ui::FAQEntryDialog)
{
    ui->setupUi(this);
    loadWidgetGeometry(ProgramOptions::getInstance(), this, "FAQEntryDialog");

    ProgramOptions::getConfigQPlainTextEdit(ui->edtAnswer, "FAQEntryDialog/edtAnswer");
    ProgramOptions::getConfigQPlainTextEdit(ui->edtQuestion, "FAQEntryDialog/edtQuestion");

}

FAQEntryDialog::~FAQEntryDialog()
{
    saveWidgetGeometry(ProgramOptions::getInstance(), this, "FAQEntryDialog");
    ProgramOptions::setConfigQPlainTextEdit(ui->edtAnswer, "FAQEntryDialog/edtAnswer");
    ProgramOptions::setConfigQPlainTextEdit(ui->edtQuestion, "FAQEntryDialog/edtQuestion");
    delete ui;
}

QString FAQEntryDialog::insertText() const
{
    QString ID=QString("<a name=\"%1\">").arg(ui->edtID->text());
    if (ui->edtID->text().isEmpty()) ID="";
    return QString("$$faq_start$$\n  %1<b>%2</b>\n$$faq_answer$$\n  %3\n$$faq_end$$").arg(ID).arg(ui->edtQuestion->toPlainText()).arg(ui->edtAnswer->toPlainText());
}
