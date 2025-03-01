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


#include "qffitfunctiongeneralsigmoid.h"
#include "qfmathtools.h"
#include "statistics_tools.h"
#include <cmath>


QFFitFunctionGeneralSigmoid::QFFitFunctionGeneralSigmoid() {
    //           type,         id,                        name,                                                    label (HTML),                      unit,          unitlabel (HTML),        fit,       userEditable, userRangeEditable, displayError, initialFix,                initialValue, minValue, maxValue, inc, absMin, absMax
    addParameter(FloatNumber,  "offset",                    "offset",                                                  "Y<sub>0</sub>",                   "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 0.0,          -DBL_MAX, DBL_MAX,  1,   -DBL_MAX,    DBL_MAX  );
    #define PARAM_BASE 0
    addParameter(FloatNumber,  "max",                    "max",                                                  "Y<sub>max</sub>",                   "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 1.0,          -DBL_MAX, DBL_MAX,  1,   -DBL_MAX,    DBL_MAX  );
    #define PARAM_MAX 1
    addParameter(FloatNumber,  "position",                    "position",                                         "X<sub>1/2</sub>",                  "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 0.0,          -DBL_MAX, DBL_MAX,  1,   -DBL_MAX,    DBL_MAX  );
    #define PARAM_POSITION 2
    addParameter(FloatNumber,  "rate",                    "rate",                                                               "w",                  "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 1.0,          -DBL_MAX, DBL_MAX,  1,   -DBL_MAX,    DBL_MAX  );
    #define PARAM_RATE 3

}

double QFFitFunctionGeneralSigmoid::evaluate(double t, const double* data) const {
    return data[PARAM_BASE]+(data[PARAM_MAX]-data[PARAM_BASE])/(1.0+exp((data[PARAM_POSITION]-t)/data[PARAM_RATE]));

}


void QFFitFunctionGeneralSigmoid::calcParameter(double* /*data*/, double* /*error*/) const {

}

bool QFFitFunctionGeneralSigmoid::isParameterVisible(int parameter, const double* data) const {

    Q_UNUSED(parameter);
    Q_UNUSED(data);
    return true;
}
unsigned int QFFitFunctionGeneralSigmoid::getAdditionalPlotCount(const double* params) const {

    Q_UNUSED(params);
    return 0;
}

QString QFFitFunctionGeneralSigmoid::transformParametersForAdditionalPlot(int plot, double* params) const {


    Q_UNUSED(plot);
    Q_UNUSED(params);
    return "";
}

bool QFFitFunctionGeneralSigmoid::get_implementsDerivatives() const
{
    return false;
}
