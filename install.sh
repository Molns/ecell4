#!/bin/bash -x

CURDIR=$(cd $(dirname $0); pwd)
# PREFIX=/usr/local
# PREFIX=${HOME}/local
# PREFIX=${CURDIR}/local
# PREFIX=

# make clean; rm -rf ${PREFIX}; rm -rf python/build python/lib/ecell4/*.cpp; rm CMakeCache.txt
# rm -rf python/build python/lib/ecell4/*.cpp
cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} .
make
make test
make install

cd python
mkdir -p ${PREFIX}/lib/python2.7/site-packages
LD_LIBRARY_PATH=${PREFIX}/lib PYTHONPATH=${PREFIX}/lib/python2.7/site-packages:/usr/local/lib/python2.7/dist-packages:${PYTHONPATH} python setup.py build_ext -L${PREFIX}/lib -I${PREFIX}/include install --prefix=${PREFIX}
PYTHONPATH=${PREFIX}/lib/python2.7/site-packages:/usr/local/lib/python2.7/dist-packages:${PYTHONPATH} LD_LIBRARY_PATH=${PREFIX}/lib python setup.py test
