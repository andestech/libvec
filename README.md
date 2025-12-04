# Andes Vector library

Andes Vector Library provides comprehensive functions include basic operations for vectors or matrices,
functions that compute more complicated algorithms such as filtering or image resizing, and transform functions.

- Andes Vector Library user manual
    The user manual of Andes Vector Library can be found under docs/html.
- Prebuilt library
    The prebuilt library compiled with x86_64 GCC is available in the `prebuilt/` folder.
- For any toolchains
    - Steps to build the Andes Vector Library
        Run ***build_purec_lib.sh*** with the compiler name as the argument. For example:

        `./build_purec_lib.sh "riscv32-unknown-elf-gcc"`

        Once the build is complete, a static library named ***libvec.a*** will be generated under ***build_dir*** folder. For more usage details, please refer to the comments at the beginning of ***build_purec_lib.sh***.
    - Steps to build the example
        1. Navigate to *example* folder
        2. Build the example with the following command (assumes *riscv32-unknown-elf-gcc* or "gcc" as the compiler):

           `riscv32-elf-gcc -I./../include -o demo.adx demo.c ../build_dir/libvec.a`

           `gcc  -I./../include/ -o demo.adx demo.c ../build_dir/libvec.a`
