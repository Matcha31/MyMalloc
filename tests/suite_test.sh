#!/bin/sh

cd $(dirname "$0")
cd ..

test_malloc ()
{
    arg=$1
    a=$($arg)
    ref=$?
    b=$(LD_PRELOAD=./libmalloc.so $arg)
    tes=$?
    if [ $tes -eq $ref ]; then
        echo SUCCESS with args $arg
    else
        echo FAILED with args $arg
    fi
}

test_malloc "ls"
test_malloc "ls -a"
test_malloc "cd ./"
test_malloc "find ./"
test_malloc "factor 20 30 40 50 60 70 80 90"
test_malloc "cat Makefile"
test_malloc "ip a"
test_malloc "tar -cf malloc.tar libmalloc.so"
tets_malloc "tree"
test_malloc "od libmalloc.so"
test_malloc "git status"
test_malloc "less ./Makefile"
