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


#ifndef QFFitFunctionGeneralParabula_H
#define QFFitFunctionGeneralParabula_H
#include "qfpluginfitfunction.h"




/*! \brief QFFitFunction class for fit function
    \ingroup qf3fitfunp_fitfunction_general

*/
class QFFitFunctionGeneralParabula: public QFFitFunction {
    public:
        QFFitFunctionGeneralParabula();
        virtual ~QFFitFunctionGeneralParabula() {}
        /*! \copydoc QFFitFunction::name()   */
        virtual QString name() const { return QObject::tr("General: Parabula (Polynom of degree 2)"); }
        /** \copydoc QFFitFunction::shortName() */
        virtual QString shortName() const { return QObject::tr("General: Parabula"); }
        /*! \copydoc QFFitFunction::id()   */
        virtual QString id() const { return QString("gen_parabula"); }
        /*! \copydoc QFFitFunction::category()   */
        virtual QString category() const { return QObject::tr("General Fit Functions"); }

        /*! \copydoc QFFitFunction::evaluate()   */
        virtual double evaluate(double t, const double* parameters) const;

        /*! \copydoc QFFitFunction::calcParameter()   */
        virtual void calcParameter(double* parameterValues, double* error=NULL) const;

        /*! \copydoc QFFitFunction::isParameterVisible()   */
        virtual bool isParameterVisible(int parameter, const double* parameterValues) const;
        /*! \copydoc QFFitFunction::getAdditionalPlotCount()   */
        virtual unsigned int getAdditionalPlotCount(const double* params) const;

        /*! \copydoc QFFitFunction::transformParametersForAdditionalPlot()   */
        virtual QString transformParametersForAdditionalPlot(int plot, double* params) const;

        /*! \copydoc QFFitFunction::get_implementsDerivatives()   */
        virtual bool get_implementsDerivatives() const;


        /*! \brief if implemented (and returns \c true) this function tries to estimate the initial parameters of a fit function from provided data. */
        virtual bool estimateInitial(double* params, const double* dataX, const double* dataY, long N, const bool *fix=NULL) const;

};

#endif // QFFitFunctionGeneralParabula_H
