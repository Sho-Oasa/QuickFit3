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

#ifndef QFRDRIMAGINGFCSTHREADPROGRESS_H
#define QFRDRIMAGINGFCSTHREADPROGRESS_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

namespace Ui {
    class QFRDRImagingFCSThreadProgress;
}

/*! \brief progress widget for the correlation progress
    \ingroup qf3rdrdp_imaging_fcs
*/
class QFRDRImagingFCSThreadProgress : public QWidget {
        Q_OBJECT

    public:
        explicit QFRDRImagingFCSThreadProgress(QWidget *parent = 0);
        ~QFRDRImagingFCSThreadProgress();
        bool wasCanceled() const;
        bool isDone() const;
        void resetWidget();
        int getRangeMax() const;
        int getRangeMin() const;
        int getProgress() const;
        int getStatus() const;
    public slots:
        void setName(QString name);
        /*! \brief set the status displayed

              - \a status == 0: waiting
              - \a status == 1: running
              - \a status == 2: done
              - \a status == -1: error
          */
        void setStatus(int status=0);
        void setMessage(QString message);
        void setProgress(int p);
        void incProgress(int pi);
        void setRange(int min, int max);
        void updateTime();
    signals:
        void cancelClicked();
        void closeClicked();
    protected slots:
        void on_btnCancel_clicked();
    private:
        Ui::QFRDRImagingFCSThreadProgress *ui;
        bool m_wasCanceled;
        bool m_isDone;
        int m_status;
        QElapsedTimer started;
        qint64 runMins;
        qint64 runSecs;
        QTimer* timer;
};

#endif // QFRDRIMAGINGFCSTHREADPROGRESS_H
