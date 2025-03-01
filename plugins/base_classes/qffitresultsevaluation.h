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

#ifndef QFFITRESULTSEVALUATION_H
#define QFFITRESULTSEVALUATION_H

#include <QHash>
#include <QMap>
#include <QSettings>

#include "qfevaluationitem.h"
#include "qfevaluationitemfactory.h"
#include "qffitfunction.h"
#include "qffitalgorithm.h"
#include "qffitparameterbasicinterface.h"
#include "qffitalgorithmthreaddedfit.h"
#include "qffitalgorithmparameterstorage.h"

/*! \brief evaluation item base class for data fits using QFFitAlgorithm and QFFitFunction
    \ingroup qf3evaluationplugins

    Features of this class are:
      - it allows to easily implement evaluation items which store fit results to the raw data records.
      - The fit algorithm and model/function may be selected globally.
      - parameter value ranges and fix-indicators are set globally for all records
      - fit parameter values and errors are stored locally in the raw data record after the first fit (hasFit() returns \c true ).
        Before the first fit they are stored globally. So the user first sees the global parameters which are valid as starting
        point for every fit. Then he  runs a fit and ends up with the local parameters. This way it is always possible to
        go back to the initial/global values to reset the fit and set the initial values for a whole bunch of files at once.
      - The \c getFit...() and \c setFit...() functions already care for the structure described in the last topic. They are defined in different flavors:
          -# The most basic versions (which also really contain the implementation) have a QFRawDataRecord, the \c resultID and a \a parameterID
             as parameters. This triplet allows to access a value from the result store in a QFRawDataRecord and also the internal
             datastore, if no fit has been carried out yet. The rusultID is created by functions called getEvaluationResultID() with
             different sets of parameters. The resultID (a short string) should be built to include all important settings that
             distinguish different parameter sets, i.e. the ID of the fit function and maybe something like the
             selected run. Here is an example:
             \code QString("%1_%2_%3_r%4").arg(getType()).arg(getID()).arg(fitFunctionID).arg(currentRun)\endcode
             Usually you won't use
          -# In addition there are also functions to access the values of the currently selected/highlighted record. These rely on
             an implementtaion of getEvaluationResultID() with no parameters, which means to take the currently selected
             fit function, fit algorithm, ...
          -# The resultID may be transformed before usage, sugin transformResultID() In the default implementation this is does not do
             any transformation.
        .
      - the global fit parameters are stored internally in the protected member parameterStore. They are also saved to
        the project file (see intWriteData() and intReadData() ). If you want to save additional data either implement these
        functions in your class and call the functions from this class explicitly (using e.g. <code>QFFitResultsEvaluation::intWriteData()</code>),
        or implement the functions intReadDataFitParam() and intWriteDataFitParam() which allow to save additional data for every
        fit parameter.
      - the initial values for fit parameters are read from the initial values given in the QFFitFunction. As a second option this
      class opens the file \code <configDir>/fitparams_<type>.ini\endcode and reads the parameters from it in the form \code
[fitFunctionID]
param=<initial value>
param_min=<initial_range_min>
param_max=<initial_range_max>
param_fix=<initial_fix>
      \endcode
      - implementing rdrPointerToParameterStoreID() can e used to make the internal default value store specific to a QFRawDataRecord*.
        The returned value (default: empty string) will be used to build the parameterStoreID. E.g. if you have several RDRs open at the
        same time in a parallel fit, this function might return the number in the editor of the given RDR and every RDR may have it's own
        default parameters
    .

*/
class QFFitResultsEvaluation : public QFEvaluationItem, public QFFitParameterBasicInterface, public QFFitAlgorithmParameterStorage {
    Q_OBJECT
    Q_INTERFACES(QFFitParameterBasicInterface)
public:
    /*! \brief class conrtuctor

        \param fitFunctionPrefix only fit functions with this prefix will be used by this object, leave empty to use all, you may supply a list, separated by commas
        \param parent project this object belongs to
     */
    QFFitResultsEvaluation(const QString& fitFunctionPrefix, QFProject* parent, bool showRDRList=true, bool useSelection=false);
    virtual ~QFFitResultsEvaluation();


    /** \copydoc QFEvaluationItem::setPresetProperty() */
    virtual void setPresetProperty(const QString& id, const QVariant& data, bool usereditable=true, bool visible=true);


    /** \brief returns a list of all FCS fitting functions available for this evaluation
     *  \note This class uses only those fit functions where the ID starts with \c "fcs_"
     */
    virtual QStringList getAvailableFitFunctions() const;
    /** \brief returns the given fit function instance (DO NEVER DELETE THIS INSTANCE!) */
    virtual QFFitFunction* getFitFunction(const QString& parameterID) const;

    /** \brief set the current fitting algorithm by ID */
    virtual void setFitAlgorithm(QString fitAlgorithm);

    /** \brief get the current fitting algorithm */
    virtual QFFitAlgorithm* getFitAlgorithm() const;


    /** \brief returns a list of all available fit algorithms */
    virtual QStringList getAvailableFitAlgorithms() const;

    /** \brief return the instance of a given fit algorithm for this evaluation (DO NEVER DELETE THIS INSTANCE!)  */
    virtual QFFitAlgorithm* getFitAlgorithm(const QString& parameterID) const;
    /** \brief return a new instance of a given fit algorithm for this evaluation   */
    virtual QFFitAlgorithm* createFitAlgorithm(const QString& id) const;

    /*! \brief set the current fitting algorithm

        This function also tries to transform the parameters between different models. The transformation is based on the parameter
        name. If the new model has some parameters with the same name as the old model, the new model parameters are set to the value
        of the old parameter.
    */
    virtual void setFitFunction(QString fitFunction);

    /** \brief get the current fitting function for the selected RDR */
    virtual QFFitFunction* getFitFunction() const;

    /** \brief get the current fitting function for the selected RDR */
    virtual QString getFitFunctionID() const;

    /** \brief get the current fitting function */
    virtual QFFitFunction* getFitFunction(const QFRawDataRecord *rdr) const;

    /** \brief get the current fitting function ID */
    virtual QString getFitFunctionID(const  QFRawDataRecord* rdr) const;

    virtual QString getFitFunctionID(int num) const;

    /** \brief create a new instance of the current fit algorithm ... the user has to destroy the instance */
    virtual QFFitAlgorithm* createFitAlgorithm() const;

    /** \brief create a new instance of the current fit function ... the user has to destroy the instance */
    virtual QFFitFunction* createFitFunction(const QFRawDataRecord *rdr )const;

    /** \brief create a new instance of the current fit function ... the user has to destroy the instance */
    virtual QFFitFunction* createFitFunction()const;

    /** \brief create a new instance of the current fit function ... the user has to destroy the instance */
    virtual QFFitFunction* createFitFunction(int rdr )const;


    /** \brief may be used to transform a resultID before it is used in any of the \c QFRawDataRecord.results... functions */
    virtual QString transformResultID(const QString& resultID) const;



    /** \brief sets the group of the given fit result, this function transforms the parameter ID with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param group group to be stored
     */
    virtual void setFitResultGroup(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, const QString& group) ;
    /** \brief sets the group of the given fit result, this function does not transform the parameter ID with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param group group to be stored
     */
    virtual void setFitResultGroupNoParamTransform(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, const QString& group) ;

    /** \brief sets the label of the given fit result
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param label label to be stored
     *  \param label_richtext richtext-ed label to be stored
     */
    virtual void setFitResultLabel(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, const QString& label, const QString& label_richtext=QString("")) ;

    /** \brief sets the label of the given fit result, this function does not transform the parameter name with getFitParameterID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param label label to be stored
     *  \param label_richtext richtext-ed label to be stored
     */
    virtual void setFitResultLabelNoParamTransform(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, const QString& label, const QString& label_richtext=QString("")) ;

    /** \brief sets the group of the given evaluation result ID \a resultID
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param group group to be stored
     */
    virtual void setFitResultEvaluationGroup(QFRawDataRecord* r, const QString& resultID, const QString& group) ;

    /** \brief sets the description of the given evaluation result ID \a resultID
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param description description to be stored
     */
    virtual void setFitResultEvaluationDescription(QFRawDataRecord* r, const QString& resultID, const QString& description) ;

    /** \brief sets the group of the current fit result
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param group group to be stored
     */
    virtual void setFitResultGroup(const QString& parameterID, const QString& group) ;

    /** \brief sets the label of the current fit result
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param label label to be stored
     *  \param label_richtext richtext-ed label to be stored
     */
    virtual void setFitResultLabel(const QString& parameterID, const QString& label, const QString& label_richtext=QString("")) ;

    /** \brief sets the group of the current evaluation result ID \a resultID
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param group group to be stored
     */
    virtual void setFitResultEvaluationGroup(const QString& group) ;

    /** \brief sets the description of the current evaluation result ID \a resultID
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param description description to be stored
     */
    virtual void setFitResultEvaluationDescription(const QString& description) ;






    /*! \brief set a fit parameter of the current fit function (see m_fitFunction) to the specified value

        The value may be stored at different positions:
          - If no item is highlighted the parameter is not stored at all
          - If a current raw data record is highlighted and no fit has been carried out so far for this record, the value
            is stored in the preset value store of this QFEvaluationItem
          - If a current raw data record is highlighted and a fit has already been carried out for this record, the value
            is stored as a result in the highlighted QFRawDataRecord
        .
        This function also checks whether the value is to be stored (i.e. it's userEditable property is set)

        \param r the record to adress
        \param resultID the result ID which to access in the raw data records result store
        \param parameterID set the value of the parameter with this id (see QFFitFunction)
        \param value value to be stored

     */
    virtual void setFitValue(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value);

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValue(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit associated with \a value
     */
    virtual void setFitResultValue(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value, QString unit)  ;




    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueNoParamTransform(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit associated with \a value
     */
    virtual void setFitResultValueNoParamTransform(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value, QString unit)  ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueString(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, QString value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueInt(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, int64_t value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit associated with \a value
     */
    virtual void setFitResultValueInt(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, int64_t value, QString unit) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueBool(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, bool value) ;

    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param error error associated with \a value
     */
    virtual void setFitResultValue(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value, double error)  ;
    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param error error associated with \a value
     *  \param unit unit of the fit result
     */
    virtual void setFitResultValue(QFRawDataRecord *r, const QString &resultID, const QString &parameterID, QVector<double> value, QVector<double> error, QString unit=QString(""))  ;
    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit of the fit result
     */
    virtual void setFitResultValue(QFRawDataRecord *r, const QString &resultID, const QString &parameterID, QVector<double> value, QString unit=QString(""))  ;
    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function does not transform the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param error error associated with \a value
     */
    virtual void setFitResultValueNoParamTransform(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double value, double error)  ;
    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function does not transform the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param error error associated with \a value
     *  \param unit unit of the fit result
     */
    virtual void setFitResultValueNoParamTransform(QFRawDataRecord *r, const QString &resultID, const QString &parameterID, QVector<double> value, QVector<double> error, QString unit=QString(""))  ;
    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function does not transform the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit of the fit result
     */
    virtual void setFitResultValueNoParamTransform(QFRawDataRecord *r, const QString &resultID, const QString &parameterID, QVector<double> value, QString unit=QString(""))  ;


    /** \brief stores the given error as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param error error to be stored
     */
    virtual void setFitResultError(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double error)  ;

    /** \brief stores the given values and errors as a fit result, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param values values to be stored
     *  \param errors errors to be stored
     */
    virtual void setFitResultValues(QFRawDataRecord* r, const QString& resultID, double* values, double* errors) ;

    /** \brief stores the given values and errors as a fit result if they are visible, i.e. into the currently highlighted QFRawDataRecord, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param values values to be stored
     *  \param errors errors to be stored
     */
    virtual void setFitResultValuesVisible(QFRawDataRecord* r, const QString& resultID, double* values, double* errors) ;

    /** \brief stores the given values and errors as a fit result if they are visible, i.e. into the currently highlighted QFRawDataRecord.
     *          Also imports the fit parameter description from the QFFitFunction and sets the parameter group to the given value, this function transforms the fit parameter name with getFitParamID()
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param values values to be stored
     *  \param errors errors to be stored
     *  \param group the fit parameter group to assign
     *  \param fix indicates whether a parameter was fixed
     *  \param fixGroup the group the fix results should be added to
     *  \param sortPriority should the results be assigned sort priority?
     */
    virtual void setFitResultValuesVisibleWithGroupAndLabel(QFRawDataRecord* r, const QString& resultID, const double* values, const double* errors, const QString& group, const bool* fix=NULL, const QString& fixGroup=QString(""), bool sortPriority=true) ;
















    /*! \brief return the value of a given parameter
        \param r the record to adress
        \param resultID the result ID which to access in the raw data records result store
        \param parameterID the parameter id
        \return the value of the given parameter \a id

        This function looks for the value at several positions:
          -# if a fit has already been performed for the current record, the value is read from raw data record
          -# if no fit has been performed, the value is taken from this evaluation (i.e. the preset value)
          -# if also locally no value is available, the value is looked up in the application INI
          -# if this also fails, the value is taken from the initial value stored in the fitFunction
        .
    */
    virtual double getFitValue(const QFRawDataRecord* r, const QString& resultID, const QString& parameterID) const;
    /*! \brief return the fit error of a given parameter
        \param r the record to adress
        \param resultID the result ID which to access in the raw data records result store
        \param parameterID the parameter id
        \return the error associated with the given parameter.
    */
    virtual double getFitError(const QFRawDataRecord* r, const QString& resultID, const QString& parameterID) const;


    /*! \brief return the fix property of a given parameter

        \param r the record to adress
        \param resultID the result ID which to access in the raw data records result store
        \param parameterID the parameter id
        For a detailed description of where the value is searched, see getFitValue()
    */
    virtual bool getFitFix(const QFRawDataRecord* r, const QString& resultID, const QString& parameterID)const ;


    /*! \brief return the value of a given parameter
        \param id the parameter id
        \return the value of the given parameter \a id

        This function looks for the value at several positions:
          -# if a fit has already been performed for the current record, the value is read from raw data record
          -# if no fit has been performed, the value is taken from this evaluation (i.e. the preset value)
          -# if also locally no value is available, the value is looked up in the application INI
          -# if this also fails, the value is taken from the initial value stored in the fitFunction
        .
    */
    virtual double getFitValue(const QString& id, const QFRawDataRecord *r=NULL) const ;

    /*! \brief return the fit error of a given parameter

        \param id the parameter id
        \return the error associated with the given parameter.
    */
    virtual double getFitError(const QString& id, const QFRawDataRecord* r=NULL) const ;


    /*! \brief return the fix property of a given parameter

        \param id the parameter id
        For a detailed description of where the value is searched, see getFitValue()
    */
    virtual bool getFitFix(const QString& id, const QFRawDataRecord* r=NULL) const ;
    /*! \brief return the lower value bound of a given parameter

        \param id set the range min of the parameter with this id (see QFFitFunction)
        For a detailed description of where the value is searched, see getFitValue()
    */
    virtual double getFitMin(const QString& id, const QFRawDataRecord* r=NULL) const ;


    /*! \brief return the upper value bound of a given parameter

        \param id set the range max of the parameter with this id (see QFFitFunction)
        For a detailed description of where the value is searched, see getFitValue()
    */
    virtual double getFitMax(const QString& id, const QFRawDataRecord* r=NULL)  const ;























    /*! \brief set the error of a given parameter

        \param r the record to adress
        \param resultID the result ID which to access in the raw data records result store
        \param parameterID set the value of the parameter with this id (see QFFitFunction)
        \param error error to be set
    */
    virtual void setFitError(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, double error);

    /*! \brief set the fix property of a fit parameter of the current fit function (see m_fitFunction)

        For a description of when data is stored, see setFitValue()

        \param r the record to adress
        \param resultID the result ID which to access in the raw data records result store
        \param parameterID set the value of the parameter with this id (see QFFitFunction)
        \param fix fix to be stored

     */
    virtual void setFitFix(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, bool fix);

    /** \brief stores the given fix property as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param r the record to adress
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param parameterID set the value of the parameter with this id (see QFFitFunction)
     *  \param fix fix to be stored
     */
    virtual void setFitResultFix(QFRawDataRecord* r, const QString& resultID, const QString& parameterID, bool fix) ;


    /*! \brief set a fit parameter of the current fit function (see m_fitFunction) to the specified value

        The value may be stored at different positions:
          - If no item is highlighted the parameter is not stored at all
          - If a current raw data record is highlighted and no fit has been carried out so far for this record, the value
            is stored in the preset value store of this QFEvaluationItem
          - If a current raw data record is highlighted and a fit has already been carried out for this record, the value
            is stored as a result in the highlighted QFRawDataRecord
        .
        This function also checks whether the value is to be stored (i.e. it's userEditable property is set)

        \param id set the value of the parameter with this id (see QFFitFunction)
        \param value value to be stored

     */
    virtual void setFitValue(const QString& id, double value, QFRawDataRecord* r=NULL);

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValue(const QString& id, double value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit associated with \a value
     */
    virtual void setFitResultValue(const QString& id, double value, QString unit)  ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueString(const QString& id, QString value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueInt(const QString& id, int64_t value) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param unit unit associated with \a value
     */
    virtual void setFitResultValueInt(const QString& id, int64_t value, QString unit) ;

    /** \brief stores the given value as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     */
    virtual void setFitResultValueBool(const QString& id, bool value) ;

    /** \brief stores the given value and error as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param value value to be stored
     *  \param error error associated with \a value
     */
    virtual void setFitResultValue(const QString& id, double value, double error) ;

    /** \brief stores the given error as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param error error to be stored
     */
    virtual void setFitResultError(const QString& id, double error) ;

    /** \brief stores the given values and errors as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param values values to be stored
     *  \param errors errors to be stored
     */
    virtual void setFitResultValues(double* values, double* errors)  ;

    /** \brief stores the given values and errors as a fit result if they are visible, i.e. into the currently highlighted QFRawDataRecord
     *  \param values values to be stored
     *  \param errors errors to be stored
     */
    virtual void setFitResultValuesVisible(double* values, double* errors) ;


    /*! \brief set the error of a given parameter

        \param id set the value of the parameter with this id (see QFFitFunction)
        \param error error to be set
    */
    virtual void setFitError(const QString& id, double error, QFRawDataRecord* r=NULL);

    /*! \brief set the fix property of a fit parameter of the current fit function (see m_fitFunction)

        For a description of when data is stored, see setFitValue()

        \param id set the value of the parameter with this id (see QFFitFunction)
        \param fix fix to be stored

     */
    virtual void setFitFix(const QString& id, bool fix, QFRawDataRecord* r=NULL);

    /** \brief stores the given fix property as a fit result, i.e. into the currently highlighted QFRawDataRecord
     *  \param id set the value of the parameter with this id (see QFFitFunction)
     *  \param fix fix to be stored
     */
    virtual void setFitResultFix(const QString& id, bool fix) ;


    /*! \brief set the value range of a fit parameter of the current fit function (see m_fitFunction)

        This range is stored locally in the evaluation, never in the raw data record

        \param id set the value of the parameter with this id (see QFFitFunction)
        \param min lower bound for the value
        \param max upper bound for the value

     */
    virtual void setFitRange(const QString& id, double min, double max, QFRawDataRecord* r=NULL);

    /*! \brief set the value range of a fit parameter of the current fit function (see m_fitFunction)

        This range is stored locally in the evaluation, never in the raw data record

        \param id set the value of the parameter with this id (see QFFitFunction)
        \param min lower bound for the value

     */
    virtual void setFitMin(const QString& id, double min, QFRawDataRecord *r=NULL);

    /*! \brief set the value range of a fit parameter of the current fit function (see m_fitFunction)

        This range is stored locally in the evaluation, never in the raw data record

        \param id set the value of the parameter with this id (see QFFitFunction)
        \param max upper bound for the value

     */
    virtual void setFitMax(const QString& id, double max, QFRawDataRecord* r=NULL);




    /** \brief determine whether a fit has been carried out for the given record and resultID
     *
     *  \param r the given record, if \c NULL the currently highlighted record is checked
     *  \param evalID the evaluation ID which to access in the raw data records result store
     *  \return \c true if fit results exist for the current evaluation and fit function in the highlighted QFRawDataRecord result store.
     *          If no record is highlighted or any other circumstances prevent the determination of a proper result, the result will be \c false!
     */
    virtual bool hasFit(const QFRawDataRecord* r, const QString& evalID) const;

    /** \brief determine whether a fit has been carried out for the given record and the currently selected resultID
     *
     *  \param resultID the result ID which to access in the raw data records result store
     *  \return \c true if fit results exist for the current evaluation and fit function in the highlighted QFRawDataRecord result store.
     *          If no record is highlighted or any other circumstances prevent the determination of a proper result, the result will be \c false!
     */
    virtual bool hasFit(const QFRawDataRecord* r) const;

    /** \brief determine whether a fit has been carried out for the currently highlighted record and the currently (implicitly) selected resultID
     *
     *  \return \c true if fit results exist for the current evaluation and fit function in the highlighted QFRawDataRecord result store.
     *          If no record is highlighted or any other circumstances prevent the determination of a proper result, the result will be \c false!
     */
    virtual  bool hasFit() const ;




    /** \brief fill the given array of doubles with the current parameter values, as appropriate to use together with QFFitFunction
     *  \param param parameter vector to fill
     */
    virtual void fillParameters(double* param, QFFitFunction* function=NULL) const;
    /** \brief fill the given array of doubles with the current parameter errors, as appropriate to use together with QFFitFunction
     *  \param param parameter error vector to fill
    */
    virtual void fillParameterErrors(double* param, QFFitFunction* function=NULL) const ;
    /** \brief fill the given array of doubles with the current parameter lower bounds, as appropriate to use together with QFFitFunction
     *  \param param parameter minimums vector to fill
    */
    virtual void fillParametersMin(double* param, const QFRawDataRecord* r, QFFitFunction* function=NULL) const;
    /** \brief fill the given array of doubles with the current parameter upper bounds, as appropriate to use together with QFFitFunction
     *  \param param parameter maximums vector to fill
    */
    virtual void fillParametersMax(double* param, const QFRawDataRecord *r , QFFitFunction *function=NULL)const;
    /** \brief fill the given array of bools with the current parameter fix values, as appropriate to use together with QFFitFunction
     *  \param param parameter fix vector to fill
    */
    virtual void fillFix(bool* param, QFFitFunction* function=NULL) const ;

    /*! \brief fill a newly allocated array of doubles with the current parameter values, as appropriate to use together with QFFitFunction
        \return parameter vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual double* allocFillParameters(QFFitFunction* function=NULL) const;
    /*! \brief fill a newly allocated array of doubles with the current parameter errors, as appropriate to use together with QFFitFunction
        \return parameter error vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual double* allocFillParameterErrors(QFFitFunction* function=NULL) const ;
    /*! \brief fill a newly allocated array of doubles with the current parameter lower bounds, as appropriate to use together with QFFitFunction
        \return parameter minimums vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual double* allocFillParametersMin(const QFRawDataRecord* r, QFFitFunction* function=NULL) const;
    /*! \brief fill a newly allocated array of doubles with the current parameter upper bounds, as appropriate to use together with QFFitFunction
        \return parameter maximums vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual double* allocFillParametersMax(const QFRawDataRecord *r , QFFitFunction *function=NULL)const ;
    /*! \brief fill a newly allocated array of bools with the current parameter fix values, as appropriate to use together with QFFitFunction
        \return parameter fix vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual bool* allocFillFix(QFFitFunction* function=NULL) const;










    /*! \brief fill a newly allocated array of doubles with the current parameter values, as appropriate to use together with QFFitFunction
        \return parameter vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<double> allocVecFillParameters(QFFitFunction* function=NULL) const;
    /*! \brief fill a newly allocated array of doubles with the current parameter errors, as appropriate to use together with QFFitFunction
        \return parameter error vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<double> allocVecFillParameterErrors(QFFitFunction* function=NULL) const ;
    /*! \brief fill a newly allocated array of doubles with the current parameter lower bounds, as appropriate to use together with QFFitFunction
        \return parameter minimums vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<double> allocVecFillParametersMin(const QFRawDataRecord* r, QFFitFunction* function=NULL) const;
    /*! \brief fill a newly allocated array of doubles with the current parameter upper bounds, as appropriate to use together with QFFitFunction
        \return parameter maximums vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<double> allocVecFillParametersMax(const QFRawDataRecord *r , QFFitFunction *function=NULL)const ;
    /*! \brief fill a newly allocated array of bools with the current parameter fix values, as appropriate to use together with QFFitFunction
        \return parameter fix vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<bool> allocVecFillFix(QFFitFunction* function=NULL) const;
    /*! \brief fill a newly allocated array of doubles with the current parameter values, as appropriate to use together with QFFitFunction
        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \return parameter vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<double> allocVecFillParameters(const QFRawDataRecord* r,  const QString& resultID, QFFitFunction* function=NULL) const ;
    /*! \brief fill a newly allocated array of doubles with the current parameter errors, as appropriate to use together with QFFitFunction
        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \return parameter error vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<double> allocVecFillParameterErrors(const QFRawDataRecord* r,  const QString& resultID, QFFitFunction* function=NULL) const ;
    /*! \brief fill a newly allocated array of bools with the current parameter fix values, as appropriate to use together with QFFitFunction
        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \return parameter fix vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual QVector<bool> allocVecFillFix(const QFRawDataRecord* r,  const QString& resultID, QFFitFunction* function=NULL) const ;








    /** \brief fill the given array of doubles with the current parameter values, as appropriate to use together with QFFitFunction
     *  \param r record this appplies to
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param param parameter vector to fill
     */
    virtual void fillParameters(const QFRawDataRecord* r,  const QString& resultID, double* param, QFFitFunction* function=NULL) const;
    /** \brief fill the given array of doubles with the current parameter errors, as appropriate to use together with QFFitFunction
     *  \param r record this appplies to
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param param parameter error vector to fill
    */
    virtual void fillParameterErrors(const QFRawDataRecord* r,  const QString& resultID, double* param, QFFitFunction* function=NULL) const ;
    /** \brief fill the given array of bools with the current parameter fix values, as appropriate to use together with QFFitFunction
     *  \param r record this appplies to
     *  \param resultID the result ID which to access in the raw data records result store
     *  \param param parameter fix vector to fill
    */
    virtual void fillFix(const QFRawDataRecord* r,  const QString& resultID, bool* param, QFFitFunction* function=NULL) const;

    /*! \brief fill a newly allocated array of doubles with the current parameter values, as appropriate to use together with QFFitFunction
        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \return parameter vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual double *allocFillParameters(const QFRawDataRecord* r,  const QString& resultID, QFFitFunction* function=NULL) const ;
    /*! \brief fill a newly allocated array of doubles with the current parameter errors, as appropriate to use together with QFFitFunction
        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \return parameter error vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual double* allocFillParameterErrors(const QFRawDataRecord* r,  const QString& resultID, QFFitFunction* function=NULL) const ;
    /*! \brief fill a newly allocated array of bools with the current parameter fix values, as appropriate to use together with QFFitFunction
        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \return parameter fix vector
        \note This function calls \c qfCalloc() internally, so you will have to free the created arrays using \c qfFree() of the C standard library!
    */
    virtual bool* allocFillFix(const QFRawDataRecord* r,  const QString& resultID, QFFitFunction* function=NULL) const ;


    /*! \brief reset the given parameter \a id to the initial/global/default value */
    virtual void resetDefaultFitValue(const QString& id);
    /*! \brief reset the given parameter \a id to the initial/global/default fix */
    virtual void resetDefaultFitFix(const QString& id);
    /*! \brief return the default/initial/global value of a given parameter        */
    virtual double getDefaultFitValue(const QString& id, const QFRawDataRecord *r=NULL) const;
    /*! \brief return the default/initial/global fix of a given parameter        */
    virtual bool getDefaultFitFix(const QString& id, const QFRawDataRecord *r=NULL) const ;
    /*! \brief reset the all fit results in the given record and resultID therein */
    virtual void resetAllFitResults(QFRawDataRecord *r, const QString& resultID);
    /*! \brief reset the all fit results to the initial/global/default value in the currently displayed curve/data */
    virtual void resetAllFitResultsCurrent(QFRawDataRecord *r=NULL);
    /*! \brief reset the all fit results to the initial/global/default value in all raw data records */
    virtual void resetAllFitResultsAllFiles();
    /*! \brief reset all parameters to the initial/global/default value in current file and resultID */
    virtual void resetAllFitValueCurrent(QFRawDataRecord *r=NULL);
    /*! \brief reset all parameters to the initial/global/default fix in current file and resultID */
    virtual void resetAllFitFixCurrent(QFRawDataRecord *r=NULL);
    /*! \brief set a fit parameter of the current fit function (see m_fitFunction) to the specified value in the initial parameters
        \param id set the value of the parameter with this id (see QFFitFunction)
        \param value value to be stored
        \param error error assigned to \a value

     */
    virtual void setInitFitValue(const QString& id, double value, double error=0.0, QFRawDataRecord *r=NULL);
    /*! \brief set a fit parameter of the current fit function (see m_fitFunction) to the specified value in the initial parameters
        \param id set the value of the parameter with this id (see QFFitFunction)
        \param value value to be stored
        \param error error assigned to \a value

     */
    virtual void setInitFitError(const QString& id, double error, QFRawDataRecord *r=NULL);
    /*! \brief set the fix property of a fit parameter in the initial parameters
        \param id set the value of the parameter with this id (see QFFitFunction)
        \param fix value to be stored
     */
    virtual void setInitFitFix(const QString& id, bool fix, QFRawDataRecord *r=NULL);




    /*! \brief save all parameters of a given QFFitAlgorithm to the internal fit algorithm parameter store algorithm_parameterstore
    */
    //virtual void storeQFFitAlgorithmParameters(QFFitAlgorithm* algorithm);
    /*! \brief load all parameters of a given QFFitAlgorithm from the internal fit algorithm parameter store algorithm_parameterstore
    */
    //virtual void restoreQFFitAlgorithmParameters(QFFitAlgorithm* algorithm) const;



    /*! \brief return a resultID for storage of fit results in a QFRawDataRecord results store. The ID is valid for the currently highlighted record and all current settings */
    virtual QString getEvaluationResultID(const QFRawDataRecord *r) const=0;
    /*! \brief reset the given parameter value (\a id) in the given raw data record \a r for the \a resultID to its default/initial value */
    virtual void resetDefaultFitValue(QFRawDataRecord *r, const QString &resultID, const QString &id);
    /*! \brief reset the given parameter fix (\a id) in the given raw data record \a r for the \a resultID to its default/initial value */
    virtual void resetDefaultFitFix(QFRawDataRecord *r, const QString &resultID, const QString &id);

    virtual void setFitResultFitStatistics(QFRawDataRecord *record, const QString &evalID, const QFFitStatistics &result, const QString &prefix, const QString &group);
    virtual void setFitResultBasicFitStatistics(QFRawDataRecord *record, const QString &evalID, const QFBasicFitStatistics &result, const QString &prefix, const QString &group);

    virtual void getFitResultFitStatistics(QFRawDataRecord *record, const QString &evalID, QFFitStatistics &result, const QString &prefix) const;
    virtual void getFitResultBasicFitStatistics(QFRawDataRecord *record, const QString &evalID, QFBasicFitStatistics &result, const QString &prefix) const;

signals:

public slots:

protected:
    /*! \brief implement this function and return \c true, if you want to override the fit functions preset */
    virtual bool overrideFitFunctionPreset(const QFRawDataRecord *r, QString paramName, double&value) const;
    /*! \brief implement this function and return \c true, if you want to override the fit functions preset error */
    virtual bool overrideFitFunctionPresetError(const QFRawDataRecord *r, QString paramName, double &value) const ;
    /*! \brief implement this function and return \c true, if you want to override the fit functions preset fix */
    virtual bool overrideFitFunctionPresetFix(const QFRawDataRecord *r, QString paramName, bool &value) const ;


    /*! \brief struct used to locally store fit parameter properties

        For each parameter that is stored in parameterStore this saves also whether it is really set, so each
        parameter is represented by two datamemeber: \c value and \c valueSet which is \c true only if the user
        has explicitly set the \c value. This information is used in the \c getFitXYZ() functions and set in the
        \c setFitXYZ() functions.
    */
    struct FitParameter {
        double value;
        double error;
        bool fix;
        double min;
        double max;
        bool valueSet;
        bool fixSet;
        bool minSet;
        bool maxSet;
        bool errorSet;
        FitParameter() {
            value=0;
            error=0;
            fix=false;
            min=0;
            max=0;
            valueSet=false;
            fixSet=false;
            minSet=false;
            maxSet=false;
            errorSet=false;
        }
    };

    /*! \brief datastore for fit parameter values

        Internally a uniquie key for every fit parameter is used to adress the contents. The key is calculated
        from the \code fit_fuction_id+"___"+parameter_id \endcode
     */
    QHash<QString, FitParameter> parameterStore;

    /*! \brief parameter store for fit algorithm parameters

    This map effectively has two strings as key. the first is the fit algorithm ID and the second the parameter name
    The value stored for each key is a QVariant.
    */
    //QHash<QString, QHash<QString, QVariant> > algorithm_parameterstore;


    /** \brief the current fitting function */
    QString m_fitFunction;
    /** \brief the current fitting algorithm */
    QString m_fitAlgorithm;
    /** \brief map with all available fit functions */
    QMap<QString, QFFitFunction*> m_fitFunctions;
    /** \brief map with all available fit algorithms */
    QMap<QString, QFFitAlgorithm*> m_fitAlgorithms;

    /** \brief settings object to access global fit parameters */
    QSettings* fitParamGlobalSettings;
    /** \brief settings object to access fit parameters */
    QSettings* fitParamSettings;


    /** \brief thread used for fitting */
    QFFitAlgorithmThreadedFit* doFitThread;



    /** \brief return a valid ID to access parameterStore for the given parameter (id) in the current fit function (m_fitFunction) */
    QString getParameterStoreID(const QFRawDataRecord *rdr, QString parameter) const;

    /** \brief return a valid ID to access parameterStore for the given parameter (id) in the current fit function (m_fitFunction) */
    QString getParameterStoreID(const QFRawDataRecord *rdr, QString fitfunction, QString parameter) const;

    /** \brief if this returns a non-empty string (default: empty string), the result is added to the getParameterStoreID() to make the store specific to the given \a rdr */
    virtual QString rdrPointerToParameterStoreID(const QFRawDataRecord* rdr) const;

    /** \brief write object contents into XML file
    *
    *  this function saves the id of the current fit function and algorithm, as well as the contents of parameterStore to
    *  the given XML file.
    */
    virtual void intWriteData(QXmlStreamWriter& w) const;
    /** \brief read back the data stored by intWriteXML() */
    virtual void intReadData(QDomElement* e);

    /*! \brief allows to read additional information from a fit parameter node in the project file

     \param parameterID id of the current fit parameter
     \param fitParam for your information the so far save information
     \param e QDomElement to read from

    */
    virtual void intReadDataFitParam(const QString& parameterID, const FitParameter& fitParam, QDomElement& e) {Q_UNUSED(parameterID)Q_UNUSED(fitParam)Q_UNUSED(e)}
    /*! \brief allows to write additional information to a fit parameter node in the project file

     \param parameterID id of the current fit parameter
     \param w XML writer, currently in the parameter node (i.e. write subnode with w.writeStartElement() and attributes with w.writeAttribute() )
    */
    virtual void intWriteDataFitParam(const QString& parameterID, QXmlStreamWriter& w) const {Q_UNUSED(w)Q_UNUSED(parameterID)}
    /*! \brief allows to write additional information to a fit algorithm node in the project file

     \param w XML writer, currently in the parameter node (i.e. write subnode with w.writeStartElement() and attributes with w.writeAttribute() )
    */
    virtual void intWriteDataAlgorithm(QXmlStreamWriter& w) const {Q_UNUSED(w)}

    /*! \brief allows to read additional information from a fit algorithm node in the project file

     \param e QDomElement to read from

    */
    virtual void intReadDataAlgorithm(QDomElement& e) {Q_UNUSED(e)}



    /** \brief returns an ID for a given fit parameter (i.e. prepends \c fitparam_ and if \a fix is \c true, also appends \c _fix ) */
    inline QString getFitParamID(QString fitparam) const {
        return QString("fitparam_%1").arg(fitparam);
    }

    /** \brief returns an ID for a given fit parameter fix (i.e. prepends \c fitparam_ and appends \c _fix ) */
    inline QString getFitParamFixID(QString fitparam) const {
        return QString("fitparam_%1_fix").arg(fitparam);
    }
    /*! \brief allows to draw certain fit parameters from other sources (e.g. copy a property of the underlying record

        \param r record this appplies to
        \param resultID the result ID which to access in the raw data records result store
        \param paramid the parameter we are looking up
        \param[out] value in this parameter the value is returned, if the function returns \c true
        \param[out] error in this parameter the value' error is returned, if the function returns \c true
        \return \c true if a special value is found. In that case it returns the value in \a value and \a error

     */
    virtual bool hasSpecial(const QFRawDataRecord* r, const QString& resultID, const QString& paramid, double& value, double& error) const;
};

#endif // QFFITRESULTSEVALUATION_H
