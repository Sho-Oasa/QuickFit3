#ifndef DLGFCCSRESETSETPARAMETER_H
#define DLGFCCSRESETSETPARAMETER_H

#include "qfdialog.h"

namespace Ui {
    class DlgFCCSResetSetParameter;
}

class DlgFCCSResetSetParameter : public QFDialog
{
        Q_OBJECT

    public:
        struct Parameter {
            QString id;
            QString label;
            double value;
            double error;
            bool fix;
            double rmin;
            double rmax;
        };

        explicit DlgFCCSResetSetParameter(QWidget *parent = 0);
        ~DlgFCCSResetSetParameter();

        void addParameter(const Parameter& p);
        void setParameters(const QList<Parameter>& p);
        void clearParams();
        bool setInAll() const;
        bool setInInitial() const;
        enum Operation {
            opSet,
            opReset,
            opNothing
        };
        Operation operationValue() const;
        Operation operationError() const;
        Operation operationFix() const;
        double getValue() const;
        double getError() const;
        bool getFix() const;
        bool setAllIndices();
        QString getParameter() const;
        void setHelpfile(const QString help);

    protected slots:
        void updateWidgets();
        void on_cmbParam_currentIndexChanged();
        void on_btnHelp_clicked();
        void dataEdited();
    private:
        Ui::DlgFCCSResetSetParameter *ui;
        QList<Parameter> params;
        QString help;

};

#endif // DLGFCCSRESETSETPARAMETER_H
