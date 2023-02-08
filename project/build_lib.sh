##########################
# Used for project build libvec
###########################
COMPILER_NAME="$1"
COMPILER_TYPE="`echo ${COMPILER_NAME} | cut -d '-' -f3`"
PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`
EXTRA_FLAG="$2"
CUR_DIR=`pwd`

#Output setting:
BUILD_DIR="build_dir"
INSTALL_DIR="install"       #this folder is under $BUILD_DIR
SRC_ROOT="../"
cd ${SRC_ROOT}
SRC_ROOT=`pwd`

### Set support fp16 or not
#$COMPILER_NAME ${EXTRA_FLAG} -mzfh -E -dM - < /dev/null  &> /dev/null
#if [ "$?" == "0" ];then
#    EXTRA_FLAG="$EXTRA_FLAG -mzfh -DENA_FP16"
#    echo "-DENA_FP16"
#fi

if [[ $EXTRA_FLAG == *"-mext-vector"* ]]; then
    ENA_VEC=1
else
    ENA_VEC=0
fi
if [[ $EXTRA_FLAG == *"-DENA_EMPTY"* ]]; then
    EXTRA_FLAG="${EXTRA_FLAG} -include ${CUR_DIR}/../testsuite/include/ndsv_empty_function.h"
fi
if [[ $EXTRA_FLAG == *"-mzfh"* ]]; then
    ENA_FP16=1
else
    ENA_FP16=0
fi

if [ "${PREFIX}" == "" ]; then
    echo "Usage: build.sh <PREFIX> <EXTRA_FLAGS>"
    exit 1
fi

if [ ${ENA_VEC} -eq 1 ];then
    echo ""
    echo "Build with V extension: YES"
    ARCH="-mext-vector -DENA_VEC_ISA"
    source ${SRC_ROOT}/mat_mul_config
    MAT_MUL_CONFIG=${MAT_MUL_FLAGS}
else
    echo ""
    echo "Build with V extension: NO"
    ARCH=""
    MAT_MUL_CONFIG=""
fi

#Toolchain setting:
CC="${COMPILER_NAME}"
AR="${PREFIX}-ar"
RANLIB="${PREFIX}-ranlib -D"

if [ "${COMPILER_NAME}" == "gcc" ]; then
    DUMP="objdump"
else
    DUMP="${PREFIX}-objdump"
fi

INC_PATH="-I${SRC_ROOT}/include -I${SRC_ROOT}/internal"
AR_FLAG="crD"
DUMP_FLAG="-S"
#BUILD_FLAG="-O3 -c ${INC_PATH}"
BUILD_FLAG="-O3 ${INC_PATH}"

BUILD_FLAG="${ARCH} ${BUILD_FLAG} ${MAT_MUL_CONFIG} -ffunction-sections -fdata-sections -Wall -Werror -fno-math-errno ${EXTRA_FLAG}"


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
    log "End at `date`, takes $((E_TIME-S_TIME)) seconds."
}

#$1: sub-folder name
#$2: filename (w/o ext. name)
function Build_OBJ () {
    SRC_NAME=`echo ${1}| cut -d '/' -f2`
    local SRC="${SRC_ROOT}/${1}.c"
    local OBJ="${SRC_NAME}.o"
    local OBJDUMP="objdump_${SRC_NAME}.txt"

    log "Compiling ..."
    log "    In:   ${SRC}"
    log "    Out:  ${OBJ}"
    log "    Dump: ${OBJDUMP}"
    log "${CC} ${BUILD_FLAG} -c ${SRC} -o ${OBJ}"
    ${CC} ${BUILD_FLAG} -c ${SRC} -o ${OBJ}
    ${DUMP} ${DUMP_FLAG} ${OBJ} > ${OBJDUMP}
    ret=$?
    if [ "$ret" != "0" ];then
        echo "${SRC} fail"
        exit
    fi

}


#####main from here#####
rm -f ${CUR_DIR}/build_dir/build.log
start_time

LIB_NAME="libvec"
source ${SRC_ROOT}/source_file.list

## get branch nds_version.c
#cd ${SRC_ROOT}
#echo -n "Src version:  "
#./nds_autogen_info.sh libvec

cd ${CUR_DIR}

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
rm -f *.gcda *.gcno
BUILD_PATH=`pwd`
mkdir -p ${INSTALL_DIR}

rm -f *.o
rm -f objdump*txt
rm -f ${LIB_NAME}.a
rm -f ${INSTALL_DIR}/${LIB_NAME}.a

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
for obj in ${C_SRC};do
    Build_OBJ "${obj}"
done

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
echo "    ${BUILD_PATH}/${INSTALL_DIR}"
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
