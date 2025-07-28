#! /usr/bin/env bash
# Evaluate cycle, size and checking accuracy for vector project
#
# Input:
#  $1: extra compiler flags
#
# Enable switch:
#  ENA_VEC:             ## 1 for enable V-ext; 0 for pure c
#  ENA_BUILD_LIB:       ## 1 for rebuild libvec.a; 0 for not.
#  ENA_FPGA:            ## 1 for FPGA, 0 for SID
#  ENA_DEBUG:           ## 1 for run case in "../../testsuite/run_accuracy.sh"(line 110~121) , 0 for all
#  RVV_USING_LVEC       ## option for rvv using multi-lib, default is "-lvec_v_elen64_seg" (after v540)
#  ENA_LVEC:            ## 1 for using "-lvec" or "$RVV_USING_LVEC" in toolchain, 0 for self building library.
# Independent switch: (only enable one at a time)
#  ENA_ACCURACY:        ## 1 for check accuracy of libvec.a; 0 for not.
#  ENA_PERF:            ## 1 for enable "-DPF_COUNTER" to evaluate performance and size in "perf.csv"; 0 for not.
#  ENA_PERF_MAT_MUL_ALG:## 1 for evaluate the performance for mat_mul_f64 and generate the mat_mul_config for defined algorithm
#  ENA_HIGH_ADDRESS_TEST: ## 1 for check accuracy of libvec.a at high address; 0 for not.
#
# Specify defined:
#  -DENA_PMA:          for enable pma gdb command "gdb_pma.cmd"
#  -DTEST_MAT_DIM=xx:  for mat_mul_fxx to change the input squared matrix dimension, default is 32.
#  -DPRINT_DETAIL:     for print out the ouput and golden for testing functions.
#
# Example:
#  1. change enable switch you want.
#  2. ./run.sh "-O3 -DENA_PMA"

########  User Change  #########
ENA_VEC=0           #0/1
COMPILER_NAME="riscv64-elf-gcc"
ENA_BUILD_LIB=1
ENA_ACCURACY=1
ENA_PERF=0
ENA_SIZE=0
ENA_MHPMEVENT=0
ENA_PERF_MAT_MUL_ALG=0
ENA_HIGH_ADDRESS_TEST=0
ENA_MAKE_INSTALL=0  ## enable pkg the libvec.a and header.
ENA_FPGA=1
ENA_DEBUG=0
ENA_ZVLSSEG=0
ENA_FP16=1  ## v5d/v5f will default support
ENA_BF16=1  ## v5d/v5f will default support
ENA_LVEC=0
RVV_USING_LVEC="-lvec"
ENA_VEC_INTRINSIC=0
UNIT_TEST_FUNC="add_f32"  ## function name for unit test
MHPMEVENT_PFM_GP=6    ## 0~6 details please see nds_pfcounter.h
###############################

## default setting
ENA_BUILD_DOC=0   ## enable generate doc for doxygen, **not using now.
ENA_BUILD_EMPTY_LIB=0
LIB_FILE="libvec.a"

LOCAL=`pwd`
cd ../../
LIB_ROOT=`pwd`
cd ${LOCAL}
mkdir -p adx
cd adx/
ADX_PATH=`pwd`
cd ${LIB_ROOT}/testsuite/high_address_env
HIGH_ADDRESS_PATH=`pwd`
cd ${LOCAL}
rm -f perf.log accuracy.log perf.csv

#Check COMPILER_NAME exit or not
which $COMPILER_NAME
compiler_exit=$?
if [ $compiler_exit != "0" ];then
    echo "$COMPILER_NAME not found!"
    echo "please export toolchain in PATH"
    exit 1
fi

export PATH=${LOCAL}:${TOOLCHAIN_PATH}:${HIGH_ADDRESS_PATH}:${LOCAL}:${PATH}
echo $PATH

if [ ${ENA_PERF_MAT_MUL_ALG} -eq 1 ];then
    ENA_VEC=1
    ENA_ACCURACY=0
    ENA_PERF=0
fi

if [ ${ENA_VEC_INTRINSIC} -eq 1 ];then
    echo "--------------------------"
    echo "ENA_VEC_INTRINSIC=${ENA_VEC_INTRINSIC}"
    COMPILER_NAME="riscv64-elf-clang"
    echo "Only support Compiler : ${COMPILER_NAME}"
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_VEC_INTRINSIC -fno-unroll-loops"
    echo "Extra compiler option : ${EXTRA_FLAGS}"
    echo "--------------------------"
fi

if [ ${ENA_VEC} -eq 1 ];then
    if [[ ${COMPILER_NAME} == *"-gcc"* ]]; then
        VEC_FLAG="-mext-vector -DENA_VEC_ISA -fno-tree-ter"
    else
        VEC_FLAG="-mext-vector -DENA_VEC_ISA"
    fi
else
    VEC_FLAG=""
fi
VEC_FLAG="${VEC_FLAG} -mtune=andes-45-series"
if [ ${ENA_LVEC} -eq 1 ];then
    if [ ${ENA_VEC} -eq 1 ];then
        LIBVEC_PATH="${RVV_USING_LVEC}"
    else
        LIBVEC_PATH="-lvec"
    fi
    ENA_BUILD_LIB=0
else
    LIBVEC_PATH="$LIB_ROOT/project/build_dir/install/libvec.a"
fi
echo "==========================" | tee -a ${LOCAL}/accuracy.log
echo "LIBVEC_PATH : $LIBVEC_PATH" | tee -a ${LOCAL}/accuracy.log
echo "==========================" | tee -a ${LOCAL}/accuracy.log

if [ ${ENA_SIZE} -eq 1 ];then
    ENA_BUILD_EMPTY_LIB=1
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_SIZE"
fi

if [ ${ENA_MHPMEVENT} -eq 1 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_MHPMEVENT -DPFM_GP=$MHPMEVENT_PFM_GP"
fi

if [ ${ENA_PERF} -eq 1 ];then
    ENA_HIGH_ADDRESS_TEST=0
fi

## now testing onlu for sid and rv64
if [ ${ENA_HIGH_ADDRESS_TEST} -eq 1 ];then
    LFLAGS="-T ./entry_point_start_at_4g.ld"
    ENA_FPGA=0
fi

if [ ${ENA_FPGA} -eq 1 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -mvh $1"
    SIM_WRAPPER="riscv-sim-wrapper-on-board"
    rm -f Can_not_connect First_Reset_done
elif [ ${ENA_FPGA} -eq 2 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -mvh $1"
    SIM_WRAPPER="riscv-qemu-wrapper-v1024_s1024_b512_elen64"

    #Check QEMU exit or not
    which $QEMU
    qemu_exit=$?
    if [ $qemu_exit != "0" ];then
        echo "${QEMU} not found!"
        echo "please export QEMU qemu-system-riscv64"
        exit 1
    fi
else
    EXTRA_FLAGS="$EXTRA_FLAGS $1"
    SIM_WRAPPER="riscv-sim-wrapper-vec"
fi

echo "SIM_WRAPPER = $SIM_WRAPPER" | tee -a ${LOCAL}/accuracy.log
echo "==========================" | tee -a ${LOCAL}/accuracy.log

if [[ $EXTRA_FLAGS == *"-DENA_PMA"* ]]; then
    SIM_WRAPPER="riscv64-elf-gdb -q -x ${LOCAL}/gdb_pma.cmd"
fi

if [ ${ENA_ZVLSSEG} -eq 1 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_VEC_ISA_ZVLSSEG"
fi

${COMPILER_NAME} ${EXTRA_FLAGS} -mzfh -E -dM - < /dev/null  &> /dev/null
if [ "$?" != "0" ];then
    ENA_FP16=0
fi

if [[ $EXTRA_FLAGS == *"-mzfh"* ]] || [[ ${ENA_FP16} -eq 1 ]]; then
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_FP16 -mzfh"
fi

${COMPILER_NAME} ${EXTRA_FLAGS} -mzfh -mext-bf16min -E -dM - < /dev/null  &> /dev/null
if [ "$?" != "0" ];then
    ENA_BF16=0
fi
if [[ $EXTRA_FLAGS == *"-mext-bf16min"* ]] || [[ ${ENA_BF16} -eq 1 ]]; then
    #EXTRA_FLAGS="$EXTRA_FLAGS -mext-bf16min -DENA_MODE_BIT_BF16"
    EXTRA_FLAGS="$EXTRA_FLAGS -mext-bf16min"
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
rm -f ${LOCAL}/check_accuracy.log

###################### main  #################################################
## ENA_PERF_MAT_MUL_ALG = 1
if [ ${ENA_PERF_MAT_MUL_ALG} -eq 1 ];then
    cd ${LIB_ROOT}/testsuite/sel_mat_mul/
    ./run_perf_mat_mul_alg.sh "${COMPILER_NAME}" "${EXTRA_FLAGS} ${VEC_FLAG}"
    cp mat_mul_config ${LOCAL}/
fi

## Build libvec
if [ ${ENA_BUILD_LIB} -eq 1 ];then
    echo "--------------------------"
    cd ../
    BUILD_PATH=`pwd`
    LIBVEC_PATH="${BUILD_PATH}/build_dir/install/${LIB_FILE}"
    if [ "${ENA_BUILD_EMPTY_LIB}" -eq 1 ];then
        echo "Start build empty libvec.a ${VEC_FLAG} ${EXTRA_FLAGS} -DENA_EMPTY ..."
        ./build_lib.sh ${COMPILER_NAME} "${VEC_FLAG} ${EXTRA_FLAGS} -DENA_EMPTY" > ${LOCAL}/build_libvec.log
        if [ ! -e "${LIBVEC_PATH}" ]; then
            echo "Build empty libvec.a Fail: not found !!"
            exit 1
        else
            echo "Build empty libvec.a Ok !!"
            echo "${LIBVEC_PATH}"
        fi
        cp ${LIBVEC_PATH} ${BUILD_PATH}/libvec_empty.a
        echo "--------------------------"
    fi
    echo "Start build libvec.a ..."
    echo "COMPILER: ${COMPILER_NAME}"
    echo "EXTRA_FLAGS: ${VEC_FLAG} ${EXTRA_FLAGS}"
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

start_time=`date +%s`
if [ ${ENA_ACCURACY} -eq 1 ]; then
## Check accuracy
    echo "--------------------------"
    echo "Start check accuracy of libvec.a ..."
    if [ "$ENA_HIGH_ADDRESS_TEST" == 0 ]; then
        ../../testsuite/run_accuracy.sh "${COMPILER_NAME}" "${SIM_WRAPPER}" "${VEC_FLAG} ${EXTRA_FLAGS}" "${LIBVEC_PATH}" "$ADX_PATH"
    else
        HIGH_ADDRESS_SIM_WRAPPER="riscv-sim-wrapper-vec_high_address_cache"
        ../../testsuite/run_accuracy.sh "${COMPILER_NAME}" "${HIGH_ADDRESS_SIM_WRAPPER}" "${VEC_FLAG} ${EXTRA_FLAGS} ${LFLAGS}" "${LIBVEC_PATH}" "$ADX_PATH"
        HIGH_ADDRESS_SIM_WRAPPER="riscv-sim-wrapper-vec_high_address_cache_enailmdlm"
        ../../testsuite/run_accuracy.sh "${COMPILER_NAME}" "${HIGH_ADDRESS_SIM_WRAPPER}" "${VEC_FLAG} ${EXTRA_FLAGS} ${LFLAGS}" "${LIBVEC_PATH}" "$ADX_PATH"
        HIGH_ADDRESS_SIM_WRAPPER="riscv-sim-wrapper-vec_high_address_ilmdlm"
        ../../testsuite/run_accuracy.sh "${COMPILER_NAME}" "${HIGH_ADDRESS_SIM_WRAPPER}" "${VEC_FLAG} ${EXTRA_FLAGS} ${LFLAGS}" "${LIBVEC_PATH}" "$ADX_PATH"
    fi
    cd ${LOCAL}
    if [ ${ENA_ACCURACY} -eq 1 ];then
        grep  'FAIL' ${LOCAL}/accuracy.log
        if [ "$?" == "1" ]; then
            echo "All test pattern PASS" | tee -a ${LOCAL}/accuracy.log
        else
            CNT=`grep 'FAIL' ${LOCAL}/accuracy.log | wc -l`
            echo "${CNT} test FAIL !!" | tee -a ${LOCAL}/accuracy.log
        fi
    fi
    echo "--------------------------"
fi
if [ ${ENA_PERF} -eq 1 ]; then
    EXTRA_FLAGS="$EXTRA_FLAGS -DPF_COUNTER"
    ../../testsuite/run_accuracy.sh "${COMPILER_NAME}" "${SIM_WRAPPER}" "${VEC_FLAG} ${EXTRA_FLAGS}" "${LIBVEC_PATH}" "$ADX_PATH"
fi
end_time=`date +%s`

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

echo "start time: $start_time" | tee -a ${LOCAL}/accuracy.log
echo "end time :$end_time, takes $((end_time-start_time)) seconds." | tee -a ${LOCAL}/accuracy.log
