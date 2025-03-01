/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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



#ifndef QFRDRCOLUMNGRAPHINTERFACE_H
#define QFRDRCOLUMNGRAPHINTERFACE_H

#include "qfplugin.h"
#include <QColor>
#include <QVariant>

/*! \brief interface for column oriented graphs
    \ingroup qf3rdrdp_fcs

    \note a single curve is called graph and is grouped into plots (i.e. axes)
*/
class QFRDRColumnGraphsInterface {
    public:
        virtual ~QFRDRColumnGraphsInterface() {}

        enum ColumnGraphTypes {
            cgtLines,
            cgtLinesPoints,
            cgtPoints,
            cgtBars,
            cgtSteps,
            cgtImpulses,
            cgtExpression,
            cgtPolynomial,
            cgtExponential,
            cgtPowerLaw,
            cgtQFFitFunction,
            cgtImage,
            cgtRGBImage,
            cgtMaskImage,
            cgtHorizontalRange,
            cgtVerticalRange,
            cgtBoxPlot,
            cgtParametrizedScatter
        };

        enum Orientation {
            cgoHorizontal,
            cgoVertical
        };
        enum ParametrizationType {
            cgptColorLines,
            cgptColorSymbols,
            cgptSize
        };


        enum ErrorGraphTypes {
            egtNone=0,
            egtBars=4,
            egtLines=5,
            egtPolygons=3,
            egtLinesBars=2,
            egtPolygonsBars=1,
            egtSimpleBars=6,
            egtSimpleBarsLines=7,
            egtSimpleBarsPolygons=8,
            egtBoxes=9,
            egtEllipses=10

        };
        enum ImageModifierMode {
            cgtiModifyNone=0,
            cgtiModifyValue=1,
            cgtiModifySaturation=2,
            cgtiModifyAlpha=3
        };

        enum ImageColorPalette {
            cgtiRED=0,
            cgtiINVERTEDRED=1,
            cgtiGREEN=2,
            cgtiINVERTEDGREEN=3,
            cgtiBLUE=4,
            cgtiINVERTEDBLUE=5,
            cgtiGRAY=6,
            cgtiINVERTEDGRAY=7,
            cgtiMATLAB=8,
            cgtiRYGB=9,
            cgtiHSV=10,
            cgtiINVERTED_HSV=11,
            cgtiRAINBOW=12,
            cgtiHOT=13,
            cgtiOCEAN=14,
            cgtiTRAFFICLIGHT=15,
            cgtiBLUEMAGENTAYELLOW=16,
            cgtiBLUEYELLOW=17,
            cgtiCYAN=18
        };

        enum ImageGraphRangeChannel {
            cgtiDataChannel=0,
            cgtiRedChannel=cgtiDataChannel,
            cgtiGreenChannel=1,
            cgtiBlueChannel=2,
            cgtiModifierChannel=3
        };

        enum ColumnGraphSymbols {
            cgsNoSymbol=0,             /*!< \brief plots no symbol at all (usefull together with error bars) */
            cgsDot=1,                 /*!< \brief a small dot */
            cgsCross=2,               /*!< \brief a X cross */
            cgsPlus=3,                /*!< \brief a + cross */
            cgsCircle=4,              /*!< \brief an unfilled circle */
            cgsFilledCircle=5,        /*!< \brief a filled circle */
            cgsRect=6,                /*!< \brief an unfilled rectangle */
            cgsFilledRect=7,          /*!< \brief a filled rectangle */
            cgsTriangle=8,            /*!< \brief an unfilled triangle (tip at top) */
            cgsFilledTriangle=9,      /*!< \brief a filled triangle (tip at top) */
            cgsDownTriangle=10,        /*!< \brief an unfilled triangle (tip at bottom) */
            cgsFilledDownTriangle=11,  /*!< \brief a filled triangle (tip at bottom) */
            cgsTarget=12
        };


        /** \brief returns, whether setting a graph property currently causes an immediate repaint */
        virtual bool colgraphGetDoEmitSignals() const=0;
        /** \brief enable/disable immediate repaints, when chaning graph properties.
         *
         * You can use this function to ensure that a block of colgraph...() methods are executet, with only a single repaint at the end (doEmit=true causes a repaint!)
         * \begincode
          bool en=tab.colgraphGetDoEmitSignals();
          tab.colgraphSetDoEmitSignals(false);
           ... // your colgraph... code
          tab.colgraphSetDoEmitSignals(en);
          \endcode
         */
        virtual void colgraphSetDoEmitSignals(bool doEmit)=0;

        /** \brief add a plot of  columnX against columnY to the given plot */
        virtual int colgraphAddGraph(int plot, int columnX, int columnY, ColumnGraphTypes type, const QString&  title)=0;
        /** \brief add a plot of  columnX against columnY to the given plot, as a parametrized scatter plot */
        virtual int colgraphAddParametrizedScatterGraph(int plot, int columnX, int columnY, int columnP, const QString&  title, ParametrizationType type=cgptColorLines, bool colorbarVisible=false, const QString& colorbarLabel=QString())=0;
        /** \brief add a boxplot */
        virtual int colgraphAddBoxPlot(int plot, Orientation orientation, int columnX, int columnMin, int columnQ25, int columnMedian, int columnMean, int columnQ75, int columnMax, const QString&  title)=0;
        /** \brief add a plot of  columnX against columnY to the given plot */
        virtual int colgraphAddErrorGraph(int plot, int columnX, int columnXError, int columnY, int columnYError, ColumnGraphTypes type, const QString&  title, ErrorGraphTypes errorStyle=egtBars)=0;
        /** \brief set the errorbar properties of a graph */
        virtual void colgraphSetErrorGraphProperties(int plot, int graph, int columnXError, int columnYError, ErrorGraphTypes errorStyle=egtBars)=0;
        /** \brief set the x-error column of a graph */
        virtual void colgraphSetErrorGraphErrorColumnX(int plot, int graphid, int columnXError)=0;
        /** \brief set the y-error column of a graph */
        virtual void colgraphSetErrorGraphErrorColumnY(int plot, int graphid, int columnYError)=0;

        /** \brief set asymmetric x-error column of a graph */
        virtual void colgraphSetErrorGraphErrorColumnXAsymmetric(int plot, int graphid, bool xErrorAsyymetric, int columnXErrorAsymmetric=-1)=0;
        /** \brief set asymmetric y-error column of a graph */
        virtual void colgraphSetErrorGraphErrorColumnYAsymmetric(int plot, int graphid, bool yErrorAsyymetric, int columnYErrorAsymmetric=-1)=0;

        /** \brief set the errorbar type of a graph */
        virtual void colgraphSetErrorGraphStyle(int plot, int graphid, ErrorGraphTypes errorStyle)=0;
        /** \brief add a new plot */
        virtual int colgraphAddPlot(const QString&  title, const QString& xLabel=QString("x"), const QString& yLabel=QString("y"), bool logX=false, bool logY=false)=0;
        /** \brief returns number of graphs in a plot */
        virtual int colgraphGetGraphCount(int plot) const=0;
        /** \brief returns number of pplots */
        virtual int colgraphGetPlotCount() const=0;
        /** \brief remove a plot */
        virtual void colgraphRemovePlot(int plot) =0;
        /** \brief remove a plot */
        virtual void colgraphRemoveGraph(int plot, int graph) =0;
        /** \brief set plot title */
        virtual void colgraphSetPlotTitle(int plot, const QString&  title)=0;
        /** \brief returns the plot title */
        virtual QString colgraphGetPlotTitle(int plot)=0;

        /** \brief set the line style of the graph */
        virtual void colgraphSetGraphLineStyle(int plot,  int graph,  Qt::PenStyle  style)=0;
        /** \brief set the line style of whiskers in the graph */
        virtual void colgraphSetGraphWhiskerLineStyle(int plot,  int graph,  Qt::PenStyle  style)=0;
        /** \brief set the line width of the graph */
        virtual void colgraphSetGraphLineWidth(int plot,  int graph,  double width)=0;
        /** \brief set the line style of the graph */
        virtual void colgraphSetGraphFillStyle(int plot,  int graph,  Qt::BrushStyle  style)=0;

        /** \brief set the color of the graph, the fill and error color are determined automatically */
        virtual void colgraphSetGraphColor(int plot,  int graph,  QColor  color)=0;
        /** \brief set the color of the graph, the error color is determined automatically */
        virtual void colgraphSetGraphColor(int plot,  int graph,  QColor  color, QColor  fillColor)=0;
        /** \brief set the color of the graph */
        virtual void colgraphSetGraphColor(int plot,  int graph,  QColor  color, QColor  fillColor, QColor errorColor)=0;
        /** \brief set the error color of the graph */
        virtual void colgraphSetGraphErrorColor(int plot,  int graph, QColor errorColor)=0;
        /** \brief set the fill color of the graph */
        virtual void colgraphSetGraphFillColor(int plot,  int graph, QColor fillColor)=0;
        /** \brief set the error color transparency */
        virtual void colgraphSetGraphErrorTransparency(int plot,  int graph, double errorT)=0;
        /** \brief set the fill color transparency */
        virtual void colgraphSetGraphFillTransparency(int plot,  int graph, double fillT)=0;
        /** \brief set the plot color transparency */
        virtual void colgraphSetGraphTransparency(int plot,  int graph, double trans)=0;
        /** \brief set the title of the graph */
        virtual void colgraphSetGraphTitle(int plot,  int graph,  const QString& title)=0;
        /** \brief set the type of the graph */
        virtual void colgraphSetGraphType(int plot,  int graph,  ColumnGraphTypes type)=0;
        /** \brief set the error style of the graph */
        virtual void colgraphSetGraphErrorStyle(int plot,  int graph,  ErrorGraphTypes errorType)=0;
        /** \brief set the plot symbol of the graph */
        virtual void colgraphSetGraphSymbol(int plot,  int graph,  ColumnGraphSymbols symbol, double symbolSize=10.0)=0;
        /** \brief set the width property of a graph */
        virtual void colgraphSetGraphWidth(int plot,  int graph,  double width)=0;
        /** \brief set the shift property of a graph */
        virtual void colgraphSetGraphShift(int plot,  int graph,  double shift)=0;

        /** \brief set a plot's x-axis properties */
        virtual void colgraphSetPlotXAxisProps(int plot, const QString& xLabel=QString("x"), bool logX=false)=0;
        /** \brief set a plot's y-axis properties */
        virtual void colgraphSetPlotYAxisProps(int plot, const QString& yLabel=QString("y"), bool logY=false)=0;

        /** \brief get a plot's x-axis label */
        virtual QString colgraphGetPlotXAxisLabel(int plot)=0;
        /** \brief get a plot's y-axis label */
        virtual QString colgraphGetPlotYAxisLabel(int plot)=0;

        /** \brief get a plot's x-axis Log */
        virtual bool colgraphGetPlotXAxisLog(int plot)=0;
        /** \brief get a plot's y-axis Log */
        virtual bool colgraphGetPlotYAxisLog(int plot)=0;
          /** \brief set a plot's y-axis label */
        inline virtual void colgraphSetPlotYAxisLabel(int plot, const QString& yLabel) {
            colgraphSetPlotYAxisProps(plot, yLabel, colgraphGetPlotYAxisLog(plot));
        }
        /** \brief set a plot's x-axis label */
        inline virtual void colgraphSetPlotXAxisLabel(int plot, const QString& xLabel) {
            colgraphSetPlotXAxisProps(plot, xLabel, colgraphGetPlotXAxisLog(plot));
        }

        /** \brief set x-axis range */
        virtual void colgraphSetPlotXRange(int plot, double xmin, double xmax)=0;
        /** \brief set y-axis range */
        virtual void colgraphSetPlotYRange(int plot, double xmin, double xmax)=0;
        /** \brief add a function graph which takes it's parameters from a column, or doesn't have parameters
         *
         *  \note This function only works with type==cgtExpression, cgtPolynomial, cgtExponential, cgtPowerLaw, cgtQFFitFunction. Depending on
         *        the value of type, the parameter expression either contains a math expression, like \c "p1+p2*sin(p3*x)" or the name of an available QFFitFunction
         *        For the special cases cgtPolynomial, cgtExponential, cgtPowerLaw, the expression is ignored!
         *        If any non-function type is used, the the type is automatically set to be cgtExpression.
         */
        virtual int colgraphAddFunctionGraph(int plot, const QString& expression, ColumnGraphTypes type, const QString&  title, int columnParam=-1)=0;
        /** \brief add a function graph which takes it's parameters from a vector of numbers
         *
         *  \note This function only works with typ==cgtExpression, cgtPolynomial, cgtExponential, cgtPowerLaw, cgtQFFitFunction. Depending on
         *        the value of type, the parameter expression either contains a math expression, like \c "p1+p2*sin(p3*x)" or the name of an available QFFitFunction
         *        For the special cases cgtPolynomial, cgtExponential, cgtPowerLaw, the expression is ignored!
         *        If any non-function type is used, the the type is automatically set to be cgtExpression.
         */
        virtual int colgraphAddFunctionGraph(int plot, const QString& expression, ColumnGraphTypes type, const QString&  title, const QVector<double>& params)=0;

        /** \brief sets the given graph to be  a function graph which takes it's parameters from a column, or doesn't have parameters
         *
         *  \note This function only works with type==cgtExpression, cgtPolynomial, cgtExponential, cgtPowerLaw, cgtQFFitFunction. Depending on
         *        the value of type, the parameter expression either contains a math expression, like \c "p1+p2*sin(p3*x)" or the name of an available QFFitFunction
         *        For the special cases cgtPolynomial, cgtExponential, cgtPowerLaw, the expression is ignored!
         *        If any non-function type is used, the the type is automatically set to be cgtExpression.
         */
        virtual void colgraphSetFunctionGraph(int plot, int graph, const QString& expression, ColumnGraphTypes type, const QString&  title, int columnParam=-1)=0;
        /** \brief sets the given graph to be a function graph which takes it's parameters from a vector of numbers
         *
         *  \note This function only works with typ==cgtExpression, cgtPolynomial, cgtExponential, cgtPowerLaw, cgtQFFitFunction. Depending on
         *        the value of type, the parameter expression either contains a math expression, like \c "p1+p2*sin(p3*x)" or the name of an available QFFitFunction
         *        For the special cases cgtPolynomial, cgtExponential, cgtPowerLaw, the expression is ignored!
         *        If any non-function type is used, the the type is automatically set to be cgtExpression.
         */
        virtual void colgraphSetFunctionGraph(int plot, int graph, const QString& expression, ColumnGraphTypes type, const QString&  title, const QVector<double>& params)=0;

        /** \brief add an image graph which
         */
        virtual int colgraphAddImageGraph(int plot, int imageColumn, ImageColorPalette palette, double x, double y, double width, double height, int Nx, const QString& title)=0;
        /** \brief add a mask image graph which
         */
        virtual int colgraphAddImageMaskGraph(int plot, int imageColumn, double x, double y, double width, double height, int Nx, const QString& title, QColor trueColor=QColor("black"), QColor falseColor=QColor("transparent"))=0;
        /** \brief add a range graph
         */
        virtual int colgraphAddRangeGraph(int plot, Orientation orientation, double rangeStart, double rangeEnd, double rangeCenter, const QString& title, bool invertRange=false, bool fillrange=true, bool drawRangeLines=true, bool drawRangeCenter=true, QColor centerColor=QColor("red"),  Qt::PenStyle centerStyle=Qt::SolidLine, double centerWidth=1.0)=0;
        /** \brief add an RGB image graph which
         */
        virtual int colgraphAddRGBImageGrph(int plot, int imageRColumn, int imageGColumn, int imageBColumn, double x, double y, double width, double height, int Nx, const QString& title)=0;
        /** \brief set the modifier channel of the given image graph
         */
        virtual void colgraphSetImageGraphModifier(int plot, int graph, int imageModifierColumn, ImageModifierMode mode)=0;
        /** \brief set the modifier channel of the given image graph
         */
        virtual void colgraphSetImageGraphRange(int plot, int graph, ImageGraphRangeChannel channel, bool autoRange, double min=0, double max=0)=0;

        /** \brief set a named property in the given graph
         */
        virtual void colgraphSetGraphProperty(int plot, int graph, const QString& name, const QVariant& value)=0;
        /** \brief get a named property in the given graph
         */
        virtual QVariant colgraphGetGraphProperty(int plot, int graph, const QString& name, const QVariant& defaultValue=QVariant())=0;
        /** \brief set the color palette of a plot
         */
        virtual void colgraphSetColorPalette(int plot, int graph, ImageColorPalette palette)=0;


};

Q_DECLARE_INTERFACE( QFRDRColumnGraphsInterface,
                     "www.dkfz.de.b040.quickfit3.QFRDRColumnGraphsInterface/1.0")

#endif // QFRDRCOLUMNGRAPHINTERFACE_H
