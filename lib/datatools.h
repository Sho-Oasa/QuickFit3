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

#ifndef DATATOOLS_H
#define DATATOOLS_H
#include "csvtools.h"
#include "matlabtools.h"


#include <QString>
#include <QList>
#include <QVector>
#include <QTextStream>
#include <QStringList>
#include "lib_imexport.h"
#include "qfrdrtableinterface.h"

/*! \brief exchange rows and columns
    \ingroup qf3lib_tools
*/
QFLIB_EXPORT int dataGetRows(const QList<QVector<double> >& data);
/*! \brief exchange rows and columns
    \ingroup qf3lib_tools
*/
QFLIB_EXPORT int dataGetRows(const QList<QList<double> >& data);

/*! \brief exchange rows and columns
    \ingroup qf3lib_tools
*/
QFLIB_EXPORT int dataGetRows(const QList<QList<QVariant> >& data);


/*! \brief exchange rows and columns
    \ingroup qf3lib_tools
*/
QFLIB_EXPORT QList<QVector<double> > dataRotate(const QList<QVector<double> >& data);
/*! \brief exchange rows and columns
    \ingroup qf3lib_tools
*/
QFLIB_EXPORT QList<QList<double> > dataRotate(const QList<QList<double> >& data);

/*! \brief exchange rows and columns
    \ingroup qf3lib_tools
*/
QFLIB_EXPORT QList<QList<QVariant> > dataRotate(const QList<QList<QVariant> >& data);

/*! \brief if the given table of values contains lists of values (or QSize, QRect, QPoint, QVector ...) these are expanded to additional columns, if columnnames is given, blank items will be inserted as needed
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT void dataExpand(QList<QList<QVariant> >& data, QStringList* columnsNames=NULL);


/*! \brief if the given table of values contains lists of values (or QSize, QRect, QPoint, QVector ...) these are reduced to a single value
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT void dataReduce(QList<QList<QVariant> >& data, QStringList* columnsNames=NULL);

/*! \brief reduce a variant table and convert to double
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QList<QVector<double> > dataReduceToDouble(const QList<QList<QVariant> >& data, QStringList* columnsNames=NULL);

/*! \brief expand a variant table and convert to double
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QList<QVector<double> > dataExpandToDouble(const QList<QList<QVariant> >& data, QStringList* columnsNames=NULL);

/*! \brief convert a QList<QVector<double> > to a new (with qfMalloc(), so free with qfFree() ) row-major data tyble of type double
    \ingroup qf3lib_tools

    \note Empty cells will contain \c NAN!
    \note data is given as a list of columns
*/
QFLIB_EXPORT double* doubleDataToDoubleMatrix(const QList<QVector<double> >& data);


/*! \brief returns \c true, if the given dataset may be converted to a double matrix without a loss (using dataToDouble() )
    \ingroup qf3lib_tools

    \note Empty cells will contain \c NAN!
    \note data is given as a list of columns
*/
QFLIB_EXPORT bool mayLossLesslyDoDoubleDataToDouble(const QList<QList<QVariant> >& data);

/*! \brief convert a variant table to double
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QList<QVector<double> > dataToDouble(const QList<QList<QVariant> >& data, QStringList* columnsNames=NULL);


/*! \brief convert a double table to a QVariant table
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QList<QList<QVariant> > dataToVariant(const QList<QList<double> >& data);

/*! \brief convert a double table to a QVariant table
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QList<QList<QVariant> > dataToVariant(const QList<QVector<double> >& data);


/*! \brief convert the given data to SYLK
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QString toSYLK(const QList<QVector<double> >& data, const QStringList& columnsNames=QStringList(), const QStringList& rowNames=QStringList());

/*! \brief convert the given data to SYLK
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QString toSYLK(const QList<QList<QVariant> >& data, const QStringList& columnsNames=QStringList(), const QStringList& rowNames=QStringList());


/*! \brief convert the given data to QFTableModel XML
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QString toQFTableModelXML(const QList<QVector<double> >& data, const QStringList& columnsNames=QStringList(), const QStringList& rowNames=QStringList());

/*! \brief convert the given data to QFTableModel XML
    \ingroup qf3lib_tools

    \note data is given as a list of columns
*/
QFLIB_EXPORT QString toQFTableModelXML(const QList<QList<QVariant> >& data, const QStringList& columnsNames=QStringList(), const QStringList& rowNames=QStringList());

/*! \brief This tool simplifies the creation of data tables for QFDataExportHandler
    \ingroup qf3lib_tools
*/
class QFLIB_EXPORT QFDataExportTool {
    public:
        QList<QList<QVariant> > data;
        QStringList colHeaders;
        QStringList rowHeaders;
        QString comment;
        QMap<QString, QVariant> properties;

        void save(const QString& filename, int format) const;
        void saveCSV(const QString& filename, QChar decimalSep='.', const QString colSep=QString(", "), bool withHeaders=true, QChar stringDelimiter=QLatin1Char('\"'), const QString& headerSep=QString("#! "), int precision=15, int fieldWidth=-1) const;
        void saveToTable(QFRDRTableInterface* table, bool setProps=false) const;

        void clear();

        void set(int col, int row, const QVariant& value);
        void setLastColumn(int row, const QVariant& value);
        void setRowTitle(int row, const QString& title);
        void setColTitle(int col, const QString& title);
        int getRowCount() const;
        int getColCount() const;
        QVariant get(int col, int row) const;





};


/*! \brief this class groups static methods to export to different data formats
    \ingroup qf3lib_tools
*/
class QFLIB_EXPORT QFDataExportHandler {
    public:
        /** \brief you may register an object derived from this class to add new export formats.
         *
         * \note You have to implement at least one of the two save() methods. otherwise, a call to any will lead to an endles loop.
         **/
        class QFLIB_EXPORT DataWriter {
            public:
                virtual ~DataWriter() {}
                virtual QString getFilter() const=0;
                virtual void save(const QList<QVector<double> > &data, const QString& filename, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
                virtual void save(const QList<QList<QVariant> > &data, const QString& filename, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());

        };
        /** \brief used to register a new data format */
        static void registerDataWriter(DataWriter* writer);
        /** \brief used to un-register a data format */
        static void deregisterDataWriter(DataWriter* writer);

        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void save(const QList<QVector<double> > &data, int format, const QString& filename, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void save(const QList<QVector<double> > &data,const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void copyCSV(const QList<QList<double> >& data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void copyCSV(const QList<QVector<double> >& data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void copyCSV(const QList<QList<QVariant> >& data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void copyExcel(const QList<QList<double> >& data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void copyExcel(const QList<QVector<double> >& data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void copyExcel(const QList<QList<QVariant> >& data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void save(const QList<QList<QVariant> > &data, int format, const QString& filename, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void save(const QList<QList<QVariant> > &data, int format, const QString& filename, const QStringList& columnHeaders, const QStringList& rowHeaders, const QString& comment, const QMap<QString, QVariant> properties);
        /*! \brief save the given \a data in the given \A format (index into the list, as returned by dataGetFormats() )
         */
        static void save(const QList<QList<QVariant> > &data, const QStringList& columnHeaders=QStringList(), const QStringList& rowHeaders=QStringList());
        /*! \brief save the given \a data as a Matlab script
         */
        static void copyMatlab(const QList<QList<QVariant> >& data);
        /*! \brief save the given \a data as a Matlab script
         */
        static void copyMatlab(const QList<QList<double> >& data);
        /*! \brief save the given \a data as a Matlab script
         */
        static void copyMatlab(const QList<QVector<double> >& data);
        /*! \brief returns a list of filename filters for the formats to which data may be saves
        */
        static QStringList getFormats();
    protected:
        static QList<DataWriter*> writers;
};

#endif // DATATOOLS_H

