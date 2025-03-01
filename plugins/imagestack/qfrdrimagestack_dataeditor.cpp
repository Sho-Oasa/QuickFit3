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


#include "qfrdrimagestack_dataeditor.h"
#include "qfrdrimagestack_data.h"
#include "qfrawdatapropertyeditor.h"

QFRDRImageStackDataEditor::QFRDRImageStackDataEditor(QFPluginServices* services, QFRawDataPropertyEditor *propEditor, QWidget *parent):
    QFRawDataEditor(services, propEditor, parent)
{
    updateImage=true;
    dataMode=QFRDRImageStackDataEditor::dmFullHistogram;
    maskTools=new QFRDRImageMaskEditTools(this, QString("imagestackeditor/"));
    selection=NULL;
    selectionWidth=0;
    selectionHeight=0;
    viewer3D=NULL;
    createWidgets();

}

QFRDRImageStackDataEditor::~QFRDRImageStackDataEditor()
{
    if (viewer3D) {
        viewer3D->close();
        delete viewer3D;
        viewer3D=NULL;
    }
    if (selection) qfFree(selection);
}

void QFRDRImageStackDataEditor::createWidgets() {
    QVBoxLayout* mainLay=new QVBoxLayout();
    setLayout(mainLay);

    QHBoxLayout* layTop=new QHBoxLayout();
    mainLay->addLayout(layTop);
    layTop->addWidget(new QLabel(tr("<b>image stack:</b>")));
    cmbImageStack=new QComboBox(this);
    layTop->addWidget(cmbImageStack);
    layTop->addStretch();




    cmbHistogramMode=new QComboBox(this);
    cmbHistogramMode->addItem(tr("current frame"));
    cmbHistogramMode->addItem(tr("full stack"));
    layTop->addWidget(new QLabel(tr("<b>histogram:</b> mode: ")));
    layTop->addWidget(cmbHistogramMode);
    cmbHistogramStyle=new QComboBox(this);
    cmbHistogramStyle->addItem(tr("bars"));
    cmbHistogramStyle->addItem(tr("curves"));
    layTop->addWidget(new QLabel(tr("  style: ")));
    layTop->addWidget(cmbHistogramStyle);
    spinBins=new QSpinBox(this);
    spinBins->setRange(10,100000);
    spinBins->setValue(256);
    layTop->addWidget(new QLabel(tr("bins: ")));
    layTop->addWidget(spinBins);
    chkHistogramLog=new QCheckBox(tr("log-scale"), this);
    chkHistogramLog->setChecked(false);
    layTop->addSpacing(8);
    layTop->addWidget(chkHistogramLog);


    layTop=new QHBoxLayout();
    mainLay->addLayout(layTop);
    layTop->addWidget(new QLabel(tr("<b>channel mode:</b>")));
    cmbChannelMode=new QComboBox(this);
    cmbChannelMode->addItem(tr("single-channel"));
    cmbChannelMode->addItem(tr("4-channel RGBA"));
    cmbChannelMode->setCurrentIndex(0);
    layTop->addWidget(cmbChannelMode);
    layTop->addWidget(new QLabel(tr("<b>channels:</b>")));
    cmbChannelR=new QComboBox(this);
    layTop->addWidget(cmbChannelR);
    cmbChannelG=new QComboBox(this);
    layTop->addWidget(cmbChannelG);
    cmbChannelB=new QComboBox(this);
    layTop->addWidget(cmbChannelB);
    cmbChannelA=new QComboBox(this);
    layTop->addWidget(cmbChannelA);

    /*
        QComboBox* cmbChannelA;
        JKQTPImageModifierModeComboBox* cmbModifierMode;
        JKQTPMathImageColorPaletteComboBox* cmbColorbar;

      */
    layTop->addWidget(labModifierMode= new QLabel(tr("<b>A mode:</b>")));
    cmbModifierMode=new JKQTPImageModifierModeComboBox(this);
    layTop->addWidget(cmbModifierMode);
    layTop->addWidget(labColorbar= new QLabel(tr("<b>palette:</b>")));
    cmbColorbar=new JKQTPMathImageColorPaletteComboBox(this);
    layTop->addWidget(cmbColorbar);



    layTop->addStretch();

    layTop->addWidget(new QLabel(tr("<b>mask color:</b>")));
    cmbMaskColor=new ColorComboBox(this);
    layTop->addWidget(cmbMaskColor);
    layTop->addStretch();


    toolbar=new QToolBar("", this);
    mainLay->addWidget(toolbar);

    pltImage=new QFPlotter(this);
    pltImage->get_plotter()->set_userSettigsFilename(ProgramOptions::getInstance()->getIniFilename());
    pltImage->set_displayMousePosition(true);
    pltImage->set_displayToolbar(false);
    pltImage->get_plotter()->set_maintainAspectRatio(true);
    pltImage->get_plotter()->set_aspectRatio(1);
    pltImage->get_plotter()->set_maintainAxisAspectRatio(true);
    pltImage->get_plotter()->set_axisAspectRatio(1);
    pltImage->setXY(0,0,1,1);
    pltImage->setAbsoluteXY(0,1,0,1);
    pltImage->get_plotter()->getXAxis()->set_axisMinWidth(1);
    pltImage->get_plotter()->getYAxis()->set_axisMinWidth(1);
    pltImage->get_plotter()->getXAxis()->set_labelType(JKQTPCALTdefault);
    pltImage->get_plotter()->getYAxis()->set_labelType(JKQTPCALTdefault);
    pltImage->get_plotter()->getXAxis()->set_labelDigits(2);
    pltImage->get_plotter()->getYAxis()->set_labelDigits(2);
    pltImage->get_plotter()->getXAxis()->set_minTicks(5);
    //pltImage->get_plotter()->getXAxis()->addAxisTickLabel(0,"0");
    pltImage->get_plotter()->getYAxis()->set_minTicks(5);
    //pltImage->get_plotter()->getYAxis()->addAxisTickLabel(0,"0");

    pltImage->get_plotter()->getXAxis()->set_tickLabelFontSize(8);
    pltImage->get_plotter()->getYAxis()->set_tickLabelFontSize(8);
    pltImage->get_plotter()->getXAxis()->set_axisLabel("");
    pltImage->get_plotter()->getYAxis()->set_axisLabel("");
    pltImage->get_plotter()->setGrid(false);
    pltImage->get_plotter()->set_useAntiAliasingForSystem(true);
    pltImage->get_plotter()->set_useAntiAliasingForGraphs(true);
    pltImage->set_userActionCompositionMode(QPainter::CompositionMode_Xor);

    maskTools->registerPlotter(pltImage);


    QVBoxLayout* vblData=new QVBoxLayout();
    QWidget* widData=new QWidget(this);
    vblData->setContentsMargins(0,0,0,0);
    widData->setLayout(vblData);

    grpRanges=new QGroupBox(tr(" Channel Ranges "), widData);
    vblData->addWidget(grpRanges);
    QFormLayout* layRanges=new QFormLayout();
    grpRanges->setLayout(layRanges);

    cmbColorScaleMode=new QComboBox(this);
    cmbColorScaleMode->addItem(tr("autoscale each frame separately"));
    cmbColorScaleMode->addItem(tr("autoscale all frames"));
    cmbColorScaleMode->addItem(tr("manual scaling"));
    layRanges->addRow(tr("color-scaling mode:"), cmbColorScaleMode);
    cmbChannelGrouping=new QComboBox(this);
    cmbChannelGrouping->addItem(tr("channels separately"));
    cmbChannelGrouping->addItem(tr("all channels equal"));
    layRanges->addRow(tr("color-scaling grouping:"), cmbChannelGrouping);
    chkChannelRangeMindMask=new QCheckBox(grpRanges);
    chkChannelRangeMindMask->setChecked(true);
    layRanges->addRow(tr("exclude masked pixels:"), chkChannelRangeMindMask);
    for (int i=0; i<4; i++) {
        QString lab=tr("range ch. %1:").arg(i+1);
        edtChannelRange.append(new QFDoubleRangeEdit(grpRanges));
        edtChannelRangeLabels.append(new QLabel(lab, grpRanges));
        layRanges->addRow(edtChannelRangeLabels.last(), edtChannelRange.last());
        if (i>0) {
            edtChannelRange.last()->setEnabled(false);
            edtChannelRangeLabels.last()->setEnabled(false);
        }
    }




    pltData=new QFPlotter(widData);
    vblData->addWidget(pltData,1);
    pltData->get_plotter()->set_userSettigsFilename(ProgramOptions::getInstance()->getIniFilename());
    pltData->get_plotter()->getXAxis()->set_minTicks(5);
    pltData->get_plotter()->getYAxis()->set_minTicks(5);

    pltData->get_plotter()->getXAxis()->set_tickLabelFontSize(8);
    pltData->get_plotter()->getYAxis()->set_tickLabelFontSize(8);
    pltData->get_plotter()->getXAxis()->set_axisLabel("");
    pltData->get_plotter()->getYAxis()->set_axisLabel("");
    pltData->get_plotter()->setGrid(true);
    pltData->get_plotter()->set_useAntiAliasingForSystem(true);
    pltData->get_plotter()->set_useAntiAliasingForGraphs(true);
    pltData->set_userActionCompositionMode(QPainter::CompositionMode_Xor);



    splitter=new QVisibleHandleSplitter(Qt::Horizontal, this);
    splitter->addWidget(pltImage);
    splitter->addWidget(widData);
    mainLay->addWidget(splitter, 1);
    QList<int> sizes;
    sizes<<round(splitter->width()*2.0/3.0)<<round(splitter->width()/3.0);
    splitter->setSizes(sizes);

    QGridLayout* layLabels=new QGridLayout();

    labSelectiondata=new QLabel(this);
    layLabels->addWidget(labSelectiondata, 0, 1, 3,1);
    layLabels->setColumnStretch(0,1);
    layLabels->setColumnStretch(1,3);

    labDescription=new QLabel(this);
    layLabels->addWidget(labDescription, 0,0);
    labFrame=new QLabel(this);
    layLabels->addWidget(labFrame,1,0);
    mainLay->addLayout(layLabels);
    player=new QFPLayerControls(this);
    player->setVisible(false);
    mainLay->addWidget(labTAxis=new QLabel(tr("<b>time/z axis:</b>")));
    mainLay->addWidget(player);
    mainLay->addStretch();


    image=new JKQTPMathImage(0,0,1,1,JKQTPMathImageBase::DoubleArray, NULL, 0, 0, JKQTPMathImageGRAY, pltImage->get_plotter());
    image->get_colorBarRightAxis()->set_labelType(JKQTPCALTdefault);
    image->get_colorBarRightAxis()->set_labelDigits(2);
    image->get_colorBarRightAxis()->set_minTicks(3);
    //pltImage->addGraph(image);

    imageRGB=new JKQTPRGBMathImage(0,0,1,1,JKQTPMathImageBase::DoubleArray, NULL, 0, 0, pltImage->get_plotter());
    imageRGB->set_datatypeG(JKQTPMathImageBase::DoubleArray);
    imageRGB->set_datatypeB(JKQTPMathImageBase::DoubleArray);
    imageRGB->get_colorBarRightAxis()->set_labelType(JKQTPCALTdefault);
    imageRGB->get_colorBarRightAxis()->set_labelDigits(2);
    imageRGB->get_colorBarRightAxis()->set_minTicks(3);

    QColor ovlExCol=QColor("black");
    //ovlExCol.setAlphaF(0.5);
    QColor ovlSelCol=QColor("red");
    ovlSelCol.setAlphaF(0.5);
    plteOverviewExcluded=new JKQTPOverlayImageEnhanced(0,0,1,1,NULL, 0, 0, ovlExCol, pltImage->get_plotter());
    plteOverviewExcluded->set_rectanglesAsImageOverlay(true);
    plteOverviewSelected=new JKQTPOverlayImageEnhanced(0,0,1,1,NULL, 0, 0, ovlSelCol, pltImage->get_plotter());
    plteOverviewSelected->set_rectanglesAsImageOverlay(true);



    player->setSingleShot(true);

    toolbar->addAction(pltImage->get_plotter()->get_actSavePlot());
    toolbar->addAction(pltImage->get_plotter()->get_actPrint());
    toolbar->addAction(pltImage->get_plotter()->get_actCopyPixelImage());
    toolbar->addSeparator();
    toolbar->addAction(pltImage->get_plotter()->get_actZoomAll());
    toolbar->addAction(pltImage->get_plotter()->get_actZoomIn());
    toolbar->addAction(pltImage->get_plotter()->get_actZoomOut());
    toolbar->addSeparator();
    maskTools->registerMaskToolsToToolbar(toolbar);
    toolbar->addSeparator();
    maskTools->registerPlotterMaskToolsToToolbar(toolbar);

    act3DViewer=new QFActionWithNoMenuRole(QIcon(":/image_stack/viewer3d.png"), tr("&3D viewer"), this);
    connect(act3DViewer, SIGNAL(triggered()), this, SLOT(show3DViewer()));


    menuMask=propertyEditor->addMenu("&Mask", 0);
    menuTools=propertyEditor->addMenu("Stack &Tools", -1);
    menuTools->addAction(act3DViewer);


    maskTools->registerMaskToolsToMenu(menuMask);

};

void QFRDRImageStackDataEditor::connectWidgets(QFRawDataRecord* current, QFRawDataRecord* old) {
    updateImage=false;
    disconnect(cmbImageStack, SIGNAL(currentIndexChanged(int)), this, SLOT(stackChanged()));
    disconnect(player, SIGNAL(showFrame(int)), this, SLOT(showFrame(int)));
    disconnect(cmbChannelMode, SIGNAL(currentIndexChanged(int)), this, SLOT(channelModeChanged()));
    //disconnect(pltImage, SIGNAL(plotMouseClicked(double,double,Qt::KeyboardModifiers,Qt::MouseButton)), this, SLOT(plotMouseClicked(double,double,Qt::KeyboardModifiers,Qt::MouseButton)));

    if (old) {
        this->current=NULL;
        disconnect(old, SIGNAL(rawDataChanged()), this, SLOT(rawDataChanged()));
    }
    maskTools->setRDR(current);
    QFRDRImageStackData* m=qobject_cast<QFRDRImageStackData*>(current);
    cmbImageStack->clear();
    this->current=current;
    connect(current, SIGNAL(rawDataChanged()), this, SLOT(rawDataChanged()));
    if (m) {
        for (int i=0; i<m->getImageStackCount(); i++) {
            QString n=m->getImageStackDescription(i);
            if (n.isEmpty()) n=tr("stack %1").arg(i+1);
            cmbImageStack->addItem(QIcon(":/image_stack/image.png"), n);
        }
        maskTools->clearSelection();
    }


    cmbImageStack->setCurrentIndex(current->getProperty("imstack_invrimgdisp_image", 0).toInt());

    dataMode=QFRDRImageStackDataEditor::dmFullHistogram;

    player->setPosition(current->getProperty("imstack_invrimgdisp_playpos", 0).toInt());
    cmbMaskColor->setCurrentColor(QStringToQColor(current->getProperty("imstack_invrimgdisp_maskcolor", "gray").toString()));
    cmbChannelGrouping->setCurrentIndex(current->getProperty("imstack_channel_grouping", 0).toInt());
    cmbChannelMode->setCurrentIndex(current->getProperty("imstack_channel_mode", 0).toInt());
    connect(cmbImageStack, SIGNAL(currentIndexChanged(int)), this, SLOT(stackChanged()));
    connect(player, SIGNAL(showFrame(int)), this, SLOT(showFrame(int)));
    connect(cmbChannelMode, SIGNAL(currentIndexChanged(int)), this, SLOT(channelModeChanged()));
    //connect(pltImage, SIGNAL(plotMouseClicked(double,double,Qt::KeyboardModifiers,Qt::MouseButton)), this, SLOT(plotMouseClicked(double,double,Qt::KeyboardModifiers,Qt::MouseButton)));
    channelModeChanged();
    updateImage=true;
    stackChanged();
    if (viewer3D) {
        viewer3D->init(m, cmbImageStack->currentIndex());
    }
};

void QFRDRImageStackDataEditor::rawDataChanged() {
	// emitted when the raw data changes 
	QFRDRImageStackData* m=qobject_cast<QFRDRImageStackData*>(current);
    //labTest->setText(tr("loaded record with files<br><i>%1</i>").arg(m->getFiles().join("<br>")));
    displayImage();
    //qDebug()<<"QFRDRImageStackDataEditor::rawDataChanged()";
};

void QFRDRImageStackDataEditor::readSettings() {
	// read widget settings
    if (!settings) return;
    //qDebug()<<"readSettings()";
    QString prefix="image_stack/";
    bool upd=updatesEnabled();
    setUpdatesEnabled(false);
    player->setFPS(settings->getQSettings()->value(prefix+"player_fps", player->getFPS()).toDouble());
    player->setReplay(settings->getQSettings()->value(prefix+"player_replay", player->getReplay()).toBool());
    cmbChannelMode->setCurrentIndex(settings->getQSettings()->value(prefix+"imagemode", 0).toInt());
    cmbColorbar->setCurrentIndex(settings->getQSettings()->value(prefix+"colorbar", JKQTPMathImageGRAY).toInt());
    cmbModifierMode->setCurrentIndex(settings->getQSettings()->value(prefix+"modifiermode", JKQTPMathImageBase::ModifyAlpha).toInt());
    spinBins->setValue(settings->getQSettings()->value(prefix+"bins", 100).toInt());
    cmbHistogramMode->setCurrentIndex(settings->getQSettings()->value(prefix+"cmbHitogramMode", 0).toInt());
    cmbHistogramStyle->setCurrentIndex(settings->getQSettings()->value(prefix+"cmbHistogramStyle", 0).toInt());
    cmbChannelGrouping->setCurrentIndex(settings->getQSettings()->value(prefix+"cmbChannelGrouping", 0).toInt());
    chkHistogramLog->setChecked(settings->getQSettings()->value(prefix+"chkHistogramLog", false).toBool());
    chkChannelRangeMindMask->setChecked(settings->getQSettings()->value(prefix+"chkChannelRangeMindMask", true).toBool());
    // chkHistogramLog cmbHitogramMode
    loadSplitter(*(settings->getQSettings()), splitter, prefix);
    setUpdatesEnabled(upd);
    //qDebug()<<"readSettings(): done";
}


void QFRDRImageStackDataEditor::writeSettings() {
	// rwrite widget settings
    if (!settings) return;
    QString prefix="image_stack/";
    settings->getQSettings()->setValue(prefix+"player_fps", player->getFPS());
    settings->getQSettings()->setValue(prefix+"player_replay", player->getReplay());
    settings->getQSettings()->setValue(prefix+"imagemode", cmbChannelMode->currentIndex());
    settings->getQSettings()->setValue(prefix+"colorbar", cmbColorbar->currentIndex());
    settings->getQSettings()->setValue(prefix+"modifiermode", cmbModifierMode->currentIndex());
    settings->getQSettings()->setValue(prefix+"bins", spinBins->value());
    settings->getQSettings()->setValue(prefix+"chkHistogramLog", chkHistogramLog->isChecked());
    settings->getQSettings()->setValue(prefix+"chkChannelRangeMindMask", chkChannelRangeMindMask->isChecked());
    settings->getQSettings()->setValue(prefix+"cmbHitogramMode", cmbHistogramMode->currentIndex());
    settings->getQSettings()->setValue(prefix+"cmbHistogramStyle", cmbHistogramStyle->currentIndex());
    settings->getQSettings()->setValue(prefix+"cmbChannelGrouping", cmbChannelGrouping->currentIndex());


    saveSplitter(*(settings->getQSettings()), splitter, prefix);
}

void QFRDRImageStackDataEditor::addDataHistogram(double *data, bool* mask, int size, int maskSize, double dmin, double dmax, const QString &title, const QString &colX, const QString &colY, QColor col, double shift, double width) {
    JKQTPdatastore* dsData=pltData->getDatastore();

    double* histX=(double*)qfCalloc(spinBins->value(), sizeof(double));
    double* histY=(double*)qfCalloc(spinBins->value(), sizeof(double));




    if (dmax>dmin) {
        if (mask) {
            statisticsHistogramModMaskedRanged(data, mask, size, maskSize, dmin, dmax, histX, histY, spinBins->value(), false);
        } else statisticsHistogramRanged(data, size, dmin, dmax, histX, histY, spinBins->value(), false);
    } else {
        if (mask) {
            statisticsHistogramModMasked(data, mask, size, maskSize, histX, histY, spinBins->value(), false);
        } else statisticsHistogram(data, size, histX, histY, spinBins->value(), false);
    }


    size_t chx=dsData->addCopiedColumn(histX, spinBins->value(), colX);
    size_t chy=dsData->addCopiedColumn(histY, spinBins->value(), colY);

    if (cmbHistogramStyle->currentIndex()==0) {
        QColor colf=col;
        colf.setAlphaF(0.66);
        JKQTPbarHorizontalGraph* plteHistogram=new JKQTPbarHorizontalGraph(pltData->get_plotter());
        plteHistogram->set_title(title);
        plteHistogram->set_xColumn(chx);
        plteHistogram->set_yColumn(chy);
        plteHistogram->set_shift(shift);
        plteHistogram->set_width(width);
        plteHistogram->set_fillColor(colf);
        plteHistogram->set_color(col);
        pltData->addGraph(plteHistogram);
    } else {
        QColor colf=col;
        colf.setAlphaF(0.33);
        JKQTPfilledCurveXGraph* plteHistogram=new JKQTPfilledCurveXGraph(pltData->get_plotter());
        plteHistogram->set_title(title);
        plteHistogram->set_xColumn(chx);
        plteHistogram->set_yColumn(chy);
        plteHistogram->set_fillColor(colf);
        plteHistogram->set_color(col);
        plteHistogram->set_drawLine(true);
        pltData->addGraph(plteHistogram);
    }

    pltData->getYAxis()->set_logAxis(chkHistogramLog->isChecked());

    qfFree(histX);
    qfFree(histY);
}

void QFRDRImageStackDataEditor::connectWidgets()
{
    connect(cmbChannelR, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(cmbChannelG, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(cmbChannelB, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(cmbChannelA, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(cmbColorbar, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(cmbModifierMode, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(cmbMaskColor, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    connect(maskTools, SIGNAL(maskChanged()), this, SLOT(maskChanged()));
    connect(cmbHistogramMode, SIGNAL(currentIndexChanged(int)), this, SLOT(replotFrame()));
    connect(cmbHistogramStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(replotFrame()));
    connect(chkHistogramLog, SIGNAL(toggled(bool)), this, SLOT(replotFrame()));
    connect(spinBins, SIGNAL(editingFinished()), this, SLOT(replotFrame()));

    connect(cmbColorScaleMode, SIGNAL(currentIndexChanged(int)), this, SLOT(colorModeOrRangesChanged()));
    connect(cmbChannelGrouping, SIGNAL(currentIndexChanged(int)), this, SLOT(colorModeOrRangesChanged()));
    connect(chkChannelRangeMindMask, SIGNAL(toggled(bool)), this, SLOT(colorModeOrRangesChanged()));
    connectDisconnectChannelRanges(true);
}

void QFRDRImageStackDataEditor::disconnectWidgets()
{
    disconnect(cmbChannelR, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(cmbChannelG, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(cmbChannelB, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(cmbChannelA, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(cmbColorbar, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(cmbModifierMode, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(cmbMaskColor, SIGNAL(currentIndexChanged(int)), this, SLOT(displayImage()));
    disconnect(maskTools, SIGNAL(maskChanged()), this, SLOT(maskChanged()));
    disconnect(cmbHistogramMode, SIGNAL(currentIndexChanged(int)), this, SLOT(replotFrame()));
    disconnect(cmbHistogramStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(replotFrame()));
    disconnect(chkHistogramLog, SIGNAL(toggled(bool)), this, SLOT(replotFrame()));
    disconnect(spinBins, SIGNAL(editingFinished()), this, SLOT(replotFrame()));

    disconnect(cmbColorScaleMode, SIGNAL(currentIndexChanged(int)), this, SLOT(colorModeOrRangesChanged()));
    disconnect(cmbChannelGrouping, SIGNAL(currentIndexChanged(int)), this, SLOT(colorModeOrRangesChanged()));
    disconnect(chkChannelRangeMindMask, SIGNAL(toggled(bool)), this, SLOT(colorModeOrRangesChanged()));
    connectDisconnectChannelRanges(false);
}


void QFRDRImageStackDataEditor::showFrame(int frame, bool startPlayer) {
    if (!updateImage) return;
    //qDebug()<<"showFrame("<<frame<<startPlayer<<")";
    bool dd=pltImage->get_doDrawing();
    pltImage->set_doDrawing(false);
    bool ddd=pltData->get_doDrawing();
    pltData->set_doDrawing(false);
    current->setQFProperty("imstack_invrimgdisp_playpos", player->getPosition(), false, false);
    current->setQFProperty("imstack_invrimgdisp_maskcolor", cmbMaskColor->currentColor().name(), false, false);
    current->setQFProperty("imstack_channel_grouping", cmbChannelGrouping->currentIndex(), false, false);
    current->setQFProperty("imstack_channel_mode", cmbChannelMode->currentIndex(), false, false);

    pltImage->clearGraphs(false);
    pltData->clearGraphs(true);
    JKQTPdatastore* dsData=pltData->getDatastore();
    dsData->clear();
    QString selectiontextA="";
    QString selectiontextD="";

    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    if (mv && cmbImageStack->currentIndex()<mv->getImageStackCount()) {
        updateChannelRanges();
        int idx=cmbImageStack->currentIndex();
        int width=mv->getImageStackWidth(idx);
        int height=mv->getImageStackHeight(idx);
        int frames=mv->getImageStackFrames(idx);
        double rwidth=(double)width*mv->getImageStackXUnitFactor(idx);
        double rheight=(double)height*mv->getImageStackYUnitFactor(idx);

        int channelR=cmbChannelR->currentIndex();
        int channelG=cmbChannelG->currentIndex();
        int channelB=cmbChannelB->currentIndex();
        int channelA=cmbChannelA->currentIndex();
        double* ar=NULL;
        double amin=0;
        double amax=0;
        double cmin=0;
        double cmax=0;
        double gmin=0, gmax=0;
        double bmin=0, bmax=0;
        if (channelA>=0 && channelA<mv->getImageStackChannels(idx)) ar=mv->getImageStack(idx, frame, channelA);
//        if (ar && mv->maskGet()) {
//            if (cmbColorScaleMode->currentIndex()==0) statisticsMaskedMinMax(ar, mv->maskGet(), width*height, amin, amax, false);
//            if (cmbColorScaleMode->currentIndex()==1) {
//                amin=mv->getImageStackMin(idx, channelA);
//                amax=mv->getImageStackMax(idx, channelA);
//            }
//        }
        if (channelA>=0 && channelA<edtChannelRange.size()) {
            amin=edtChannelRange[channelA]->getMin();
            amax=edtChannelRange[channelA]->getMax();
        }


        if (!selection || selectionWidth!=width || selectionHeight!=height) {
            selection=(bool*)qfRealloc(selection, width*height*sizeof(bool));
            selectionWidth=width;
            selectionHeight=height;
            if (selection) for (int i=0; i<width*height; i++) selection[i]=false;
        }

        plteOverviewSelected->set_data(selection, width, height);
        plteOverviewSelected->set_width(width);
        plteOverviewSelected->set_height(height);
        maskTools->setAllowEditSelection(selection&&(selectionWidth>0)&&(selectionHeight>0), selection, selectionWidth, selectionHeight);
        maskTools->setMaskEditing(true);
        maskTools->setEditingMode(0);
        if (selection&&ar) {
            double var=0, vara=0;
            double avg=statisticsAverageVarianceMasked(var, selection, ar, width*height, true);
            double avga=statisticsAverageVariance(vara, ar, width*height);
            selectiontextA=tr("<b>A <small>[sel/all]</small>:</b> (%3&plusmn;%4) / (%1&plusmn;%2)").arg(avga).arg(sqrt(vara)).arg(avg).arg(sqrt(var));
        } else if (ar) {
            double vara=0;
            double avga=statisticsAverageVariance(vara, ar, width*height);
            selectiontextA=tr("<b>A <small>[all]</small>:</b> (%1&plusmn;%2)").arg(avga).arg(sqrt(vara));
        }

        if (cmbChannelMode->currentIndex()==0) {
            double* ir=NULL;

            if (channelR>=0 && channelR<mv->getImageStackChannels(idx)) ir=mv->getImageStack(idx, frame, channelR);
            image->set_width(width);
            image->set_height(height);
            //qDebug()<<"frame="<<frame<<"   "<<ir;

            image->set_data(ir, width, height, JKQTPMathImageBase::DoubleArray);
            image->set_dataModifier(ar, JKQTPMathImageBase::DoubleArray);
            image->set_modifierMode(cmbModifierMode->getModifierMode());
            image->set_palette(cmbColorbar->colorPalette());

            if (selection&&ir) {
                double var=0, vara=0;
                double avg=statisticsAverageVarianceMasked(var, selection, ir, width*height, true);
                double avga=statisticsAverageVariance(vara, ir, width*height);
                selectiontextD=tr("<b>D <small>[sel/all]</small>:</b> (%3&plusmn;%4) / (%1&plusmn;%2)").arg(avga).arg(sqrt(vara)).arg(avg).arg(sqrt(var));
            } else if (ir) {
                double vara=0;
                double avga=statisticsAverageVariance(vara, ir, width*height);
                selectiontextD=tr("<b>D <small>[all]</small>:</b> (%1&plusmn;%2)").arg(avga).arg(sqrt(vara));
            }
            if (mv->maskGet()) {
//                if (cmbColorScaleMode->currentIndex()==0) statisticsMaskedMinMax(ir, mv->maskGet(), width*height, cmin, cmax, false);
//                if (cmbColorScaleMode->currentIndex()==1) {
//                    cmin=mv->getImageStackMin(idx, channel);
//                    cmax=mv->getImageStackMax(idx, channel);
//                }
                if (channelR>=0 && channelR<edtChannelRange.size()) {
                    cmin=edtChannelRange[channelR]->getMin();
                    cmax=edtChannelRange[channelR]->getMax();
                }

                image->set_imageMin(cmin);
                image->set_imageMax(cmax);
                image->set_autoImageRange(false);
                image->set_modifierMin(amin);
                image->set_modifierMax(amax);
                image->set_autoModifierRange(false);
            } else {
                image->set_autoModifierRange(true);
            }

            pltImage->addGraph(image);


            if (dataMode==QFRDRImageStackDataEditor::dmFullHistogram && ir) {
                if (cmbHistogramMode->currentIndex()==0)  addDataHistogram(ir, mv->maskGet(), width*height, width*height, tr("frame histogram"), tr("histogram_x"), tr("histogram_y"));
                else if (cmbHistogramMode->currentIndex()==1)  addDataHistogram(mv->getImageStack(idx, 0, channelR), mv->maskGet(), width*height*frames, width*height, tr("full histogram"), tr("histogram_x"), tr("histogram_y"));
            }
        } else {
            double* ir=NULL;
            double* ig=NULL;
            double* ib=NULL;


            if (channelR>=0 && channelR<mv->getImageStackChannels(idx)) ir=mv->getImageStack(idx, frame, channelR);
            if (channelG>=0 && channelG<mv->getImageStackChannels(idx)) ig=mv->getImageStack(idx, frame, channelG);
            if (channelB>=0 && channelB<mv->getImageStackChannels(idx)) ib=mv->getImageStack(idx, frame, channelB);

            if (ir && channelR>=0 && channelR<edtChannelRange.size()) {
                cmin=edtChannelRange[channelR]->getMin();
                cmax=edtChannelRange[channelR]->getMax();
            }
            if (ig && channelG>=0 && channelG<edtChannelRange.size()) {
                gmin=edtChannelRange[channelG]->getMin();
                gmax=edtChannelRange[channelG]->getMax();
            }
            if (ib && channelB>=0 && channelB<edtChannelRange.size()) {
                bmin=edtChannelRange[channelB]->getMin();
                bmax=edtChannelRange[channelB]->getMax();
            }

            //qDebug()<<"frame="<<frame<<"   "<<ir<<ig<<ib;
            imageRGB->set_data(ir, ig, ib, width, height, JKQTPMathImageBase::DoubleArray);
            imageRGB->set_width(width);
            imageRGB->set_height(height);
            imageRGB->set_dataModifier(ar, JKQTPMathImageBase::DoubleArray);
            imageRGB->set_modifierMode(cmbModifierMode->getModifierMode());

            if (selection&&ir) {
                double var=0, vara=0;
                double avg=statisticsAverageVarianceMasked(var, selection, ir, width*height, true);
                double avga=statisticsAverageVariance(vara, ir, width*height);
                selectiontextD=tr("<b>R <small>[sel/all]</small>:</b> (%3&plusmn;%4) / (%1&plusmn;%2)").arg(avga).arg(sqrt(vara)).arg(avg).arg(sqrt(var));
            } else if (ir) {
                double vara=0;
                double avga=statisticsAverageVariance(vara, ir, width*height);
                selectiontextD=tr("<b>R <small>[all]</small>:</b> (%1&plusmn;%2)").arg(avga).arg(sqrt(vara));
            }
            if (!selectiontextD.isEmpty()) selectiontextD+="<br>";
            if (selection&&ig) {
                double var=0, vara=0;
                double avg=statisticsAverageVarianceMasked(var, selection, ig, width*height, true);
                double avga=statisticsAverageVariance(vara, ig, width*height);
                selectiontextD+=tr("<b>G <small>[sel/all]</small>:</b> (%3&plusmn;%4) / (%1&plusmn;%2)").arg(avga).arg(sqrt(vara)).arg(avg).arg(sqrt(var));
            } else if (ig) {
                double vara=0;
                double avga=statisticsAverageVariance(vara, ig, width*height);
                selectiontextD+=tr("<b>G <small>[all]</small>:</b> (%1&plusmn;%2)").arg(avga).arg(sqrt(vara));
            }
            if (!selectiontextD.isEmpty()) selectiontextD+="<br>";
            if (selection&&ib) {
                double var=0, vara=0;
                double avg=statisticsAverageVarianceMasked(var, selection, ib, width*height, true);
                double avga=statisticsAverageVariance(vara, ib, width*height);
                selectiontextD+=tr("<b>B <small>[sel/all]</small>:</b> (%3&plusmn;%4) / (%1&plusmn;%2)").arg(avga).arg(sqrt(vara)).arg(avg).arg(sqrt(var));
            } else if (ib) {
                double vara=0;
                double avga=statisticsAverageVariance(vara, ib, width*height);
                selectiontextD+=tr("<b>B <small>[all]</small>:</b> (%1&plusmn;%2)").arg(avga).arg(sqrt(vara));
            }

//            if (mv->maskGet()) {
                if (ir) {
//                    if (cmbColorScaleMode->currentIndex()==0) statisticsMaskedMinMax(ir, mv->maskGet(), width*height, cmin, cmax, false);
//                    if (cmbColorScaleMode->currentIndex()==1) {
//                        cmin=mv->getImageStackMin(idx, channel);
//                        cmax=mv->getImageStackMax(idx, channel);
//                    }

                    imageRGB->set_imageMin(cmin);
                    imageRGB->set_imageMax(cmax);
                }

                if (ig) {
//                    cmin=cmax=0;
//                    if (cmbColorScaleMode->currentIndex()==0) statisticsMaskedMinMax(ig, mv->maskGet(), width*height, cmin, cmax, false);
//                    if (cmbColorScaleMode->currentIndex()==1) {
//                        cmin=mv->getImageStackMin(idx, channelG);
//                        cmax=mv->getImageStackMax(idx, channelG);
//                    }
                    imageRGB->set_imageMinG(gmin);
                    imageRGB->set_imageMaxG(gmax);
                }

                if (ib) {
//                    cmin=cmax=0;
//                    if (cmbColorScaleMode->currentIndex()==0) statisticsMaskedMinMax(ib, mv->maskGet(), width*height, cmin, cmax, false);
//                    if (cmbColorScaleMode->currentIndex()==1) {
//                        cmin=mv->getImageStackMin(idx, channelB);
//                        cmax=mv->getImageStackMax(idx, channelB);
//                    }
                    imageRGB->set_imageMinB(bmin);
                    imageRGB->set_imageMaxB(bmax);
                }

                imageRGB->set_autoImageRange(false);
                image->set_modifierMin(amin);
                image->set_modifierMax(amax);
                image->set_autoModifierRange(false);

//            } else {
//                imageRGB->set_autoImageRange(true);
//                image->set_autoModifierRange(true);
//            }


            pltImage->addGraph(imageRGB);
            if (dataMode==QFRDRImageStackDataEditor::dmFullHistogram) {
                if (cmbHistogramMode->currentIndex()==0)  {
                    if (ir) addDataHistogram(ir, mv->maskGet(), width*height, width*height, tr("red frame histogram"), tr("histogram_red_x"), tr("histogram_red_y"), QColor("red"), 0, 0.3);
                    if (ig) addDataHistogram(ig, mv->maskGet(), width*height, width*height, tr("green frame histogram"), tr("histogram_green_x"), tr("histogram_green_y"), QColor("green"),0.33, 0.3);
                    if (ib) addDataHistogram(ib, mv->maskGet(), width*height, width*height, tr("blue frame histogram"), tr("histogram_blue_x"), tr("histogram_blue_y"), QColor("blue"), 0.66, 0.3);

                } else if (cmbHistogramMode->currentIndex()==1) {
                    if (ir && channelR>=0) addDataHistogram(mv->getImageStack(idx, 0, channelR), mv->maskGet(), width*height*frames, width*height, tr("red full histogram"), tr("histogram_red_x"), tr("histogram_red_y"), QColor("red"), 0, 0.3);
                    if (ig && channelG>=0 ) addDataHistogram(mv->getImageStack(idx, 0, channelG), mv->maskGet(), width*height*frames, width*height, tr("green full histogram"), tr("histogram_green_x"), tr("histogram_green_y"), QColor("green"),0.33, 0.3);
                    if (ib && channelB>=0) addDataHistogram(mv->getImageStack(idx, 0, channelB), mv->maskGet(), width*height*frames, width*height, tr("blue full histogram"), tr("histogram_blue_x"), tr("histogram_blue_y"), QColor("blue"), 0.66, 0.3);
                }
            }

        }

        if (dataMode==QFRDRImageStackDataEditor::dmFullHistogram) {
            if (cmbHistogramMode->currentIndex()==0)  {
                if (ar) addDataHistogram(ar, mv->maskGet(), width*height, width*height, tr("alpha frame histogram"), tr("histogram_red_x"), tr("histogram_red_y"), QColor("gray"), 0, 0.3);

            } else if (cmbHistogramMode->currentIndex()==1) {
                if (ar && channelA>=0) addDataHistogram(mv->getImageStack(idx, 0, channelA), mv->maskGet(), width*height*frames, width*height, tr("alpha full histogram"), tr("histogram_red_x"), tr("histogram_red_y"), QColor("gray"), 0, 0.3);
            }

         }
        plteOverviewExcluded->set_data(mv->maskGet(), mv->maskGetWidth(), mv->maskGetHeight());
        plteOverviewExcluded->set_width(width);
        plteOverviewExcluded->set_height(height);
        plteOverviewExcluded->set_trueColor(cmbMaskColor->currentColor());
        pltImage->addGraph(plteOverviewExcluded);
        pltImage->addGraph(plteOverviewSelected);



        emit displayedFrame((double)frame*mv->getImageStackTUnitFactor(idx));
        labFrame->setText(tr("<b>frame:</b> %1/%2 (%3 %4), masked: %5").arg(frame+1).arg(mv->getImageStackFrames(idx)).arg(frame*mv->getImageStackTUnitFactor(idx)).arg(mv->getImageStackTUnitName(idx)).arg(mv->maskGetCount()));
    }
    QString text="";
    if (!selectiontextA.isEmpty()) text=selectiontextA;
    if (!selectiontextD.isEmpty()) {
        if (!text.isEmpty()) text+="<br>";
        text+=selectiontextD;
    }
    labSelectiondata->setText(text);



    if (dd) {
        pltImage->update_plot();
        pltImage->set_doDrawing(dd);
        pltImage->update_plot();
        if (startPlayer) player->singleShotTimerStart();
    }
    if (ddd) {
        pltData->zoomToFit();
        pltData->set_doDrawing(ddd);
        pltData->update_plot();
    }
    //qDebug()<<"showFrame("<<frame<<startPlayer<<"): done";
}


void QFRDRImageStackDataEditor::colorModeOrRangesChanged()
{
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    int idx=cmbImageStack->currentIndex();

    // save color ranges in manual mode:
    if (mv && cmbColorScaleMode->currentIndex()==2) {
        for (int i=0; i<edtChannelRange.size(); i++) {
            if (idx<mv->getImageStackCount() && i<mv->getImageStackChannels(idx)) {
                mv->setQFProperty(QString("imstack_manualcolscaling_ch%1_min").arg(i), edtChannelRange[i]->getMin(), false, false);
                mv->setQFProperty(QString("imstack_manualcolscaling_ch%1_max").arg(i), edtChannelRange[i]->getMax(), false, false);
            }
        }
    }
    updateChannelRanges();
    replotFrame();
}

void QFRDRImageStackDataEditor::connectDisconnectChannelRanges(bool conn)
{
    edtChannelRangesConnected=conn;
    for (int i=0; i<edtChannelRange.size(); i++) {
        disconnect(edtChannelRange[i], SIGNAL(valueChanged(double,double)), this, SLOT(colorModeOrRangesChanged()));
    }
    if (conn) {
        QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
        int idx=cmbImageStack->currentIndex();
        if (cmbColorScaleMode->currentIndex()==0) { // single-frame auto
            // all disconnected
        } else if (cmbColorScaleMode->currentIndex()==1) { // all frames auto
            // all disconnected
        } else if (cmbColorScaleMode->currentIndex()==2) { // manual
            if (cmbChannelGrouping->currentIndex()==0) {// manual, channels separately
                for (int i=0; i<edtChannelRange.size(); i++) {
                    if (!mv || (mv && i<mv->getImageStackChannels(idx))) {
                        connect(edtChannelRange[i], SIGNAL(valueChanged(double,double)), this, SLOT(colorModeOrRangesChanged()));
                    }
                }
            } else if (cmbChannelGrouping->currentIndex()==1) {
                for (int i=0; i<edtChannelRange.size(); i++) {// manual, channels together
                    if (i==0) {
                        connect(edtChannelRange[i], SIGNAL(valueChanged(double,double)), this, SLOT(colorModeOrRangesChanged()));
                    }
                }
            }
        }
    }
}

void QFRDRImageStackDataEditor::updateChannelRanges()
{
    bool oldconn=edtChannelRangesConnected;
    connectDisconnectChannelRanges(false);

    if (cmbChannelMode->currentIndex()==0) {
        for (int i=0; i<edtChannelRangeLabels.size(); i++) {
            if (cmbChannelR->currentIndex()==i) {
                edtChannelRangeLabels[i]->setText(tr("<font color=\"blue\"><b>range for channel %1 (data):</b></font>").arg(i+1));
            } else if (cmbChannelA->currentIndex()==i) {
                edtChannelRangeLabels[i]->setText(tr("<font color=\"gray\"><b>range for channel %1 (alpha):</b></font>").arg(i+1));
            } else {
                edtChannelRangeLabels[i]->setText(tr("range for channel %1:").arg(i+1));
            }
        }
    } else if (cmbChannelMode->currentIndex()==1) {
        for (int i=0; i<edtChannelRangeLabels.size(); i++) {
            if (cmbChannelR->currentIndex()==i) {
                edtChannelRangeLabels[i]->setText(tr("<font color=\"red\"><b>range for channel %1 (red):</b></font>").arg(i+1));
            } else if (cmbChannelG->currentIndex()==i) {
                edtChannelRangeLabels[i]->setText(tr("<font color=\"green\"><b>range for channel %1 (green):</b></font>").arg(i+1));
            } else if (cmbChannelB->currentIndex()==i) {
                edtChannelRangeLabels[i]->setText(tr("<font color=\"blue\"><b>range for channel %1 (blue):</b></font>").arg(i+1));
            } else if (cmbChannelA->currentIndex()==i) {
                edtChannelRangeLabels[i]->setText(tr("<font color=\"gray\"><b>range for channel %1 (alpha):</b></font>").arg(i+1));
            } else {
                edtChannelRangeLabels[i]->setText(tr("range for channel %1:").arg(i+1));
            }
        }
    }

    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    int idx=cmbImageStack->currentIndex();
    if (mv) {
        if (cmbColorScaleMode->currentIndex()==0) { // single-frame auto
            for (int i=0; i<edtChannelRange.size(); i++) {
                edtChannelRange[i]->setEnabled(false);
                edtChannelRangeLabels[i]->setEnabled(i<mv->getImageStackChannels(idx));
                if (idx<mv->getImageStackCount() && i<mv->getImageStackChannels(idx)) {
                    uint64_t size=0;
                    double* img=getImageStackAndSizeForColorRanging(size, idx, i,true);
                    double dmin=0, dmax=0;
                    if (chkChannelRangeMindMask->isChecked()) {
                        statisticsModMaskedMinMax(img, mv->maskGet(), size, mv->maskGetWidth()*mv->maskGetHeight(), dmin, dmax, false);
                    } else {
                        statisticsMinMax(img, size, dmin, dmax);
                    }
                    edtChannelRange[i]->setValues(dmin, dmax);
                } else {
                    edtChannelRange[i]->setValues(0,1);
                }
            }
        } else if (cmbColorScaleMode->currentIndex()==1) { // all frames auto
            for (int i=0; i<edtChannelRange.size(); i++) {
                edtChannelRange[i]->setEnabled(false);
                edtChannelRangeLabels[i]->setEnabled(i<mv->getImageStackChannels(idx));
                if (idx<mv->getImageStackCount() && i<mv->getImageStackChannels(idx)) {
                    uint64_t size=0;
                    double* img=getImageStackAndSizeForColorRanging(size, idx, i,false);
                    double dmin=0, dmax=0;
                    if (chkChannelRangeMindMask->isChecked()) {
                        statisticsModMaskedMinMax(img, mv->maskGet(), size, mv->maskGetWidth()*mv->maskGetHeight(), dmin, dmax, false);
                    } else {
                        statisticsMinMax(img, size, dmin, dmax);
                    }
                    edtChannelRange[i]->setValues(dmin, dmax);
                } else {
                    edtChannelRange[i]->setValues(0,1);
                }
            }
        } else if (cmbColorScaleMode->currentIndex()==2) { // manual
            if (cmbChannelGrouping->currentIndex()==0) {// manual, channels separately
                for (int i=0; i<edtChannelRange.size(); i++) {
                    if (idx<mv->getImageStackCount() && i<mv->getImageStackChannels(idx)) {
                        edtChannelRange[i]->setEnabled(true);
                        edtChannelRangeLabels[i]->setEnabled(true);
                        uint64_t size=0;
                        double dmin=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_min").arg(i), 0).toDouble();
                        double dmax=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_max").arg(i), 1).toDouble();
                        if (!mv->propertyExists(QString("imstack_manualcolscaling_ch%1_min").arg(i)) || !mv->propertyExists(QString("imstack_manualcolscaling_ch%1_max").arg(i))) {
                            double* img=getImageStackAndSizeForColorRanging(size, idx, i,false);
                            if (chkChannelRangeMindMask->isChecked()) {
                                statisticsModMaskedMinMax(img, mv->maskGet(), size, mv->maskGetWidth()*mv->maskGetHeight(), dmin, dmax, false);
                            } else {
                                statisticsMinMax(img, size, dmin, dmax);
                            }
                            if (mv->propertyExists(QString("imstack_manualcolscaling_ch%1_min").arg(i))) dmin=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_min").arg(i), 0).toDouble();
                            if (mv->propertyExists(QString("imstack_manualcolscaling_ch%1_max").arg(i))) dmax=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_max").arg(i), 1).toDouble();
                        }
                        edtChannelRange[i]->setValues(dmin, dmax);
                    } else {
                        edtChannelRange[i]->setEnabled(false);
                        edtChannelRangeLabels[i]->setEnabled(false);
                        edtChannelRange[i]->setValues(0,1);
                    }
                }
            } else if (cmbChannelGrouping->currentIndex()==1) {
                for (int i=0; i<edtChannelRange.size(); i++) {// manual, channels together
                    if (idx<mv->getImageStackCount() && i==0) {
                        edtChannelRange[i]->setEnabled(true);
                        edtChannelRangeLabels[i]->setEnabled(true);
                        uint64_t size=0;
                        double dmin=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_min").arg(i), 0).toDouble();
                        double dmax=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_max").arg(i), 1).toDouble();
                        if (!mv->propertyExists(QString("imstack_manualcolscaling_ch%1_min").arg(i)) || !mv->propertyExists(QString("imstack_manualcolscaling_ch%1_max").arg(i))) {
                            double* img=getImageStackAndSizeForColorRanging(size, idx, i,false);
                            if (chkChannelRangeMindMask->isChecked()) {
                                statisticsModMaskedMinMax(img, mv->maskGet(), size, mv->maskGetWidth()*mv->maskGetHeight(), dmin, dmax, false);
                            } else {
                                statisticsMinMax(img, size, dmin, dmax);
                            }
                            if (mv->propertyExists(QString("imstack_manualcolscaling_ch%1_min").arg(i))) dmin=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_min").arg(i), 0).toDouble();
                            if (mv->propertyExists(QString("imstack_manualcolscaling_ch%1_max").arg(i))) dmax=mv->getQFProperty(QString("imstack_manualcolscaling_ch%1_max").arg(i), 1).toDouble();
                        }
                        edtChannelRange[i]->setValues(dmin, dmax);
                    } else {
                        edtChannelRange[i]->setEnabled(false);
                        if (i<mv->getImageStackChannels(idx)) {
                            edtChannelRangeLabels[i]->setEnabled(true);
                            edtChannelRange[i]->setValues(edtChannelRange.first()->getMin(), edtChannelRange.first()->getMax());
                        } else {
                            edtChannelRangeLabels[i]->setEnabled(false);
                            edtChannelRange[i]->setValues(0,1);
                        }
                    }

                }
            }
        }
    }

    if (oldconn) {
        connectDisconnectChannelRanges(true);
    }
}

double *QFRDRImageStackDataEditor::getImageStackAndSizeForColorRanging(uint64_t &size, int stack, int channel, bool currentFrame)
{
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    int idx=cmbImageStack->currentIndex();
    if (stack<0) stack=idx;
    int f=0;
    if (currentFrame) {
        f=player->getPosition();
    }
    size=0;
    double* dat=NULL;
    if (mv && stack>=0 && channel>=0) {
        if (stack<mv->getImageStackCount() && channel<mv->getImageStackChannels(stack)) {
            dat=mv->getImageStack(stack,f,channel);
            if (currentFrame) {
                size=mv->getImageStackWidth(stack)*mv->getImageStackHeight(stack);
            } else {
                size=mv->getImageStackWidth(stack)*mv->getImageStackHeight(stack)*mv->getImageStackFrames(stack);
            }
        }
    }
    return dat;
}

void QFRDRImageStackDataEditor::replotFrame()
{
    showFrame(player->getPosition(), player->isPlaying());
}

void QFRDRImageStackDataEditor::maskChanged()
{
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    if (mv) {
        mv->recalcStackMinMax();
    }
    replotFrame();
}

void QFRDRImageStackDataEditor::displayImage() {
    if (!updateImage) return;
    if (!image) return;
    //qDebug()<<"displayImage()";
    player->pause();
    current->setQFProperty("imstack_invrimgdisp_image", cmbImageStack->currentIndex(), false, false);
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    player->setVisible(false);
    player->pause();
    bool dd=pltImage->get_doDrawing();
    pltImage->set_doDrawing(false);
    if (mv) {
        int idx=cmbImageStack->currentIndex();

        int width=mv->getImageStackWidth(idx);
        double rwidth=(double)width*mv->getImageStackXUnitFactor(idx);
        int height=mv->getImageStackHeight(idx);
        double rheight=(double)height*mv->getImageStackYUnitFactor(idx);
        int frames=mv->getImageStackFrames(idx);
        int channels=mv->getImageStackChannels(idx);

        player->setRange(0, frames-1);
        //player->setPosition(0);
        player->setSingleShot(true);
        player->setVisible(true);
        pltImage->get_plotter()->setAbsoluteXY(0,width,0,height);
        pltImage->get_plotter()->set_aspectRatio(double(width)/double(height));
        pltImage->get_plotter()->set_axisAspectRatio(double(width)/double(height));
        pltImage->get_plotter()->set_maintainAspectRatio(true);
        pltImage->get_plotter()->set_maintainAxisAspectRatio(true);
        //pltImage->get_xAxis()->set_axisLabel(QString("%1 [%2]").arg(mv->getImageStackXName(idx)).arg(mv->getImageStackXUnitName(idx)));
        //pltImage->get_yAxis()->set_axisLabel(QString("%1 [%2]").arg(mv->getImageStackYName(idx)).arg(mv->getImageStackYUnitName(idx)));
        pltImage->get_xAxis()->set_axisLabel(QString("%1 [pixel]").arg(mv->getImageStackXName(idx)));
        pltImage->get_yAxis()->set_axisLabel(QString("%1 [pixel]").arg(mv->getImageStackYName(idx)));
        labDescription->setText(tr("<b>image size:</b> %1 &times; %2 &times; %3 pixels, %4 channels").arg(width).arg(height).arg(frames).arg(channels));

        showFrame(player->getPosition(), false);
    } else  {
        image->set_data(NULL, 0, 0, JKQTPMathImageBase::UInt16Array);
        player->setVisible(false);
        player->setRange(0,100);
    }
    if (dd) {
        pltImage->set_doDrawing(true);
        //pltImage->zoomToFit();
        pltImage->update_plot();
    }
    //qDebug()<<"displayImage(): done";
}

void QFRDRImageStackDataEditor::stackChanged() {
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    int idx=cmbImageStack->currentIndex();
    disconnectWidgets();
    if (cmbChannelR->currentIndex()>=0) current->setQFProperty("imstack_invrimgdisp_channelr", cmbChannelR->currentIndex(), false, false);
    if (cmbChannelG->currentIndex()>=0) current->setQFProperty("imstack_invrimgdisp_channelg", cmbChannelG->currentIndex(), false, false);
    if (cmbChannelB->currentIndex()>=0) current->setQFProperty("imstack_invrimgdisp_channelb", cmbChannelB->currentIndex(), false, false);
    if (cmbChannelA->currentIndex()>=0) current->setQFProperty("imstack_invrimgdisp_channela", cmbChannelA->currentIndex(), false, false);
    cmbChannelR->clear();
    cmbChannelG->clear();
    cmbChannelB->clear();
    cmbChannelA->clear();
    if (mv) {
        for (int c=0; c<mv->getImageStackChannels(idx); c++) {
            cmbChannelR->addItem(mv->getImageStackChannelName(idx, c));
            cmbChannelG->addItem(mv->getImageStackChannelName(idx, c));
            cmbChannelB->addItem(mv->getImageStackChannelName(idx, c));
            cmbChannelA->addItem(mv->getImageStackChannelName(idx, c));
        }
        cmbChannelR->addItem(tr("--- none ---"));
        cmbChannelG->addItem(tr("--- none ---"));
        cmbChannelB->addItem(tr("--- none ---"));
        cmbChannelA->addItem(tr("--- none ---"));

        int frames=mv->getImageStackFrames(idx);
        player->setRange(0, frames-1);

    }
    cmbChannelR->setCurrentIndex(current->getProperty("imstack_invrimgdisp_channelr", 0).toInt());
    cmbChannelG->setCurrentIndex(current->getProperty("imstack_invrimgdisp_channelg", 1).toInt());
    cmbChannelB->setCurrentIndex(current->getProperty("imstack_invrimgdisp_channelb", 2).toInt());
    cmbChannelA->setCurrentIndex(current->getProperty("imstack_invrimgdisp_channela", 3).toInt());

    connectWidgets();
    displayImage();
    pltImage->zoomToFit();
    pltImage->update_plot();
}

void QFRDRImageStackDataEditor::channelModeChanged() {
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    int idx=cmbImageStack->currentIndex();
    disconnectWidgets();
    if (cmbChannelMode->currentIndex()>=0) current->setQFProperty("imstack_invrimgdisp_channelmode", cmbChannelMode->currentIndex(), false, false);

    cmbChannelR->setVisible(true);
    cmbChannelA->setVisible(true);
    cmbChannelG->setVisible(cmbChannelMode->currentIndex()==1);
    cmbChannelB->setVisible(cmbChannelMode->currentIndex()==1);
    cmbColorbar->setVisible(cmbChannelMode->currentIndex()==0);
    labColorbar->setVisible(cmbChannelMode->currentIndex()==0);
    cmbModifierMode->setVisible(true);
    labModifierMode->setVisible(true);

    connectWidgets();
    updateChannelRanges();
    displayImage();
}

void QFRDRImageStackDataEditor::show3DViewer()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QFRDRImageStackData* mv=qobject_cast<QFRDRImageStackData*>(current);
    int idx=cmbImageStack->currentIndex();
    if (!viewer3D) {
        viewer3D=new QFRDRImageStack3DViewer(NULL);
    }
    viewer3D->init(mv, idx);
    viewer3D->show();
    QApplication::restoreOverrideCursor();
}




