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

#include "qfpfitfunctionsspimfcs.h"
#include "qffitfunctionsspimfcsdiff.h"
#include "qffitfunctionsspimfcsdiffe2.h"
#include "qffitfunctionsspimfcsadiff.h"
#include "qffitfunctionsspimfccsdiffflow.h"
#include "qffitfunctionsspimfccsdiff.h"
#include "qffitfunctionsspimfccsdiff2color.h"
#include "qffitfunctionsspimfcsdiffc.h"
#include "qffitfunctionsspimfcsdiffce2.h"
#include "qffitfunctionsspimfcsdiffe2_newveff.h"
#include "qffitfunctionsspimfcsdiffe2_newveff_relbright.h"
#include "qffitfunctionsspimfcsadiffe2.h"
#include "qffitfunctionsspimfcsadiffce2.h"
#include "qffitfunctionsspimfccsfwdiff2colorccf.h"
#include "qffitfunctionsspimfccsfwdiff2coloracfg.h"
#include "qffitfunctionsspimfccsfwdiff2coloracfr.h"
#include "qffitfunctionsspimfccsfwfactordiff2colorccf.h"
#include "qffitfunctionsspimfccsfwfactordiff2coloracfg.h"
#include "qffitfunctionsspimfccsfwfactordiff2coloracfr.h"
#include "qffitfunctionsspimfccsfw2cdiff2colorccf.h"
#include "qffitfunctionsspimfccsfw2cdiff2coloracfg.h"
#include "qffitfunctionsspimfccsfw2cdiff2coloracfr.h"
#include "qffitfunctionsspimfccsfwadiff2colorccf.h"
#include "qffitfunctionsspimfccsfwadiff2coloracfg.h"
#include "qffitfunctionsspimfccsfwadiff2coloracfr.h"
#include "qffitfunctionsspimfccsadiffce2_newVeff.h"
#include "qffitfunctionsspimfccsfw2csepdiff2colorccf.h"
#include "qffitfunctionsspimfccsfw2csepdiff2coloracfg.h"
#include "qffitfunctionsspimfccsfw2csepdiff2coloracfr.h"
#include "qffitfunctionsspimfccsfwsepdiff2colorccf.h"
#include "qffitfunctionsspimfccsfwsepdiff2coloracfg.h"
#include "qffitfunctionsspimfccsfwsepdiff2coloracfr.h"

#include "qffitfunctionsspimfccsfw2csep2ddiffxy2colorccf.h"
#include "qffitfunctionsspimfccsfw2csep2ddiffxy2coloracfg.h"
#include "qffitfunctionsspimfccsfw2csep2ddiffxy2coloracfr.h"

#include "qffitfunctionsspimfccsfw2csep2ddiffxz2colorccf.h"
#include "qffitfunctionsspimfccsfw2csep2ddiffxz2coloracfg.h"
#include "qffitfunctionsspimfccsfw2csep2ddiffxz2coloracfr.h"

#include "qffitfunctionsspimfccsdiffflowce2_newVeff.h"
#include "qffitfunctionsspimfcsdiffflowce2_newVeff.h"
#include "qffitfunctionsspimfccsdiffce2_newVeff.h"
#include "qffitfunctionsspimfccsdiffflowce2_newVeff_2.h"

#include "qffitfunctionsspimfccsfw2csepadiff2coloracfg.h"
#include "qffitfunctionsspimfccsfw2csepadiff2coloracfr.h"
#include "qffitfunctionsspimfccsfw2csepadiff2colorccf.h"

#include "qftools.h"

QStringList QFPFitFunctionsSPIMFCS::getIDs() const {
    QStringList res;
    res<<"fcs_spim_diff";
    res<<"fcs_spim_diffc";
    res<<"fcs_spim_adiff";

    res<<"fcs_spim_diffe2";
    res<<"fcs_spim_diffe2_newveff";
    res<<"fcs_spim_diffe2_newveff_diffbright";

    res<<"fcs_spim_diffce2";
    res<<"fcs_spim_adiffe2";
    res<<"fcs_spim_adiffce2";
    res<<"fccs_spim_adiffce2";

    res<<"fccs_spim_diff";
    res<<"fccs_spim_diffflow";
    res<<"fccs_spim_diffflowce2";
    res<<"fccs_spim_diff2color";
    res<<"fccs_spim_diffce2";
    res<<"fccs_spim_global_diffflowce2";
    res<<"fcs_spim_diffflowce2";

    res<<"fccs_spim_fw_diff2colorccf";
    res<<"fccs_spim_fw_diff2coloracfg";
    res<<"fccs_spim_fw_diff2coloracfr";

    res<<"fccs_spim_fw_factordiff2colorccf";
    res<<"fccs_spim_fw_factordiff2coloracfg";
    res<<"fccs_spim_fw_factordiff2coloracfr";

    res<<"fccs_spim_fw_sepdiff2coloracfg";
    res<<"fccs_spim_fw_sepdiff2coloracfr";
    res<<"fccs_spim_fw_sepdiff2colorccf";

    res<<"fccs_spim_fw_2cdiff2colorccf";
    res<<"fccs_spim_fw_2cdiff2coloracfg";
    res<<"fccs_spim_fw_2cdiff2coloracfr";

    res<<"fccs_spim_fw_2csepdiff2coloracfg";
    res<<"fccs_spim_fw_2csepdiff2coloracfr";
    res<<"fccs_spim_fw_2csepdiff2colorccf";

    res<<"fccs_spim_fw_2csep2ddiffxy2coloracfg";
    res<<"fccs_spim_fw_2csep2ddiffxy2coloracfr";
    res<<"fccs_spim_fw_2csep2ddiffxy2colorccf";

    res<<"fccs_spim_fw_2csep2ddiffxz2coloracfg";
    res<<"fccs_spim_fw_2csep2ddiffxz2coloracfr";
    res<<"fccs_spim_fw_2csep2ddiffxz2colorccf";

    res<<"fccs_spim_fw_adiff2coloracfg";
    res<<"fccs_spim_fw_adiff2coloracfr";
    res<<"fccs_spim_fw_adiff2colorccf";

    res<<"fccs_spim_fw_2csepadiff2coloracfg";
    res<<"fccs_spim_fw_2csepadiff2coloracfr";
    res<<"fccs_spim_fw_2csepadiff2colorccf";
    return res;
}

QFFitFunction* QFPFitFunctionsSPIMFCS::get(const QString &id) const  {
    //Q_UNUSED(parent);
    if (id=="fcs_spim_diff") {
        return new QFFitFunctionsSPIMFCSDiff();
    } else if (id=="fcs_spim_diffc") {
        return new QFFitFunctionsSPIMFCSDiffC();
    } else if (id=="fcs_spim_diffce2") {
        return new QFFitFunctionsSPIMFCSDiffCE2();
    } else if (id=="fcs_spim_diffe2") {
        return new QFFitFunctionsSPIMFCSDiffE2();
    } else if (id=="fccs_spim_diffce2") {
        return new QFFitFunctionsSPIMFCCSDiffCENewVeff2();
    } else if (id=="fccs_spim_global_diffflowce2") {
        return new QFFitFunctionsSPIMFCCSDiffFlowCENewVeff2Global();
    } else if (id=="fcs_spim_diffflowce2") {
        return new QFFitFunctionsSPIMFCSDiffFlowCENewVeff2();
    } else if (id=="fcs_spim_diffe2_newveff") {
        return new QFFitFunctionsSPIMFCSDiffE2NewVeff();
    } else if (id=="fcs_spim_diffe2_newveff_diffbright") {
        return new QFFitFunctionsSPIMFCSDiffE2NewVeffDiffBright();
    } else if (id=="fcs_spim_adiff") {
        return new QFFitFunctionsSPIMFCSADiff();
    } else if (id=="fcs_spim_adiffe2") {
        return new QFFitFunctionsSPIMFCSADiffE2();
    } else if (id=="fcs_spim_adiffce2") {
        return new QFFitFunctionsSPIMFCSADiffCE2();
    } else if (id=="fccs_spim_diff") {
        return new QFFitFunctionsSPIMFCCSDiff();
    } else if (id=="fccs_spim_diffflow") {
        return new QFFitFunctionsSPIMFCCSDiffFlow();
    } else if (id=="fccs_spim_diffflowce2") {
        return new QFFitFunctionsSPIMFCCSDiffFlowCENewVeff2();
    } else if (id=="fccs_spim_diff2color") {
        return new QFFitFunctionsSPIMFCCSDiff2Color();
    } else if (id=="fccs_spim_adiffce2") {
        return new QFFitFunctionsSPIMFCCSADiffCENewVeff2();
    } else if (id=="fccs_spim_fw_diff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFWDiff2ColorACFG();
    } else if (id=="fccs_spim_fw_diff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFWDiff2ColorACFR();
    } else if (id=="fccs_spim_fw_diff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFWDiff2ColorCCF();
    } else if (id=="fccs_spim_fw_factordiff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFWFactorDiff2ColorACFG();
    } else if (id=="fccs_spim_fw_factordiff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFWFactorDiff2ColorACFR();
    } else if (id=="fccs_spim_fw_factordiff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFWFactorDiff2ColorCCF();
    } else if (id=="fccs_spim_fw_2cdiff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFW2CDiff2ColorACFG();
    } else if (id=="fccs_spim_fw_2cdiff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFW2CDiff2ColorACFR();
    } else if (id=="fccs_spim_fw_2cdiff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFW2CDiff2ColorCCF();
    } else if (id=="fccs_spim_fw_adiff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFWADiff2ColorCCF();
    } else if (id=="fccs_spim_fw_adiff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFWADiff2ColorACFG();
    } else if (id=="fccs_spim_fw_adiff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFWADiff2ColorACFR();
    } else if (id=="fccs_spim_fw_2csepdiff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFW2CSepDiff2ColorACFG();
    } else if (id=="fccs_spim_fw_2csepdiff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFW2CSepDiff2ColorACFR();
    } else if (id=="fccs_spim_fw_2csepdiff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFW2CSepDiff2ColorCCF();
    } else if (id=="fccs_spim_fw_2csep2ddiffxy2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFW2CSep2DDiffXY2ColorACFG();
    } else if (id=="fccs_spim_fw_2csep2ddiffxy2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFW2CSep2DDiffXY2ColorACFR();
    } else if (id=="fccs_spim_fw_2csep2ddiffxy2colorccf") {
        return new QFFitFunctionsSPIMFCCSFW2CSep2DDiffXY2ColorCCF();

    } else if (id=="fccs_spim_fw_2csep2ddiffxz2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFW2CSep2DDiffXZ2ColorACFG();
    } else if (id=="fccs_spim_fw_2csep2ddiffxz2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFW2CSep2DDiffXZ2ColorACFR();
    } else if (id=="fccs_spim_fw_2csep2ddiffxz2colorccf") {
        return new QFFitFunctionsSPIMFCCSFW2CSep2DDiffXZ2ColorCCF();

    } else if (id=="fccs_spim_fw_sepdiff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFWSepDiff2ColorACFG();
    } else if (id=="fccs_spim_fw_sepdiff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFWSepDiff2ColorACFR();
    } else if (id=="fccs_spim_fw_sepdiff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFWSepDiff2ColorCCF();

    } else if (id=="fccs_spim_fw_2csepadiff2colorccf") {
        return new QFFitFunctionsSPIMFCCSFW2CSepADiff2ColorCCF();
    } else if (id=="fccs_spim_fw_2csepadiff2coloracfr") {
        return new QFFitFunctionsSPIMFCCSFW2CSepADiff2ColorACFR();
    } else if (id=="fccs_spim_fw_2csepadiff2coloracfg") {
        return new QFFitFunctionsSPIMFCCSFW2CSepADiff2ColorACFG();

    }
    return NULL;
}

int QFPFitFunctionsSPIMFCS::getGlobalFitConfigCount() const
{
    return 14;
}

QFFitFunctionConfigForGlobalFitInterface::GlobalFitConfig QFPFitFunctionsSPIMFCS::getGlobalFitConfig(int i) const
{

    int c=0;
    QString spimfccslabel=tr("configure for SPIM-FCCS ...");
    QFFitFunctionConfigForGlobalFitInterface::GlobalFitConfig res;
    if (i==c++) { // fccs_spim_fw_diff2coloracfg, fccs_spim_fw_diff2coloracfr, fccs_spim_fw_diff2colorccf
        res.groupLabel=spimfccslabel;
        res.shortLabel=QString("2-COLOR-SPIM-FCCS/NORMAL_1DPERSPECIES");
        res.menuEntryLabel=tr("... 3D Normal Diffusion, species A+B+AB, c/D per species");
        res.models<<"fccs_spim_fw_diff2coloracfg"<<"fccs_spim_fw_diff2coloracfr"<<"fccs_spim_fw_diff2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);


    } else if (i==c++) { // fccs_spim_fw_diff2coloracfg, fccs_spim_fw_diff2coloracfr, fccs_spim_fw_diff2colorccf
        res.groupLabel=spimfccslabel;
        res.menuEntryLabel=tr("... 3D Normal Diffusion, species A+B+AB, c/D per species, Da=Db=Dab");
        res.models<<"fccs_spim_fw_diff2coloracfg"<<"fccs_spim_fw_diff2coloracfr"<<"fccs_spim_fw_diff2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_coeff_a", "diff_coeff_b", "diff_coeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);

    } else if (i==c++) { // fccs_spim_fw_diff2coloracfg, fccs_spim_fw_diff2coloracfr, fccs_spim_fw_diff2colorccf
        res.groupLabel=spimfccslabel;
        res.menuEntryLabel=tr("... 3D Normal Diffusion, species A+B+AB, c/D per species, Da=Db, Dab");
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.models<<"fccs_spim_fw_diff2coloracfg"<<"fccs_spim_fw_diff2coloracfr"<<"fccs_spim_fw_diff2colorccf";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_coeff_a", "diff_coeff_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);



    } else if (i==c++) { // fccs_spim_fw_factordiff2coloracfg, fccs_spim_fw_factordiff2coloracfr, fccs_spim_fw_factordiff2colorccf
       res.groupLabel=spimfccslabel;
       res.menuEntryLabel=tr("... 3D Normal Diffusion, species A+B+AB, c/D per species, Da, Db=Fb*Da, Dab=Fab*Da");
       res.models<<"fccs_spim_fw_factordiff2coloracfg"<<"fccs_spim_fw_factordiff2coloracfr"<<"fccs_spim_fw_factordiff2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coefffactor_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coefffactor_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);



    } else    if (i==c++) { // fccs_spim_fw_sepdiff2coloracfg, fccs_spim_fw_sepdiff2coloracfr, fccs_spim_fw_sepdiff2colorccf
       res.groupLabel=spimfccslabel;
       res.shortLabel=QString("2-COLOR-SPIM-FCCS/NORMAL_1DPERCHANNEL");
       res.menuEntryLabel=tr("... 3D Normal Diffusion, species A+B+AB, c per species, D per channel");
       res.models<<"fccs_spim_fw_sepdiff2coloracfg"<<"fccs_spim_fw_sepdiff2coloracfr"<<"fccs_spim_fw_sepdiff2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";

       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);

       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);



    } else    if (i==c++) { // fccs_spim_fw_2cdiff2coloracfg, fccs_spim_fw_2cdiff2coloracfr, fccs_spim_fw_2cdiff2colorccf
       res.groupLabel=spimfccslabel;
       res.shortLabel=QString("2-COLOR-SPIM-FCCS/NORMAL_2DPERSPECIES");
       res.menuEntryLabel=tr("... 2-comp. 3D Normal Diffusion, species A+B+AB, c/D1/D2 per species");
       res.roles<<"acf0"<<"acf1"<<"fccs";
       res.models<<"fccs_spim_fw_2cdiff2coloracfg"<<"fccs_spim_fw_2cdiff2coloracfr"<<"fccs_spim_fw_2cdiff2colorccf";
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff2_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff2_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff2_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);

    } else    if (i==c++) { // fccs_spim_fw_sepdiff2coloracfg, fccs_spim_fw_sepdiff2coloracfr, fccs_spim_fw_sepdiff2colorccf
       res.groupLabel=spimfccslabel;
       res.shortLabel=QString("2-COLOR-SPIM-FCCS/ANOMALOUS_1DPERCHANNEL");
       res.menuEntryLabel=tr("... 3D Anomalous Diffusion, species A+B+AB, c per species, Gamma/alpha per channel");
       res.models<<"fccs_spim_fw_2csepadiff2coloracfg"<<"fccs_spim_fw_2csepadiff2coloracfr"<<"fccs_spim_fw_2csepadiff2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";

       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);

       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);



    } else    if (i==c++) { // fccs_spim_fw_2cdiff2coloracfg, fccs_spim_fw_2cdiff2coloracfr, fccs_spim_fw_2cdiff2colorccf
       res.groupLabel=spimfccslabel;
       res.menuEntryLabel=tr("... 2-comp. 3D Normal Diffusion, species A+B+AB, c/D1/D2 per species, D1A=D1B=D1AB, ...");
       res.roles<<"acf0"<<"acf1"<<"fccs";
       res.models<<"fccs_spim_fw_2cdiff2coloracfg"<<"fccs_spim_fw_2cdiff2coloracfr"<<"fccs_spim_fw_2cdiff2colorccf";
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_coeff_a","diff_coeff_b","diff_coeff_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_coeff2_a","diff_coeff2_b","diff_coeff2_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_rho2_a","diff_rho2_b","diff_rho2_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);


    } else    if (i==c++) { // fccs_spim_fw_2csepdiff2coloracfg, fccs_spim_fw_2csepdiff2coloracfr, fccs_spim_fw_2csepdiff2colorccf
       res.groupLabel=spimfccslabel;
       res.shortLabel=QString("2-COLOR-SPIM-FCCS/NORMAL_2DPERCHANNEL");
       res.menuEntryLabel=tr("... 2-comp. 3D Normal Diffusion, species A+B+AB, c per species, D1/D2 per channel");
       res.models<<"fccs_spim_fw_2csepdiff2coloracfg"<<"fccs_spim_fw_2csepdiff2coloracfr"<<"fccs_spim_fw_2csepdiff2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";

       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);

       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);

    } else    if (i==c++) { // fccs_spim_fw_2csep2ddiffxy2coloracfg, fccs_spim_fw_2csep2ddiffxy2coloracfr, fccs_spim_fw_2csep2ddiffxy2colorccf
       res.groupLabel=spimfccslabel;
       res.menuEntryLabel=tr("... 2-comp. 2D Normal Diffusion (xy), species A+B+AB, c per species, D1/D2 per channel");
       res.models<<"fccs_spim_fw_2csep2ddiffxy2coloracfg"<<"fccs_spim_fw_2csep2ddiffxy2coloracfr"<<"fccs_spim_fw_2csep2ddiffxy2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";

       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);

       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);


    } else    if (i==c++) { // fccs_spim_fw_2csep2ddiffxz2coloracfg, fccs_spim_fw_2csep2ddiffxz2coloracfr, fccs_spim_fw_2csep2ddiffxz2colorccf
       res.groupLabel=spimfccslabel;
       res.menuEntryLabel=tr("... 2-comp. 2D Normal Diffusion (xz), species A+B+AB, c per species, D1/D2 per channel");
       res.models<<"fccs_spim_fw_2csep2ddiffxz2coloracfg"<<"fccs_spim_fw_2csep2ddiffxz2coloracfr"<<"fccs_spim_fw_2csep2ddiffxz2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";

       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);

       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);

    } else if (i==c++) { // fccs_spim_fw_adiff2coloracfg, fccs_spim_fw_adiff2coloracfr, fccs_spim_fw_adiff2colorccf
        res.groupLabel=spimfccslabel;
        res.shortLabel=QString("2-COLOR-SPIM-FCCS/ANOMALOUS_1DPERSPECIES");
        res.menuEntryLabel=tr("... 3D Anomalous Diffusion, species A+B+AB, c/Gamma/alpha per species");
        res.models<<"fccs_spim_fw_adiff2coloracfg"<<"fccs_spim_fw_adiff2coloracfr"<<"fccs_spim_fw_adiff2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);

    } else if (i==c++) { // fccs_spim_fw_adiff2coloracfg, fccs_spim_fw_adiff2coloracfr, fccs_spim_fw_adiff2colorccf
        res.groupLabel=spimfccslabel;
        res.shortLabel=QString("2-PIXEL-SPIM-FCCS/NORMAL_FLOW");
        res.menuEntryLabel=tr("... 3D Normal Diffusion+Flow, ACF + 4 neighbors");
        res.roles<<"acf"<<"dccf"<<"dccf"<<"dccf"<<"dccf";
        res.models<<"fccs_spim_diffflowce2"<<"fccs_spim_diffflowce2"<<"fccs_spim_diffflowce2"<<"fccs_spim_diffflowce2"<<"fccs_spim_diffflowce2";
        res.globalParams << constructQListWithMultipleItems(QStringList("n_particle"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("n_components"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("vflowx"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("vflowy"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width"), 5);
    } else if (i==c++) { // fccs_spim_fw_adiff2coloracfg, fccs_spim_fw_adiff2coloracfr, fccs_spim_fw_adiff2colorccf
        res.groupLabel=spimfccslabel;
        res.shortLabel=QString("2-PIXEL-SPIM-FCCS/ANOMALOUS");
        res.menuEntryLabel=tr("... 3D Anomalous DIffusion, ACF + 4 neighbors");
        res.roles<<"acf"<<"dccf"<<"dccf"<<"dccf"<<"dccf";
        res.models<<"fccs_spim_adiffce2"<<"fccs_spim_adiffce2"<<"fccs_spim_adiffce2"<<"fccs_spim_adiffce2"<<"fccs_spim_adiffce2";
        res.globalParams << constructQListWithMultipleItems(QStringList("n_particle"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("n_components"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff1"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha1"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width"), 5);


    }
    return res;
}

Q_EXPORT_PLUGIN2(fitfunctions_spimfcs, QFPFitFunctionsSPIMFCS)
