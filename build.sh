#!/usr/bin/env bash

build_dir=""
cmake_options=""

build()
{
    echo "Build dir: ${build_dir}"
    echo "CMake options: ${cmake_options}"

    if [ ! -d $build_dir ]; then
        mkdir -p $build_dir
    fi

    cd $build_dir

    cmake $cmake_options ../../
    make && make install

    cd ../../
    cp -r data/ bin/
}

clean()
{
    if [ -d build/ ]; then
        echo "Removing build dir"
        rm -rf build/
    fi

    if [ -d bin/ ]; then
        echo "Removing bin dir"
        rm -rf bin/
    fi

    echo "Done"
}

move-compile-commands()
{
    echo "Moving compile_commands.json"
    mv $build_dir/compile_commands.json build/
}

print-usage()
{
    cat << EOF
    Usage:
        -h        Print usage
        -c        Clean (removes build and bin dir)
        -d        Debug build
        -r        Release build
EOF
}

while getopts "hcdr" arg; do
    case $arg in
        c) clean
           exit 0
           ;;
        d) build_dir="build/debug/"
           cmake_options="-DCMAKE_BUILD_TYPE=Debug"
           ;;
        r) build_dir="build/release/"
           cmake_options="-DCMAKE_BUILD_TYPE=Release"
           ;;
        *) print-usage
           exit 1
           ;;
    esac
done

if [ -z $build_dir ]; then
    print-usage
    exit 1
fi

build
move-compile-commands
