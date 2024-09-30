#!/bin/bash
pwd=`pwd`
mkdir libs
tar -zxvf libgpg-error-1.46.tar.gz
tar -zxvf libgcrypt-1.10.0.tar.gz
cd libgpg-error-1.46
./configure --prefix="${pwd}/libs" && make && make install
cd ../libgcrypt-1.10.0
./configure --prefix="${pwd}/libs" CFLAGS="-I${pwd}/libs/include" LDFLAGS="-L${pwd}/libs/lib" && make && make install
cd ..
export LIBRARY_PATH=$LIBRARY_PATH:${pwd}/libs/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${pwd}/libs/lib
export LIBDIR=$LIBDIR:${pwd}/libs/lib
export LD_RUN_PATH=$LD_RUN_PATH:${pwd}/libs/lib
make EXTRAS="-I${pwd}/libs/include -L${pwd}/libs/lib"
