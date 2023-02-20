# libvec
Andes Vector Library

Open source for Andes Vector library

- User manual:

   * docs/html/index.html

- How to build libvec.a

  - execute the script "build_purec_lib.sh", then "libvec.a" will be compiled in the "build_dir/" folder

     $ ./build_purec_lib.sh ${COMPILER_NAME}
       - $1: COMPILER_NAME (ex: riscv32-elf-gcc, riscv64-elf-gcc, gcc, riscv32-elf-clang, riscv64-elf-clang , ...)

     Ex: ./build_purec_lib.sh "riscv32-elf-gcc"  ## for Andes toolchain <br>
     Ex: ./build_purec_lib.sh "gcc"              ## for x86 toolchain

- How to link libvec.a

  - Here is a demo.c for function "riscv_vec_add_f32"

     $ riscv32-elf-gcc -I./include -o demo.adx demo.c build_dir/libvec.a  ## for Andes toolchain <br>
     $ gcc -I./include -o demo.adx demo.c build_dir/libvec.a              ## for x86 toolchain
