##########################
# Used for project build libvec
###########################
COMPILER_NAME="$1"
COMPILER_TYPE="`echo ${COMPILER_NAME} | cut -d '-' -f3`"
PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`
EXTRA_FLAG="$2"
CUR_DIR=`pwd`

#Output setting:
BUILD_DIR="$3"
if [ "${BUILD_DIR}" == "" ]; then
    BUILD_DIR="${CUR_DIR}/build_dir"
fi
INSTALL_DIR="${BUILD_DIR}/install"       #this folder is under $BUILD_DIR
SRC_ROOT="../"
cd ${SRC_ROOT}
SRC_ROOT=`pwd`

## Set support fp16 or not
ENA_FP16=0
$COMPILER_NAME ${EXTRA_FLAG} -mzfh -E -dM - < /dev/null  &> /dev/null
if [ "$?" == "0" ];then
    ENA_FP16=1
    EXTRA_FLAG="$EXTRA_FLAG -mzfh"
fi

if [[ $EXTRA_FLAG == *"-mext-vector"* ]]; then
    ENA_VEC=1
else
    ENA_VEC=0
fi

if [[ $EXTRA_FLAG == *"-DENA_EMPTY"* ]]; then
    EXTRA_FLAG="${EXTRA_FLAG} -include ${CUR_DIR}/../testsuite/include/ndsv_empty_function.h"
fi

## check support bf16 or not and enable source file list
ENA_BF16=0  ## option for build command
$COMPILER_NAME ${EXTRA_FLAG} -mext-bf16min -E -dM - < /dev/null  &> /dev/null
if [ "$?" == "0" ];then
    EXTRA_FLAG="${EXTRA_FLAG} -mext-bf16min"
    ENA_BF16=1
    if [[ $EXTRA_FLAG == *"-DENA_VEC_ISA_ZVLSSEG"* ]] || [[ $EXTRA_FLAG == *"-mtune=andes-45-series"* ]] || [[ $COMPILER_NAME == *"linux"* ]]; then
        EXTRA_FLAG="${EXTRA_FLAG} -DENA_MODE_BIT_BF16"
    fi
fi

if [ "${PREFIX}" == "" ]; then
    echo "Usage: build.sh <PREFIX> <EXTRA_FLAGS>"
    exit 1
fi

if [ ${ENA_VEC} -eq 1 ];then
    echo ""
    echo "Build with V extension: YES"
    ARCH="-mext-vector -DENA_VEC_ISA"
else
    echo ""
    echo "Build with V extension: NO"
    ARCH=""
fi

## Check bash verion to decide parallel building
REQ_BASH_MAJOR=5
BASH_MAJOR=`bash --version | head -n1 | cut -d' ' -f4 | cut -d'.' -f1`
PARALLEL_BUILD="false"
if { [ $BASH_MAJOR -gt $REQ_BASH_MAJOR ] || [ $BASH_MAJOR -eq $REQ_BASH_MAJOR ]; } then
    PARALLEL_BUILD="true"
fi
echo "[INFO] Parallel building': $PARALLEL_BUILD"

#Toolchain setting:
CC="${COMPILER_NAME}"
AR="${PREFIX}-ar"
RANLIB="${PREFIX}-ranlib -D"

if [ "${COMPILER_NAME}" == "gcc" ]; then
    DUMP="objdump"
else
    DUMP="${PREFIX}-objdump"
fi


source ${SRC_ROOT}/mat_mul_config
MAT_MUL_CONFIG=${MAT_MUL_FLAGS}

INC_PATH="-I${SRC_ROOT}/include -I${SRC_ROOT}/internal"
AR_FLAG="crD"
DUMP_FLAG="-S"
#BUILD_FLAG="-O3 -c ${INC_PATH}"
BUILD_FLAG="-O3 ${INC_PATH}"

if [[ ${COMPILER_NAME} == *"clang"* ]] ; then
    BUILD_FLAG="${BUILD_FLAG} -mno-implicit-float"
fi

BUILD_FLAG="${ARCH} ${BUILD_FLAG} ${MAT_MUL_CONFIG} -ffunction-sections -fdata-sections -Wall -Werror -fno-math-errno -fno-tree-ter -fno-tree-slp-vectorize -fno-tree-vectorize -mcmodel=large ${EXTRA_FLAG}"

BEGIN_TIME=`date +%s`
S_TIME=0
E_TIME=0

function log () {
    echo "$*" | tee -a ${CUR_DIR}/build_dir/build.log
}

function start_time () {
    log "Start at `date`"
    S_TIME=`date +%s`
}

function end_time () {
    E_TIME=`date +%s`
    log "End at `date`, Total library takes $((E_TIME-S_TIME)) seconds."
}

#$1: sub-folder name
#$2: filename (w/o ext. name)
function Build_OBJ () {
    SRC_NAME=`echo ${1} | rev | cut -d '/' -f1 | rev`
    local SRC="${SRC_ROOT}/${1}.c"
    local OBJ="${SRC_NAME}.o"
    local OBJDUMP="objdump_${SRC_NAME}.txt"

    log "Compiling ..."
    log "    In:   ${SRC}"
    log "    Out:  ${OBJ}"
    log "    Dump: ${OBJDUMP}"
    log "${CC} ${BUILD_FLAG} -c ${SRC} -o ${OBJ}"
    log "Start at `date`"
    unit_stime=`date +%s`
    ${CC} ${BUILD_FLAG} -c ${SRC} -o ${OBJ}
    unit_etime=`date +%s`
    log "End at `date`, takes $((unit_etime-unit_stime)) seconds."
    ${DUMP} ${DUMP_FLAG} ${OBJ} > ${OBJDUMP}
    ret=$?
    if [ "$ret" != "0" ];then
        echo "${SRC} fail"
        touch build_fail_hint
        exit
    fi

}


#####main from here#####
start_time

LIB_NAME="libvec"
source ${SRC_ROOT}/source_file.list

# get branch nds_version.c
cd ${SRC_ROOT}
echo -n "Src version:  "
if [ ! -e ${SRC_ROOT}/nds_version.c ]; then
    ./nds_autogen_info.sh libvec
fi

cd ${CUR_DIR}

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
rm -f *.gcda *.gcno
BUILD_PATH=`pwd`
mkdir -p ${INSTALL_DIR}
rm -f ${CUR_DIR}/build_dir/build.log
touch ${CUR_DIR}/build_dir/build.log

rm -f *.o
rm -f objdump*txt
rm -f ${LIB_NAME}.a
rm -f ${INSTALL_DIR}/${LIB_NAME}.a
rm -f build_fail_hint

#Print information:
log "=========================================================================="
log "Compiler: ${CC}"
log "Objdump: ${DUMP}"
log "Ar: ${AR}"
log "Toolchain information:"
${CC} --version
log "C flags: ${BUILD_FLAG}"
log "DUMP flags: ${DUMP_FLAG}"
log "Source root path: ${SRC_ROOT}"
log "Build path: ${BUILD_PATH}"
log "=========================================================================="

#functions:
if [ $PARALLEL_BUILD == "true" ]; then
    N=8
    for obj in ${C_SRC};do
        (
            # .. do your stuff here
            Build_OBJ "${obj}"
        ) &

        # allow to execute up to $N jobs in parallel
        if [[ $(jobs -r -p | wc -l) -ge $N ]]; then
            # now there are $N jobs already running, so wait here for any job
            # to be finished so there is a place to start next one.
            wait -n
        fi

    done
else
    for obj in ${C_SRC};do
        Build_OBJ "${obj}"

        if [[ $? -ne 0 ]]; then
            ret = $?
            printf "!!!\n[ERROR] build error! building is terminated.\n!!!\n"
            exit $((ret))
        fi
    done
fi

# no more jobs to be started but wait for pending jobs
# (all need to be finished)
wait

FAIL_HINT="${BUILD_DIR}/build_fail_hint"
if [ -e "${FAIL_HINT}" ]; then
    echo "Build libvec.a error!"
    exit 1
fi

#library:
ALL_OBJ=`ls *.o`

${AR} ${AR_FLAG} ${LIB_NAME}.a ${ALL_OBJ}
${RANLIB} ${LIB_NAME}.a
if [[ $EXTRA_FLAG == *"-shared"* ]]; then
    DEFAULT_DYNAMIC_LIB_FLAG="-fpic -shared -lm"
    ${CC} ${BUILD_FLAG} ${DEFAULT_DYNAMIC_LIB_FLAG} -o ${LIB_NAME}.a ${ALL_OBJ}
fi

#Install:
cp -rf ${LIB_NAME}.a ${INSTALL_DIR}

#Print building information:
echo
echo "***********************************************************"
echo "Result:"
echo "Library name: ${LIB_NAME}.a"
echo "Install path:"
echo "    ${INSTALL_DIR}"
echo "Build log:"
echo "    ${BUILD_PATH}/build.log"

if [ ${ENA_VEC} -eq 1 ];then
    echo "Build with V extension: YES"
else
    echo "Build with V extension: NO"
fi

echo "***********************************************************"
echo
end_time

cd ${CUR_DIR}
