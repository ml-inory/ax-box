#!/usr/bin/env bash
SRC=`pwd`/3rdlib_src
DST=`pwd`/3rdlibs
if ! [ -d ${SRC} ]; then
  mkdir -p ${SRC}
fi

# jsoncpp
cd ${SRC}
git clone https://github.com/open-source-parsers/jsoncpp.git jsoncpp -b 1.9.5 --depth=1
cd jsoncpp
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=./install -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DJSONCPP_WITH_EXAMPLE=OFF -DBUILD_SHARED_LIBS=OFF
make -j8
make install

mkdir -p ${DST}
if [ -d ${DST}/jsoncpp ]; then
  rm -rf ${DST}/jsoncpp
fi
mv install ${DST}/jsoncpp

