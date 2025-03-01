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

#ifndef QFGLOBALFITTOOL_H
#define QFGLOBALFITTOOL_H
#include "qffitalgorithm.h"
#include "lib_imexport.h"
#include <QThread>
#include <QObject>
#include "qffitfunction.h"

/*! \brief this is a special functor which is used for data fitting of a multi-dataterm fit problem with objective functions declared by QFFitFunction
    \ingroup qf3lib_fitting

            \image html QFFitMultiQFFitFunctionFunctor.png
            \image latex QFFitMultiQFFitFunctionFunctor.png


    The QFFitFunction declares a set of \f$ k=1..K\f$ 1D function \f$ f_k(x; \vec{p}_k), \vec{p}_k\in\mathbb{R}^{N_k} \f$ .
    This functor is provided with a set of data \f$ (x_{k,m}, y_{k,m}, \sigma_{k,m}), m=1..M_k \f$
    where \f$ \sigma_{k,m} \f$ are weights for the data.

    Then This Functor implementation maps the 1D Function \f$ f_k(x; \vec{p}_k) \f$ to a \f$ \sum\limits_kM_k \f$ dimensional function
    \f$ \vec{g}(\vec{p}_1,...,\vec{p}_K) \f$ with:
        \f[ \vec{g}(\vec{p}_1,...,\vec{p}_K) = \begin{pmatrix}
            (y_{1,1}-f_1(x_{1,1}; m(\vec{p}_1)))/\sigma_{1,1} \\
            \ldots \\
            (y_{1,M_1}-f_1(x_{1,M_1}; m(\vec{p}_1)))/\sigma_{1,M_1} \\
            (y_{2,1}-f_2(x_{2,1}; m(\vec{p}_2)))/\sigma_{2,1} \\
            \ldots \\
            (y_{2,M_2}-f_2(x_{2,M_2}; m(\vec{p}_2)))/\sigma_{2,M_2} \\
            \ldots \\
            \ldots \\
            (y_{K,M_K}-f_K(x_{K,M_K}; m(\vec{p}_K)))/\sigma_{K,M_K} \\
            \end{pmatrix}
        \f]
    This allows to solve global optimization problems where the objective function contains several dataterms:
      \f[ \xhi^2(\vec{p}_1,...,\vec{p}_K)=\vec{g}(\vec{p}_1,...,\vec{p}_K)\cdot\vec{g}(\vec{p}_1,...,\vec{p}_K) = \sum\limits_{k=1}^K \sum\limits_{m=^}^{M_k}\left|\frac{y_{k,m}-f_k(x_{k,m}; \vec{p}_k)}{\sigma_{k,m}}\right|^2 \f]
    Internally this class uses a set of FitQFFitFunctionFunctor objects to evaluate each separate fit function.

    In addition this class allows to link some of the parameters together: A set of global parameter \f$ P_i \f$ may be used
    in several of the single parameter vectors \f$ \vec{p}_k \f$. So for each \f$ P_i \f$ a list of pairs \f$ \langle k,\iota_p\rangle_j \f$
    has to be stored that says which parameter \f$ \iota_p \f$ (as index in \f$ \vec{p}_k \f$ ) in the k-th data-term is set to the global
    parameter \f$ P_i \f$. The parameter of evaluate() will then have this format:
\beginverbatim
     N(globals)       N(1)-N(1globals)             N(2)-N(2globals)
+-----------------+----------------------+-------------------------------------+ . . . . . . .
|    global       |    local parameter   |           local parameter           |
| parameters P(.) | of dataterm 1: p1(.) |        of dataterm 2: p2(.)         |
+-----------------+----------------------+-------------------------------------+ . . . . . . .
              ++++                     ++                                     ++
          ++++                        +  +                                      ++
      ++++                           +    +                                       ++
  ++++                              +      +                                        ++
  +---------------------------------+      +------------------------------------------+ . . . . . . .
  |     PARAMETER VECTOR TO         |      |           PARAMETER VECTOR TO            |
  |     dataterm1.evaluate()        |      |           dataterm2.evaluate()           |
  +---------------------------------+      +------------------------------------------+ . . . . . . .
  | p1(0), p1(1), P(0), p1(2), ...  |      | p2(0), p2(1), P(0), p2(2), P(1), p2(3)...|
  +---------------------------------+      +------------------------------------------+ . . . . . . .
          N(1) elements                                   N(2) elements
\endverbatim
    Here \c N(1), \c N(2),... are the number of parameters in each data term and \c N(1globals), \c N(2globals),... are
    the number of these parameters, replaced by global parameters. The data of each data term is stored in an element
    of subFunctors. The functor subFunctorData::f is used to evaluate \f$ f_k(x; \vec{p}_k) \f$, the property
    subFunctorData::mapToLocal contains the index in the main parameter array (top in image above) of the
    according parameter.

*/
class QFLIB_EXPORT QFFitMultiQFFitFunctionFunctor: public QFFitAlgorithm::Functor, public QFFitAlgorithm::FunctorBootstrapInterface {
    public:
        /*! \brief constructor, initializes the functor
        */
        QFFitMultiQFFitFunctionFunctor() ;

        virtual ~QFFitMultiQFFitFunctionFunctor();


        /** \brief evaluate the function \f$ \vec{g}(\vec{q}) \f$ */
        virtual void evaluate(double* evalout, const double* params) const;

        /** \brief evaluate the functions jacobian \f$ J_{n,m}(\vec{q})=\frac{\partial g_m(\vec{q})}{\partial q_n}=-\frac{1}{\sigma_m}\cdot\frac{\partial f(x_m, m(\vec{q}))}{\partial m(q_n)} \f$ */
        virtual void evaluateJacobian(double* evalout, const double* params) const;

        /** \brief returns \c true if all models implement theor jacobians analytically and therefore evaluateJacobian() may be used */
        virtual bool get_implementsJacobian() const;

        /** \brief return the number of parameters \f$ Q \f$ in \f$ \vec{q}=m(\vec{p}) \f$ */
        virtual int get_paramcount() const;

        /** \brief add data term */
        void addTerm(QFFitFunction* model, const double* currentParams, const bool* fixParams, const double* dataX, const double* dataY, const double* dataWeight, uint64_t M, const double *paramsMin=NULL, const double *paramsMax=NULL);
        /** \brief init linking information */
        void setAllLinks(const QList<QList<QPair<int, int> > >& linkedParams);
        /** \brief add global parameter */
        void addGlobalParam(const QList<QPair<int, int> > & links);
        /** \brief add linking information to global parameter */
        void addToGlobalParam(int param, QPair<int, int> link);
        /** \brief add linking information to global parameter */
        void addToGlobalParam(int param, int term, int localParam);
        /** \brief set the number of global parameters
         *
         *  \note This implicitly clears all links from the functor!
         */
        void setGlobalParamCount(int count);

        /** \brief clear object */
        void clear();

        bool isLinkedSingle(int idx) const;
        /** \brief returns the IDX of the global parameter (i.e. of the outer list in linkedParams) to which the pait <dataTermID, parameterID> is linked */
        int getLinkID(int dataTermID, int parameterID);
        /** \brief returns the number of sub functors */
        inline int getSubFunctorCount() const { return subFunctors.size(); }
        inline  QFFitFunction* getModel(int idx) const { return subFunctors[idx].f->getModel(); }
        inline double* getParamsMin(int idx) const { return subFunctors[idx].paramsMin; }
        inline double* getParamsMax(int idx) const { return subFunctors[idx].paramsMax; }
        inline bool* getParamsFix(int idx) const { return subFunctors[idx].paramsFix; }
        inline  QFFitAlgorithm::FitQFFitFunctionFunctor* getSubFunctor(int idx) const { return subFunctors[idx].f; }
        inline int getLinkedPrameterCount() const { return m_linkedParamsCount; }
        inline int mapSubFunctorToGlobal(int functor, int parameter) const { return subFunctors[functor].mapToLocal[parameter]; }

        void setDoRecalculateInternals(bool enabled);

        /** \copydoc QFFitAlgorithm::FitFunctionFunctor::isWeightedLSQ() */
        virtual bool isWeightedLSQ() const;

        virtual bool areAllWeightsOne() const;




        /** \brief prepares a new selection of data for bootstrapping
         *
         * \note if you only want to reapply the current selection and not select new data, call reapplyBootstrapselection() instead.
         */
        virtual void prepareBootstrapSelection();
        /** \brief this function reapplies the current bootstrappig selection, i.e. if the input-data changed, but not its number and also the selection should not be recreated.
         *
         * \note to create a new subset from the input data, call prepareBootstrapSelection()
         */
        virtual void reapplyBootstrapselection();
        /** \brief switches bootstrapping on and off, if \c enabled=true and \c prepBootstrapping=true, the function prepareBootstrapSelection() is called */
        virtual void setBootstrappingEnabled(bool enabled, bool prepBootstrapping=true);
        /** \brief sets the fraction of the datapoints, that are selected by prepareBootstrapSelection(), if \c bootstrappingEnabled=true and \c prepBootstrapping=true, the function prepareBootstrapSelection() is called */
        virtual void setBootstrappingFraction(double fraction, bool prepBootstrapping=true);

        virtual void evaluateJacobianNum(double* evalout, const double* params, double h=10.0*QF_SQRT_DBL_EPSILON) const;

        /** \brief calculate the \f$ \chi^2=\sum\limits_i\left(\frac{y_i-f(x_i)}{\sigma_i}\right)^2 */
        double calculateChi2(const double *params) const;

        inline QStringList getParamNames() const { return paramNames; }
    protected:
        struct subFunctorData {
            /** \brief functor to evaluate this data term */
            QFFitAlgorithm::FitQFFitFunctionFunctor* f;
            /** \brief number of local parameters to this data term */
            int localParams;
            /** \brief maps from the global parameter vector (as supplied to evaluate() ) to the local vector, as supplied to f->evaluate() */
            int* mapToLocal;
            /** \brief parameter lower bounds (if any) */
            double* paramsMin;
            /** \brief parameter upper bounds (if any) */
            double* paramsMax;
            /** \brief parameter vector */
            double* modelParams;
            /** \brief parameter fix state */
            bool* paramsFix;
        };

        /** \brief functions to evaluate the single data terms */
        QList<subFunctorData> subFunctors;

        /*! \brief encodes the linked parameters.

            This data structure contains a list of global parameters. For each such parameter it contains a list
            of <dataTermID, parameterID> pairs that specify which properties are linked together.
         */
        QList<QList<QPair<int, int> > > linkedParams;

        int m_paramCount;
        QStringList paramNames;
        int m_linkedParamsCount;

        bool doRecalculateInternals;

        bool bsEnabled;
        double bsFraction;

        void recalculateInternals();

};





/*! \brief tool class to perform global fitting with linked parameters, using QFFitMultiQFFitFunctionFunctor
    \ingroup qf3lib_fitting
 */
class QFLIB_EXPORT QFGlobalFitTool {
    public:
        QFGlobalFitTool(QFFitAlgorithm* algorithm);
        ~QFGlobalFitTool();

        /** \brief add data term */
        void addTerm(QFFitFunction* model, const double* currentParams, const bool* fixParams, const double* dataX, const double* dataY, const double* dataWeight, uint64_t M, const double *paramsMin=NULL, const double *paramsMax=NULL);
        /** \brief init linking information */
        void setAllLinks(const QList<QList<QPair<int, int> > >& linkedParams);
        /** \brief add global parameter */
        void addGlobalParam(const QList<QPair<int, int> > & links);
        /** \brief add linking information to global parameter */
        void addToGlobalParam(int param, QPair<int, int> link);
        /** \brief add linking information to global parameter */
        void addToGlobalParam(int param, int term, int localParam);
        /** \brief set the number of global parameters */
        void setGlobalParamCount(int count);
        /** \brief clear object */
        void clear();

        void setRepeats(int fitRepeat);
        void setGlobalLocalRepeats(int fitRepeat);

        void setDoRecalculateInternals(bool enabled);

        void createLocalFitFunctors();


        QFFitAlgorithm::FitResult fit(const QList<double *>& paramsOut, const QList<double *>& paramErrorsOut, const QList<double *>& initialParams, const QList<double *>& paramErrorsIn, QFBasicFitStatistics* fitstat=NULL, QVector<double> *rawFitResults=NULL, QVector<double> *rawFitResultsErrors=NULL, double *chi2=NULL, QVector<double> *cov_matrix=NULL, QStringList *paramNames=NULL, QVector<double>* fitX=NULL, QVector<double>* fitY=NULL, QVector<double>* fitW=NULL, QVector<double>* fitM=NULL);
        void evalueChi2Landscape(double* chi2Landscape, int paramXFile, int paramXID, const QVector<double>& paramXValues, int paramYFile, int paramYID, const QVector<double>& paramYvalues, const QList<double *>& initialParams);

        QFFitMultiQFFitFunctionFunctor* getFunctor() const;

    protected:

        QFFitAlgorithm* m_algorithm;
        QFFitMultiQFFitFunctionFunctor* functor;
        QList<QFFitAlgorithm::FitQFFitFunctionFunctor*> localFunctors;
        int m_repeatFit;
        int m_globalLocalRepeats;

        QFFitAlgorithm::FitResult fitGlobal(const QList<double *>& paramsOut, const QList<double *>& paramErrorsOut/*, const QList<double *>& initialParams, const QList<double *>& paramErrorsIn*/, QVector<double> *rawFitResults=NULL, QVector<double> *rawFitResultsErrors=NULL, double *chi2=NULL, QVector<double> *cov_matrix=NULL, QStringList *paramNames=NULL, QVector<double>* fitX=NULL, QVector<double>* fitY=NULL, QVector<double>* fitW=NULL, QVector<double>* fitM=NULL);
        QList<QFFitAlgorithm::FitResult> fitLocal(const QList<double *>& paramsOut, const QList<double *>& paramErrorsOut/*, const QList<double *>& initialParams, const QList<double *>& paramErrorsIn*/);
};




/*! \brief QThread to perform a global fit using QFGlobalFitTool
    \ingroup qf3lib_fitting

    After allocating the memory for an object, call init() to intialize the data. Then you may
    start() the thread. Finally the results may be read using getResult() and the fit runtime in milliseconds using
    getDeltaTime().

*/
class QFLIB_EXPORT QFGlobalThreadedFit: public QThread {
        Q_OBJECT
    public:
        /** \brief class cosntructor */
        explicit QFGlobalThreadedFit(QObject* parent=NULL);
        /** \brief class destructor */
        virtual ~QFGlobalThreadedFit();

        /** \brief initialize the fit parameters */
        void init(QFGlobalFitTool* globalTool, QList<double*> paramsOut, QList<double*> paramErrorsOut, QList<double*> initialParams, QList<double *> paramErrorsIn, double* chi2=NULL, QFBasicFitStatistics* fitstat=NULL, QVector<double> *rawFitResults=NULL, QVector<double> *rawFitResultsErrors=NULL, QVector<double>* COV=NULL, QStringList* paramNames=NULL, QVector<double>* fitX=NULL, QVector<double>* fitY=NULL, QVector<double>* fitW=NULL, QVector<double>* fitM=NULL);

        /** \brief return the result of the last fit call */
        inline QFFitAlgorithm::FitResult getResults() const { return results; }
        /** \brief return the runtime of the fit in milliseconds */
        inline double getDeltaTime() const { return deltaTime; }

    protected:
        QList<double*> paramsOut;
        QList<double*> paramErrorsOut;
        QFGlobalFitTool* globalTool;
        QList<double*> initialParams;
        QList<double *> paramErrorsIn;
        QFFitAlgorithm::FitResult results;
        double* chi2;
        QVector<double>* COV;
        QStringList* paramNames;
        double deltaTime;
        QVector<double>* fitX;
        QVector<double>* fitY;
        QVector<double>* fitW;
        QVector<double>* fitM;

        QFBasicFitStatistics* fitstat;
        QVector<double> *rawFitResults;
        QVector<double> *rawFitResultsErrors;

        /** \brief here the fitting takes place */
        virtual void run();
};

#endif // QFGLOBALFITTOOL_H
