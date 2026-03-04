#!/usr/bin/env bash

CLASS="${1:-*}"

LOCK_FILE=".lock"
if [[ -f "${LOCK_FILE}" ]]; then exit 0 ; fi
touch ${LOCK_FILE}

CORES=$(( $( grep -c 'processor' /proc/cpuinfo ) - 1 ))

export CC="riscv64-linux-gnu-gcc-14"
export CXX="riscv64-linux-gnu-g++-14"

mkdir -p .build

ELF_DIR="$( find ".build" -type d -name 'luajit.elf.dir' | head -n 1 )"
if [[ -d "${ELF_DIR}" ]]; then
	find "${ELF_DIR}" -name "${CLASS}.*.o" -delete
fi

pushd .build 1>/dev/null || exit
cmake .. -G Ninja -DCMAKE_COLOR_DIAGNOSTICS=ON -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSTRIPPED=OFF 
cmake --build . -j "${CORES}"
BUILD_EXIT_CODE=$?
popd 1>/dev/null || exit

rm -f ${LOCK_FILE}
exit $BUILD_EXIT_CODE
