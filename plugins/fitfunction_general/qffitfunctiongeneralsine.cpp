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


#include "qffitfunctiongeneralsine.h"
#include "qfmathtools.h"
#include "statistics_tools.h"
#include <cmath>


QFFitFunctionGeneralSine::QFFitFunctionGeneralSine() {
    //           type,         id,                        name,                                                    label (HTML),                      unit,          unitlabel (HTML),        fit,       userEditable, userRangeEditable, displayError, initialFix,                initialValue, minValue, maxValue, inc, absMin, absMax
    addParameter(FloatNumber,  "offset",                    "offset",                                                  "Y<sub>0</sub>",                   "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 0.0,          -DBL_MAX, DBL_MAX,  1,   -DBL_MAX,    DBL_MAX  );
    #define PARAM_OFFSET 0
    addParameter(FloatNumber,  "amplitude",                    "amplitude",                                                  "A",                   "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 1.0,          -DBL_MAX, DBL_MAX,  1,   -DBL_MAX,    DBL_MAX  );
    #define PARAM_AMPLITUDE 1
    addParameter(FloatNumber,  "frequency",                    "frequency",                                         "f",                  "",            "",                      true,      true,         true,              QFFitFunction::DisplayError,       false, 10,          0, DBL_MAX,  1,   0,    DBL_MAX  );
    #define PARAM_FREQUENCY 2
    addParameter(FloatNumber,  "phase",                    "phase",                                         "&phi;",                  "2*pi",            "2&pi;",                      true,      true,         true,              QFFitFunction::DisplayError,       true, 0,          0, 2.0,  1,   -2.0,    2.0  );
    #define PARAM_PHASE 3
    addParameter(FloatNumber,  "period",                    "period",                                         "T",                  "1/[f]",            "1/[f]",                      false,      false,         false,              QFFitFunction::DisplayError,       true, 0);
    #define PARAM_PERIOD 4

}

double QFFitFunctionGeneralSine::evaluate(double t, const double* data) const {
    return data[PARAM_OFFSET]+data[PARAM_AMPLITUDE]*sin(2.0*M_PI*(data[PARAM_FREQUENCY]*t+data[PARAM_PHASE]));

}


void QFFitFunctionGeneralSine::calcParameter(double* data, double* error) const {
    if (data) {
        data[PARAM_PERIOD]=1.0/data[PARAM_FREQUENCY];
        if (error) error[PARAM_PERIOD]=qfErrorDiv(1,0,data[PARAM_FREQUENCY],error[PARAM_FREQUENCY]);
    }
}

bool QFFitFunctionGeneralSine::isParameterVisible(int parameter, const double* data) const {

    Q_UNUSED(parameter);
    Q_UNUSED(data);
    return true;
}
unsigned int QFFitFunctionGeneralSine::getAdditionalPlotCount(const double* params) const {

    Q_UNUSED(params);
    return 0;
}

QString QFFitFunctionGeneralSine::transformParametersForAdditionalPlot(int plot, double* params) const {


    Q_UNUSED(plot);
    Q_UNUSED(params);
    return "";
}

bool QFFitFunctionGeneralSine::get_implementsDerivatives() const
{
    return false;
}



