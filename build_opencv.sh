#!/usr/bin/env bash
SRC=`pwd`/3rdlib_src
DST=`pwd`/3rdlibs
if ! [ -d ${SRC} ]; then
  mkdir -p ${SRC}
fi

# opencv
cd ${SRC}
git clone https://github.com/opencv/opencv.git opencv -b 3.4.12 --depth=1
cd opencv
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=./install -DWITH_IPP=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_DOCS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_apps=OFF -DWITH_ITT=OFF -DWITH_JPEG=ON -DWITH_TIFF=OFF -DWITH_JASPER=OFF -DWITH_PNG=OFF -DWITH_OPENEXR=OFF -DWITH_WEBP=OFF
make -j8
make install

mkdir -p ${DST}
if [ -d ${DST}/opencv ]; then
  rm -rf ${DST}/opencv
fi
mv install ${DST}/opencv

