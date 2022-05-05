#!/usr/bin/env bash

lib_need_copy=(
    ../libSrc/basic.json
    ../libSrc/transform.json
    ../libSrc/filters.json
)

force_lib=(
    libfftw3f-3.dll
)

build_dir=../build
lib_dir=../build/lib


if [ ! -d $lib_dir ]; then
    mkdir $lib_dir
fi

if [ ! ${#force_lib[@]} -eq 0 ]; then
    cp ${force_lib[@]} $build_dir
fi

if [ ! ${#lib_need_copy[@]} -eq 0 ]; then
    cp ${lib_need_copy[@]} $lib_dir
fi

