##########################
# Used for project to build pure c libvec.a
# Input:
#  $1 : compile name, (riscv32-elf-gcc, riscv32-elf-clang, riscv64-elf-gcc,
#                       riscv64-elf-clang, gcc, arm-none-eabi-gcc)
#  $2 : Extra compile flags, default for "-O3" and some libdsp default defined
#
# Ex:
#    ./build_lib.sh "riscv32-elf-gcc" "-O0"
####################################
COMPILER_NAME="$1"
PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`
EXTRA_FLAGS="$2"

#Output setting:
BUILD_DIR="build_dir"
LIB_ROOT=`pwd`
BUILD_LOG="$LIB_ROOT/build_lib.log"
rm -rf ${BUILD_LOG}

function log () {
    echo "$*" | tee -a ${BUILD_LOG}
}

log "=================================================="
which $COMPILER_NAME > /dev/null
if [ "$?" != 0 ];then
    log "Input 1: \"$COMPILER_NAME\" not found !!"
    log "please check your PATH !!"
    exit
else
    log "Input 1 (COMPILER_NAME) = $COMPILER_NAME"
    if [ "$EXTRA_FLAGS" != "" ];then
        log "Input 2 (EXTRA_FLAGS) = $EXTRA_FLAGS"
    fi
fi
log "=================================================="

CC="${COMPILER_NAME}"
AR="${PREFIX}-ar"

INC_PATH="-I${LIB_ROOT}/include -I${LIB_ROOT}/internal"
AR_FLAG="crD"
BUILD_FLAG="-O3 ${INC_PATH}"

BUILD_FLAG="${BUILD_FLAG} -ffunction-sections -fdata-sections -Wall -Werror ${EXTRA_FLAGS}"

#$1: sub-folder name
#$2: filename (w/o ext. name)
function Build_OBJ () {
    SRC_NAME=`echo ${1}| cut -d '/' -f2`
    local SRC="${LIB_ROOT}/${1}.c"
    local OBJ="${SRC_NAME}.o"

    echo "Compiling ..." >> ${BUILD_LOG}
    echo "    In:   ${SRC}" >> ${BUILD_LOG}
    echo "    Out:  ${OBJ}" >> ${BUILD_LOG}
    echo "${CC} ${BUILD_FLAG} -c ${SRC} -o ${OBJ}" >> ${BUILD_LOG}
    ${CC} ${BUILD_FLAG} -c ${SRC} -o ${OBJ}
    ret=$?
    if [ "$ret" != "0" ];then
        echo "${SRC} fail"
        exit
    fi
}

#####main from here#####
LIB_NAME="libvec"
if [[ $EXTRA_FLAGS =~ .*ENA_VEC_ISA.* ]]; then
	ENA_FP16=1
fi
source ${LIB_ROOT}/source_file.list

# get branch nds_version.c
cd ${LIB_ROOT}

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
BUILD_PATH=`pwd`

rm -f *.o ${LIB_NAME}.a

#functions:
for obj in ${C_SRC};do
    Build_OBJ "${obj}"
done

#library:
ALL_OBJ=`ls *.o`
${AR} ${AR_FLAG} ${LIB_NAME}.a ${ALL_OBJ}

log "=================================================="
log "build libvec.a sucess"
log "    lib: ${BUILD_DIR}/libvec.a"
log "    build log: build_lib.log"
log "=================================================="
cd ${LIB_ROOT}
log "Demo: riscv_vec_add_f32"
log "    ${CC} ${BUILD_FLAG} -I./include/ -o demo.adx demo.c ${BUILD_DIR}/libvec.a"
${CC} ${BUILD_FLAG} -I./include/ -o demo.adx demo.c ${BUILD_DIR}/libvec.a
log "=================================================="
