#! /usr/bin/env bash
# Evaluate cycle, size and checking accuracy for vector project
#
# Input:
#  $1: extra compiler flags
#
# Enable switch:
#  ENA_BUILD_LIB:       ## 1 for rebuild libvec.a; 0 for not.
#  ENA_DEBUG:           ## 1 for run case in "../../testsuite/run_accuracy.sh"(line 110~121) , 0 for all
# Independent switch: (only enable one at a time)
#  ENA_ACCURACY:        ## 1 for check accuracy of libvec.a; 0 for not.
#
# Specify defined:
#  -DTEST_MAT_DIM=xx:  for mat_mul_fxx to change the input squared matrix dimension, default is 32.
#  -DPRINT_DETAIL:     for print out the ouput and golden for testing functions.
#
# Example:
#  1. change enable switch you want.
#  2. ./run.sh "-O3 "

########  User Change  #########
COMPILER_NAME="gcc"
ENA_BUILD_LIB=1
ENA_ACCURACY=1
ENA_MAKE_INSTALL=0  ## enable pkg the libvec.a and header.
ENA_DEBUG=0
ENA_FP16=0
UNIT_TEST_FUNC="add_f32"  ## function name for unit test
###############################

## default setting
ENA_BUILD_DOC=0   ## enable generate doc for doxygen, **not using now.
ENA_BUILD_EMPTY_LIB=0
ENA_SIZE=0
LIB_FILE="libvec.a"

LOCAL=`pwd`
cd ../../
LIB_ROOT=`pwd`
cd ${LOCAL}
mkdir -p adx
cd adx/
ADX_PATH=`pwd`
cd ${LOCAL}

export PATH=${LOCAL}:${TOOLCHAIN_PATH}:${PATH}

VEC_FLAG=""
LIBVEC_PATH="$LIB_ROOT/project/build_dir/install/libvec.a"

if [ ${ENA_SIZE} -eq 1 ];then
    ENA_BUILD_EMPTY_LIB=1
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_SIZE"
fi

EXTRA_FLAGS="$EXTRA_FLAGS $1"
SIM_WRAPPER=""

if [[ $EXTRA_FLAGS == *"-mzfh"* ]] || [[ ${ENA_FP16} -eq 1 ]]; then
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_FP16"
fi

if [ ${ENA_DEBUG} -eq 1 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_DEBUG"
    if [ "$UNIT_TEST_FUNC" == "" ]; then
        echo "Error: UNIT_TEST_FUNC is empty !!"
        exit
    fi
    export UNIT_TEST_LIST="$UNIT_TEST_FUNC"
fi

rm -f ${LOCAL}/build_libvec.log
rm -f ${LOCAL}/accuracy.log

###################### main  #################################################
## Build libvec
if [ ${ENA_BUILD_LIB} -eq 1 ];then
    echo "--------------------------"
    cd ../
    BUILD_PATH=`pwd`
    LIBVEC_PATH="${BUILD_PATH}/build_dir/install/${LIB_FILE}"
    echo "Start build libvec.a ${VEC_FLAG} ${EXTRA_FLAGS} ..."
    ./build_lib.sh "${COMPILER_NAME}" "${VEC_FLAG} ${EXTRA_FLAGS}" > ${LOCAL}/build_libvec.log
    if [ ! -e "${LIBVEC_PATH}" ]; then
        echo "Build libvec.a Fail: not found !!"
        ENA_ACCURACY=0
        ENA_PERF=0
        ENA_BUILD_DOC=0
        ENA_MAKE_INSTALL=0
        exit 1
    else
        echo "Build libvec.a Ok !!"
        echo "install: ${LIBVEC_PATH}"
    fi
    echo "    log: ${BUILD_PATH}/build_dir/build.log"
    cd ${LOCAL}
    echo "--------------------------"
fi

if [[ ${ENA_ACCURACY} -eq 1 ]] || [[ ${ENA_PERF} -eq 1 ]]; then
## Check accuracy
    echo "--------------------------"
    echo "Start check accuracy of libvec.a ..."
        ../../testsuite/run_accuracy.sh "${COMPILER_NAME}" "${SIM_WRAPPER}" "${VEC_FLAG} ${EXTRA_FLAGS}" "${LIBVEC_PATH}" "$ADX_PATH"
    cd ${LOCAL}
    if [ ${ENA_ACCURACY} -eq 1 ];then
        grep  'FAIL' ${LOCAL}/accuracy.log
        if [ "$?" == "1" ]; then
            echo "All test pattern PASS"
        else
            CNT=`grep 'FAIL' ${LOCAL}/accuracy.log | wc -l`
            echo "${CNT} test FAIL !!"
        fi
    fi
    echo "--------------------------"
fi

## Generate Document
if [ ${ENA_BUILD_DOC} -eq 1 ]; then
    cd ${LIB_ROOT}/Docs/
    rm -rf ./docs
    echo "**********************************************"
    echo " Generating document for libvec library ..."
    echo "**********************************************"
    echo
    doxygen Doxyfile > log_doxygen.txt && cd docs/latex && make > log_make_latex.txt 2>&1
    mv log_make_latex.txt ${LOCAL}
    cd ..
    mkdir -p pdf
    cp latex/refman.pdf pdf/libvec_user_manual.pdf
    cp rtf/refman.rtf rtf/libvec_user_manual.rtf
    cd ${LOCAL}
    echo
    echo "Done!"
    echo "**********************************************"
    echo "Vector Library (libvec) User Manual is generated with two format:"
    echo "html folder: ${LIB_ROOT}/Docs/docs/html"
    echo "PDF file: ${LIB_ROOT}/Docs/docs/pdf/libvec_user_manual.pdf"
    echo "**********************************************"
fi

## Installation
if [ ${ENA_MAKE_INSTALL} -eq 1 ]; then
    RELEASE_FOLDER="${LIB_ROOT}/release"
    mkdir -p ${RELEASE_FOLDER}
    rm -rf ${RELEASE_FOLDER}/*
    cp -f ${LIBVEC_PATH} ${RELEASE_FOLDER}
    cp -rf ${LIB_ROOT}/include/ ${RELEASE_FOLDER}

    if [ ${ENA_BUILD_DOC} -eq 1 ]; then
        cp -rf ${LIB_ROOT}/Docs/docs/pdf ${RELEASE_FOLDER}
    fi
    echo "**********************************************"
    echo " libvec library installation done!"
    echo " Folder: ${RELEASE_FOLDER}"
    echo "**********************************************"
    echo
fi  #MAKE_INSTALL
