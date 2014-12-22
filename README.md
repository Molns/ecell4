# E-Cell System version 4 

## What is E-Cell System?

E-Cell System, a software platform for modeling, simulation and analysis of complex, heterogeneous and multi-scale systems like the cell.

## Installing E-Cell (Windows)

### Requirements

- Python 2.7.8(**32bit**) https://www.python.org/ftp/python/2.7.8/python-2.7.8.msi
- virtualenv https://pypi.python.org/packages/source/v/virtualenv/virtualenv-1.11.6.tar.gz
- HDF5-1.8.13 Pre-built Binary(**32-bit**) http://www.hdfgroup.org/ftp/HDF5/current/bin/windows/hdf5-1.8.13-win32-VS2012-shared.zip

```
:: creating virtualenv environment
cd C:\path\to\virtualenv-1.11.6 
python virtualenv.py C:\path\to\myenv
cd C:\path\to\myenv\Scripts
activate.bat
:: now you are in virtual environment
pip install http://dev.e-cell.org/downloads/ecell4/win/latest/ecell4-0.0.0-cp27-none-win32.whl
```

## Installing E-Cell (Mac OS X)

### Requirements

- hdf5 (latest)
- pip (latest)

```shell
# here we use homebrew to install hdf5, please install hdf5 to /usr/local/lib
brew install homebrew/science/hdf5 --enable-cxx
sudo easy_install pip
pip install http://dev.e-cell.org/downloads/ecell4/mac/mavericks/latest/ecell4-0.0.0-cp27-none-macosx_10_9_intel.whl
# if you are using Mountain Lion
# pip install http://dev.e-cell.org/downloads/ecell4/mac/mountainlion/latest/ecell4-0.0.0-cp27-none-macosx_10_8_intel.whl
```

## Installing E-Cell (Ubuntu 14.04)

```shell
$ sudo apt-get install python-pip libgsl0-dev libhdf5-serial-dev libboost-dev
$ sudo pip install http://dev.e-cell.org/downloads/ecell4/ubuntu-trusty-amd64/latest/ecell4-0.0.0-cp27-none-linux_x86_64.whl
```

## Building and installing E-Cell (Ubuntu 14.04)

```shell
# dependent packages
$ sudo apt-get install libgsl0-dev libboost-dev libboost-test-dev libboost-regex-dev libhdf5-serial-dev
$ sudo apt-get instal python-dev cython

$ wget https://github.com/ecell/ecell4/archive/master.zip   
$ unzip master.zip
$ cd ecell4-master
$ PREFIX=/path/to PYTHONPATH=/path/to/lib/python2.7/site-packages ./install.sh
```

## How to use?

```
$ LD_LIBRARY_PATH=/pat/to/lib PYTHONPATH=/path/to/lib/python2.7/site-packages python
Python 2.7.6 (default, Mar 22 2014, 22:59:56) 
[GCC 4.8.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> from ecell4.core import *
>>> sp = Species("B.A.C")
>>> print sp.serial()
A.B.C
>>> 
```

## Running E-Cell4 with IPython notebook (by docker)

If you use following docker images, you don't need to do OS dependent installation.  
We have already installed E-Cell4 to docker environment.

### boot2docker (Windows or Mac)

The latest version of boot2docker sets up a host only network adaptor which provides access to the container's ports.

```shell
$ boot2docker ssh
######## in boot2docker
docker@boot2docker:~$ docker pull ecell/ecell4:develop
docker@boot2docker:~$ docker run --rm -i -t -p 8888:8888 ecell/ecell4:develop
```

Then you should be able to access the E-Cell4 IPython notebook server using the IP address reported to you using:

```shell
$ boot2docker ip
```

Typically, it is 192.168.59.103, so please open 192.168.59.103:8888 with your favorite browser.
(But it could get changed by Virtualbox's DHCP implementation.)

### Docker (Linux)

```shell
$ sudo docker pull ecell4/ecell4:develop
$ sudo docker run --rm -i -t -p 8888:8888 ecell/ecell4:develop
```

Open localhost:8888 with your favorite browser.


[![Build Status](https://travis-ci.org/ecell/ecell4.svg?branch=master)](https://travis-ci.org/ecell/ecell4)


## IPython notebooks (tutorials) for E-Cell4

Please see http://nbviewer.ipython.org/github/ecell/ecell4/blob/develop/ipynb/index.ipynb
