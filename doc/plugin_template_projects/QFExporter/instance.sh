#! /bin/bash

function replace_in_all {
	echo "replace: " $1 
	for i in ./$FNAME/*.cpp ./$FNAME/*.h ./$FNAME/*.pro ./$FNAME/*.qrc ./$FNAME/help/*.html ./$FNAME/help/*.ini; do 
	  echo "  -> " $i
	  sed $1 $i > $i.temp 
	  mv $i.temp $i
	done
}

CLASSNAME=$1
TARGETNAME=$2
IMPORTERCLASSNAME=$3
IMPORTERNAME=$4
BASENAME=exporter_inst

echo "This script will create a set of usable files from this template. 
In this course you will be asked for some information (e.g. class names ...)!

"

if [ "$CLASSNAME" == "" ]; then
	echo -n "class name for QFExporter plugin implementation: "
	read -e CLASSNAME
else
	echo -n "class name for QFExporter plugin implementation: " $CLASSNAME "
"
fi

if [ "$TARGETNAME" == "" ]; then
	echo -n "name for plugin library (also plugin ID): "
	read -e TARGETNAME
else
	echo -n "name for plugin library (also plugin ID): " $TARGETNAME "
"
fi


if [ "$IMPORTERCLASSNAME" == "" ]; then
	echo -n "class name for first exporter: "
	read -e IMPORTERCLASSNAME
else
	echo -n "class name for first exporter: " $IMPORTERCLASSNAME "
"
fi


if [ "$IMPORTERNAME" == "" ]; then
	echo -n "ID for first fit function: "
	read -e IMPORTERNAME
else
	echo -n "ID for first exporter: " $IMPORTERNAME "
"
fi

CLASSNAME_UC=`echo $CLASSNAME | tr a-z A-Z`
CLASSNAMEFF_UC=`echo ${IMPORTERCLASSNAME} | tr a-z A-Z`

FNAME=`echo $TARGETNAME | tr A-Z a-z`
echo -n "file name for QFFitFunction plugin implementation: " $FNAME ".*
"
FNAMEFF=`echo $IMPORTERCLASSNAME | tr A-Z a-z`
echo -n "file name for QFFitFunction implementation: " $FNAMEFF ".*
"
mkdir -p $FNAME

for i in ${BASENAME}.*; do 
  cp -v "$i" "./$FNAME/${FNAME}${i/$BASENAME}"; 
done

for i in ${BASENAME}_f1.*; do 
  cp -v "$i" "./$FNAME/${FNAMEFF}${i/${BASENAME}_f1}"; 
done

mkdir -p $FNAME/translations
mkdir -p $FNAME/examples
mkdir -p $FNAME/help
mkdir -p $FNAME/help/pic
cp ftarget.html $FNAME/help/${TARGETNAME}.html
cp f1.html $FNAME/help/${IMPORTERNAME}.html


replace_in_all 's/QFFitAlgorithmInst_F1/'$IMPORTERCLASSNAME'/g'
replace_in_all 's/QFFitAlgorithmInst/'$CLASSNAME'/g'
replace_in_all 's/target_id/'$TARGETNAME'/g'
replace_in_all 's/ff_id/'$IMPORTERNAME'/g'

replace_in_all 's/'$BASENAME'_f1.h/'$FNAMEFF'.h/g'
replace_in_all 's/'$BASENAME'_f1.cpp/'$FNAMEFF'.cpp/g'
replace_in_all 's/HEADER_F1_H/'$CLASSNAMEFF_UC'_H/g'

replace_in_all 's/'$BASENAME'.png/'$FNAME'.png/g'
replace_in_all 's/'$BASENAME'.h/'$FNAME'.h/g'
replace_in_all 's/'$BASENAME'.cpp/'$FNAME'.cpp/g'
replace_in_all 's/'$BASENAME'.qrc/'$FNAME'.qrc/g'
replace_in_all 's/'$BASENAME'.ui/'$FNAME'.ui/g'
replace_in_all 's/HEADER_H/'$CLASSNAME_UC'_H/g'

replace_in_all 's/doxygen_GROUPNAME/qf3exporterplugins_'$TARGETNAME'/g'
replace_in_all 's/f1.html/'$IMPORTERNAME'.html/g'
