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

#ifndef ALV6000TOOLS_H
#define ALV6000TOOLS_H

#include <QDate>
#include <QTime>
#include <QString>
#include <QFileInfo>

#include <ostream>
#include <exception>
#include <cstdio>
#include <QVector>
#include <iostream>

#include "cpptools.h"

/*
  Simple mdaptation from alv5000tools.h
*/
/*!
    \defgroup quickfit2ALV ALV-6000 Data File (ACSII) Import
    \ingroup qf3rdrdp_fcs

    This group contains methods, structs and enums that are used to import ALV-6000 data files.

    \section Quickfit2_ALVFileFormat File Format of ALV-6000 data files (ASCII)

    The header contains several lines with additional data fields. Each line looks like this:
    \verbatim  <name> : <data> \endverbatim
    Here the \c <name> is a string which specifies what the parameter is. The \c <data> may either be a number (dot '.' as
    decimal separator) or a string which will be enclosed in quotation marks '"'.
    The first line of the header identifies the file format and ALV software version (standard <tt>ALV-6000/E-WIN Data</tt>).

    After the header the file contains the correlation data and the intensity of the last run (for multiple runs). The correlation
    data section is started by a line which contains <tt>"Correlation ..."</tt>. After the word Correlation there may be additional
    words specifying what is saved here. Note the quotation marks around this line. The next line contains the column titles,
    each enclosed in quotation marks. In a standard file (two channel auto correlation, two runs) it reads:
    \verbatim "Lag [ms]"	"Average(0)"	"Run  1"	"Run  2"	"Average(1)"	" 1,   0.000"	" 2,   0.000"\endverbatim
    So as you can see the data is separated by whitespaces. The first column is the lag time in milliseconds. The second column
    is the average over all correlation functions (multiple runs!) for the first channel (CH0). The next two columns contain the
    correlation functions themselves. Then the same data is provided for the second channel (CH1). Note that there is a software bug
    and the titles of the next runs is wrong.

    After the correlation data the file also contains the count rates of the last run. This section is started by a line, containing
    <tt>"Count Rate"</tt>. The count rates are stored in multiple columns. The first column contains the time index in seconds,
    and the next columns contain the countrates for CH0 (column 2) and CH1 (column 3).
 */


/** \brief allowed tokens for the ALV file parser
 *  \ingroup quickfit2ALV
 */
enum ALV6_TOKENTYPE {
    ALV6_EOF,
    ALV6_LINEBREAK,
    ALV6_COLON,
    ALV6_NAME,
    ALV6_QUOTED,
    ALV6_VALUE
};

/** \brief represents one token (incl. type and value) for the ALV file parser
 *  \ingroup quickfit2ALV
 */
typedef struct {
    ALV6_TOKENTYPE type;
    QString value;
    double doubleValue;
} ALV6_TOKEN;

/** \brief an exception class used to report errors while parsing ALV files
 *  \ingroup quickfit2ALV
 */
class alv6_exception : public std::exception {
    private:
        std::string error;
    public:
        alv6_exception(std::string error_desc) throw() { error=error_desc; std::cout<<error.c_str()<<std::endl; };
        virtual ~alv6_exception() throw() {};
        virtual const char* what() const throw() { return error.c_str(); };
};


/** \brief reads the next token from an ALV datafile
 *  \ingroup quickfit2ALV
 *
 * This method implements a simple tokenizer for ALV files
 *    \param alv_file      The file object to read from
 *    \param readingHeader Do we currently read from the file header? If yes names may contain whitespaces (but no lineabreaks)!
 */
ALV6_TOKEN ALV6_getToken(FILE* alv_file, bool readingHeader);

/** \brief reads until the next \a quotationMark and returns the read string
 *  \ingroup quickfit2ALV
 *
 * this method will swallow all white spaces bevore the quoted string. Then it expects a \a quotationMark.
 * Now it reads until the next quotationMark, or the end of the line.
 */
QString ALV6_ReadQuoted(FILE* alv_file, char quotationMark='"');

/** \brief converts a date in the format "<tt>22-Aug-08</tt>" (ALV date format) into a QDate object
 *  \ingroup quickfit2ALV
 */
QDate ALV6_toDate(QString d);

/** \brief converts a time in the format "<tt>15:09:10</tt>" (ALV date format) into a QDate object
 *  \ingroup quickfit2ALV
 */
QTime ALV6_toTime(QString d);

/** \brief read a matrix of numbers, used by ALV_readCorrelation() and ALV_readRate(). The result
 *         is written into \a datm.
 *  \ingroup quickfit2ALV
 *
 * This function returns the last read token which is the first token which does not belong to the
 * correlation data. You can use this return value in a token evaluation loop!
 */
ALV6_TOKEN ALV6_readNumberMatrix(FILE* alv_file, QVector<QVector<double> >* datm);

/** \brief read some parameters from the given ALV6000 file
 *  \ingroup quickfit2ALV
 *
 * \param filename filename of the ALV6000 file to be loaded
 * \param mode the mode string stored in the ALV6000 file
 * \param channelCount number of channels in the file
 * \param runCount number of runs in the file
 * \param crossCorrelation \c true if the file contains a cross correlation curve
 */
void ALV6_analyze(QString filename, QString& mode, unsigned int& channelCount, unsigned int& runCount, bool& crossCorrelation);




#endif // ALV6000TOOLS_H
