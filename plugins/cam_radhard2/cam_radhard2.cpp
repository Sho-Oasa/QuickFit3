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

#include "cam_radhard2.h"
#include<QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif


#include "qfplugin.h"
#include <iostream>
#include <unistd.h>
#include <time.h>

#define LOG_PREFIX "Radhard2:  "

QFExtensionCameraRadhard2::QFExtensionCameraRadhard2(QObject* parent):
    QObject(parent)
{
    conn=false;
    radhard2=NULL;
    subtractOne=true;
    iterations=100;
    divider=1;
    logService=NULL;
    edtBitfile=NULL;
    edtFlashSuccess=NULL;
    autoflashbitfile="";
    spIterations=NULL;
    spDivider=NULL;
    autoflash=true;
    autoflashbitfile=bitfile="";
    retries=10;
    retryDelay=1000;
}

QFExtensionCameraRadhard2::~QFExtensionCameraRadhard2() {

}


void QFExtensionCameraRadhard2::deinit() {
	/* add code for cleanup here */
}

void QFExtensionCameraRadhard2::projectChanged(QFProject* /*oldProject*/, QFProject* /*project*/) {
	/* usually cameras do not have to react to a change of the project in QuickFit .. so you don't need to do anything here
	   But: possibly you could read config information from the project here
	 */
}

void QFExtensionCameraRadhard2::initExtension() {
    services->log_global_text(tr("%2initializing extension '%1' ...\n").arg(getName()).arg(LOG_PREFIX));
    bitfile=autoflashbitfile=bitfile=services->getOptions()->getAssetsDirectory()+"/plugins/"+getID()+"/radhard2_top_cell.bit";
    actProgramFPGA=new QFActionWithNoMenuRole(QIcon(":/cam_radhard2_flash.png"), tr("Flash Radhard2 FPGA"), this);
    connect(actProgramFPGA, SIGNAL(triggered()), this, SLOT(programFPGA()));
    QMenu* extm=services->getMenu("extensions");
    if (extm) {
        QMenu* subMenu=extm->addMenu(QIcon(":/cam_radhard2_logo.png"), tr("Radhard 2 Tools"));
        subMenu->addAction(actProgramFPGA);
    }

    loadSettings(NULL);

    services->log_global_text(tr("%2initializing extension '%1' ... DONE\n").arg(getName()).arg(LOG_PREFIX));
}

void QFExtensionCameraRadhard2::programFPGA() {
    QFRadhard2Flashtool* dlg=new QFRadhard2Flashtool(this, NULL);
    loadSettings(NULL);

    dlg->setBitfile(bitfile);
    dlg->setAutoBitfile(autoflashbitfile);
    dlg->setAutoFlash(autoflash);
    dlg->setRetries(retries);
    dlg->setRetryDelayMS(retryDelay);

    dlg->exec();

    bitfile=dlg->bitfile();
    autoflashbitfile=dlg->autoBitfile();
    retries=dlg->retries();
    autoflash=dlg->autoflash();
    retryDelay=dlg->retryDelayMS();

    storeSettings(NULL);
    delete dlg;
    edtBitfile=NULL;
    edtFlashSuccess=NULL;
}


bool QFExtensionCameraRadhard2::flashFPGA(QString bitfile, char fpga, QString& messageOut, int retries, int retryDelayMS) {
    messageOut="";
    int res=0;
    int i=0;
    QString name="master";
    if (fpga=='S' || fpga=='s') name="slave";
    while ((i<retries) && (res==0)) {
        char message[8192];
        res=flash_bitfile(bitfile.toLatin1().data(), message, fpga);
        if (i>0) messageOut+="\n\n";
        messageOut += tr("try %4 %1/%2:\n%3").arg(i+1).arg(retries).arg(message).arg(name);
        i++;
        QTime time;
        time.start();
        while (time.elapsed()<retryDelayMS) {
            QApplication::processEvents();
        }
    }
    return res!=0;
}


void QFExtensionCameraRadhard2::loadSettings(ProgramOptions* /*settingspo*/) {
    QSettings settings(services->getConfigFileDirectory()+"/plugins/"+getID()+"/"+getID()+".ini", QSettings::IniFormat);
    autoflashbitfile=settings.value("radhard2/autobitfile", autoflashbitfile).toString();
    bitfile=settings.value("radhard2/bitfile", bitfile).toString();
    autoflash=settings.value("radhard2/autoflash", autoflash).toBool();
    retries=settings.value("radhard2/retries", retries).toInt();
    retryDelay=settings.value("radhard2/retryDelay", retryDelay).toInt();
}

void QFExtensionCameraRadhard2::storeSettings(ProgramOptions* /*settingspo*/) {
    QSettings settings(services->getConfigFileDirectory()+"/plugins/"+getID()+"/"+getID()+".ini", QSettings::IniFormat);
    settings.setValue("radhard2/autobitfile", autoflashbitfile);
    settings.setValue("radhard2/bitfile", bitfile);
    settings.setValue("radhard2/autoflash", autoflash);
    settings.setValue("radhard2/retries", retries);
    settings.setValue("radhard2/retryDelay", retryDelay);
}

unsigned int QFExtensionCameraRadhard2::getCameraCount() const {
    return 1;
}

void QFExtensionCameraRadhard2::useCameraSettings(unsigned int camera, const QSettings& settings) {
    iterations=settings.value("radhard2/iterations", 1000).toUInt();
    divider=settings.value("radhard2/divider", 1).toUInt();
    subtractOne=settings.value("radhard2/subtract1", true).toBool();
    if (isCameraConnected(camera)) {
        sendIterations();
        sendDivider();
     }
}

bool QFExtensionCameraRadhard2::prepareCameraAcquisition(unsigned int camera, const QSettings& settings, QString /*filenamePrefix*/) {
    useCameraSettings(camera, settings);
    return true;
}

void QFExtensionCameraRadhard2::showCameraSettingsDialog(unsigned int /*camera*/, QSettings& settings, QWidget* parent) {
	/* open a dialog that configures the camera.

	   usually you should display a modal QDialog descendent which writes back config when the user clicks OK

	   alternatively you may also display a window which stays open and allows the suer to set settings also
	   during the measurement.
	*/
    QFDialog* dlg=new QFDialog(parent);

    QVBoxLayout* lay=new QVBoxLayout();
    dlg->setLayout(lay);

    QFormLayout* formlayout=new QFormLayout();



    spIterations=new QSpinBox(dlg);
    spIterations->setMinimum(0);
    spIterations->setMaximum(100000);
    spIterations->setValue(settings.value("radhard2/iterations", 1000).toUInt());
    formlayout->addRow(tr("iterations:"), spIterations);
    connect(spIterations, SIGNAL(valueChanged(int)), this, SLOT(updateAcquisitionTime()));

    spDivider=new QSpinBox(dlg);
    spDivider->setMinimum(0);
    spDivider->setMaximum(100000);
    spDivider->setValue(settings.value("radhard2/divider", 1).toUInt());
    formlayout->addRow(tr("divider:"), spDivider);
    connect(spDivider, SIGNAL(valueChanged(int)), this, SLOT(updateAcquisitionTime()));

    QLabel* l=new QLabel(dlg);
    formlayout->addRow(tr("acquisition time:"), l);
    connect(this, SIGNAL(displayAcquisitionTime(const QString&)), l, SLOT(setText(const QString&)));

    QCheckBox* chkSubtract1=new QCheckBox("", dlg);
    chkSubtract1->setChecked(settings.value("radhard2/subtract1", true).toBool());
    formlayout->addRow(tr("subtract offset 1:"), chkSubtract1);


    lay->addLayout(formlayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, dlg);
    lay->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));

    if ( dlg->exec()==QDialog::Accepted ) {
         //  read back values entered into the widgets
         settings.setValue("radhard2/iterations", spIterations->value());
         settings.setValue("radhard2/divider", spDivider->value());
         settings.setValue("radhard2/subtract1", chkSubtract1->isChecked());

    }
    delete dlg;
    spIterations=NULL;
    spDivider=NULL;
}

int QFExtensionCameraRadhard2::getCameraImageWidth(unsigned int /*camera*/) {
    return 32;
}

int QFExtensionCameraRadhard2::getCameraImageHeight(unsigned int /*camera*/) {
    return 32;
}

bool QFExtensionCameraRadhard2::isCameraConnected(unsigned int /*camera*/) {
    return conn;
}

bool QFExtensionCameraRadhard2::acquireOnCamera(unsigned int camera, uint32_t* data, uint64_t* timestamp, QMap<QString, QVariant>* parameters) {
    sendIterations();
    sendDivider();

    if ((!conn) || (!radhard2)) {
        log_error(tr("not connected to hardware."));
        return false;
    }
    if (timestamp!=NULL) {
        *timestamp=(uint64_t)round(timer.get_time());
    }

    unsigned int result;
    int exposureT=getCameraExposureTime(camera);

	// Ask for the pictures
    radhard2->SendCommand(CMD_START_ACQUISITION);
    radhard2->ReadData(&result);
    if(result != OPERATION_DONE) {
		log_error(tr("Received wrong ACK constant after START_ACQUISITION (result was %1) command.\n").arg(8,16));
    }

    usleep(20000 + 2* int(exposureT));

	// Get the pictures
    radhard2->SendCommand(CMD_GET_MEMORY);
    radhard2->ReadImage(data);

    if (subtractOne) {
        register int imagesize=getCameraImageWidth(camera)*getCameraImageHeight(camera);
        for(register int j=0; j<imagesize; j++) {
            data[j] =  data[j]-1;
        }
    }

    if (parameters) {
        // TODO: fill in acquisition parameters
    }

    return true;
}

bool QFExtensionCameraRadhard2::connectCameraDevice(unsigned int /*camera*/) {
    conn=false;
    if (radhard2) delete radhard2;

    if (autoflash && QFile(autoflashbitfile).exists()) {
        log_text(tr("flashing Radhard2 FPGAs (bit file: %1)\n").arg(autoflashbitfile));
        QString flashMessage;
        bool ok=flashFPGA(autoflashbitfile, 'm', flashMessage, retries, retryDelay);
        flashMessage.replace('\n', QString("\n%1  ").arg(LOG_PREFIX));
        if (ok) {
            log_text(tr("  %2\nflashing Radhard2 FPGAs (bit file: %1) ... DONE!\n").arg(autoflashbitfile).arg(flashMessage));
        } else {
            log_error(tr("  %2\nflashing Radhard2 FPGAs (bit file: %1) ... ERROR!\n").arg(autoflashbitfile).arg(flashMessage));
            return false;
        }
    } else {
        if (autoflash) log_warning(tr("could not flash Radhard2 FPGAs, as bit file '%1' does not exist!\n").arg(autoflashbitfile));
    }

    radhard2=new Radhard2;
    radhard2->SendCommand(CMD_TEST_COMLINK, 0);
    unsigned int magicn=0;
    radhard2->ReadData(&magicn);
    log_text(tr("connection initialized\n%2read: MAGIC_NUMBER = %1\n").arg(QString::number(magicn, 16).toUpper()).arg(LOG_PREFIX));
    if (magicn!=MAGIC_NUMBER) {
        log_error(tr("initialization failed (wrong magic number). Expected: MAGIC_NUMBER = %1\n").arg(QString::number(MAGIC_NUMBER, 16).toUpper()));
        return false;
    }
    sendDivider();
    sendIterations();
    timer.start();
    conn=true;
    return conn;
}

void QFExtensionCameraRadhard2::disconnectCameraDevice(unsigned int /*camera*/) {
    if (radhard2) delete radhard2;
    radhard2=NULL;
    log_text(tr("disconnected\n"));
    conn = false;
}

double QFExtensionCameraRadhard2::getCameraExposureTime(unsigned int /*camera*/) {
    return calcExposureTime(iterations, divider)/1.0e6;
}

double QFExtensionCameraRadhard2::calcExposureTime(double piterations, double pdivider) {
	return (double)( ((3840.0/1000000.0) + (20.0*pdivider/1000000.0))*piterations*1000.0 );
}


void QFExtensionCameraRadhard2::updateAcquisitionTime() {
	emit displayAcquisitionTime(tr("%1 usecs").arg(calcExposureTime(spIterations->value(), spDivider->value())));
}

void QFExtensionCameraRadhard2::sendDivider() {
    if (conn) {
        radhard2->SendCommand(CMD_SET_DIVIDER, divider);
        //log_text(tr("setting DIVIDER = %1\n").arg(divider));
    }
}

void QFExtensionCameraRadhard2::sendIterations() {
    if (conn) {
        radhard2->SendCommand(CMD_SET_ITERATIONS, iterations);
        unsigned int result;
        radhard2->ReadData(&result);
        if(result == OPERATION_DONE) {
            //log_text(tr("setting ITERATIONS = %1\n").arg(iterations));
        } else {
            log_error(tr("setting ITERATIONS = %1 failed! (result = %2)\n").arg(iterations).arg(QString::number(result, 16)));
        }
    }
}


bool QFExtensionCameraRadhard2::startCameraAcquisition(unsigned int /*camera*/) {
    return false;
}

void QFExtensionCameraRadhard2::cancelCameraAcquisition(unsigned int /*camera*/) {
}

bool QFExtensionCameraRadhard2::isCameraAcquisitionRunning(unsigned int /*camera*/) {
    return false;
}

void QFExtensionCameraRadhard2::getCameraAcquisitionDescription(unsigned int /*camera*/, QList<QFExtensionCamera::CameraAcquititonFileDescription>* /*files*/, QMap<QString, QVariant>* /*parameters*/) {
}

bool QFExtensionCameraRadhard2::getCameraAcquisitionPreview(unsigned int /*camera*/, uint32_t* /*data*/) {
    return false;
}

int QFExtensionCameraRadhard2::getCameraAcquisitionProgress(unsigned int /*camera*/) {
    return 0;
}

QString QFExtensionCameraRadhard2::getCameraName(unsigned int /*camera*/) {
    return QString("RadHard2");
}

QString QFExtensionCameraRadhard2::getCameraSensorName(unsigned int /*camera*/) {
	return QString("Radhard2");
}
void QFExtensionCameraRadhard2::log_text(QString message) {
	if (logService) logService->log_text(LOG_PREFIX+message);
	else if (services) services->log_text(LOG_PREFIX+message);
}

void QFExtensionCameraRadhard2::log_warning(QString message) {
	if (logService) logService->log_warning(LOG_PREFIX+message);
	else if (services) services->log_warning(LOG_PREFIX+message);
}

void QFExtensionCameraRadhard2::log_error(QString message) {
	if (logService) logService->log_error(LOG_PREFIX+message);
	else if (services) services->log_error(LOG_PREFIX+message);
}


double QFExtensionCameraRadhard2::getCameraPixelWidth(unsigned int /*camera*/) {
	return 30;
}

double QFExtensionCameraRadhard2::getCameraPixelHeight(unsigned int /*camera*/) {
	return 30;
}

bool QFExtensionCameraRadhard2::isCameraSettingChangable(QFExtensionCamera::CameraSetting /*which*/)   {
    return false;
}

void QFExtensionCameraRadhard2::changeCameraSetting(QSettings& /*settings*/, QFExtensionCamera::CameraSetting /*which*/, QVariant /*value*/)  {

}

QVariant QFExtensionCameraRadhard2::getCameraSetting(QSettings& /*settings*/, QFExtensionCamera::CameraSetting /*which*/)  {
    return QVariant();
}

QVariant QFExtensionCameraRadhard2::getCameraCurrentSetting(int /*camera*/, QFExtensionCamera::CameraSetting /*which*/)
{
    return QVariant();
}

//Q_EXPORT_PLUGIN2(cam_radhard2, QFExtensionCameraRadhard2)
Q_EXPORT_PLUGIN2(TARGETNAME, QFExtensionCameraRadhard2)

// global variables for
