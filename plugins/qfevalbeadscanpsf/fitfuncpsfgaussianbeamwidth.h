/*
Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate: 2014-09-26 12:40:44 +0200 (Fr, 26 Sep 2014) $  (revision $Rev: 3508 $)

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

#ifndef QFFitFunctionGeneralGaussianBeamWidth_H
#define QFFitFunctionGeneralGaussianBeamWidth_H
#include "qfpluginfitfunction.h"




/*! \brief QFFitFunction class for fit function: PSF: Gaussian Beam: Width(z)
    \ingroup qf3rdrdp_eval_beadscanpsf

*/
class QFFitFunctionPSFGaussianBeamWidth: public QFFitFunction {
    public:
        QFFitFunctionPSFGaussianBeamWidth();
        virtual ~QFFitFunctionPSFGaussianBeamWidth() {}
        /*! \copydoc QFFitFunction::name()   */
        virtual QString name() const { return QObject::tr("PSF: Gaussian Beam: Width(z)"); };
        /** \copydoc QFFitFunction::shortName() */
        virtual QString shortName() const { return name(); };
        /*! \copydoc QFFitFunction::id()   */
        virtual QString id() const { return QString("beadscan_psf_gaussianbeam_width"); };
        /*! \copydoc QFFitFunction::category()   */
        virtual QString category() const { return QString("Point Spread Function (PSF)"); };

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

        bool estimateInitial(double *params, const double *dataX, const double *dataY, long N, const bool *fix=NULL) const;
};

#endif // QFFitFunctionGeneralGaussianBeamWidth_H
