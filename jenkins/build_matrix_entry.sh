#!/usr/bin/env bash
# This script is executed on Jenkins using
#
#     $WORKSPACE/jenkins/build_matrix_entry.sh <compiler> <build_type>
#
# The exit code determines if the test succeeded or failed.
# Note that the environment variable WORKSPACE must be set (Jenkins
# will do this automatically).

# Exit on error
set -e
# Echo each command
set -x

compiler=$1
build_type=$2

echo "inside build_matrix entry"
# special case for README builds
if [[ $build_type == "readme" ]]; then
  python2 $WORKSPACE/jenkins/parseREADME.py $WORKSPACE/README.md $WORKSPACE
  exit
fi

# set modules and install paths

jali_version=1.0.0
lapack_version=3.8.0

export NGC=/usr/local/codes/ngc
ngc_include_dir=$NGC/private/include

# compiler-specific settings
if [[ $compiler == "intel" ]]; then
  intel_version=18.0.1
  cxxmodule=intel/${intel_version}
  # openmpi version that libs were built against
  openmpi_version=2.1.2
  # openmpi module for compiling and linking
  mpi_module=openmpi/2.1.2
  jali_install_dir=$NGC/private/jali/${jali_version}-intel-${intel_version}-openmpi-${openmpi_version}
  lapacke_dir=$NGC/private/lapack/${lapack_version}-patched-intel-${intel_version}
elif [[ $compiler == "gcc" ]]; then
  gcc_version=6.4.0
  cxxmodule=gcc/${gcc_version}
  # openmpi version that libs were built against
  openmpi_version=2.1.2
  # openmpi module for compiling and linking
  mpi_module=openmpi/2.1.2
  jali_install_dir=$NGC/private/jali/${jali_version}-gcc-${gcc_version}-openmpi-${openmpi_version}
  lapacke_dir=$NGC/private/lapack/${lapack_version}-patched-gcc-${gcc_version}
fi

# build-type-specific settings
cmake_build_type=Release
extra_flags=
if [[ $build_type == "debug" ]]; then
  cmake_build_type=Debug
fi

export SHELL=/bin/sh

export MODULEPATH=""
. /opt/local/packages/Modules/default/init/sh
module load $cxxmodule
module load ${mpi_module}
module load cmake # 3.0 or higher is required

echo $WORKSPACE
cd $WORKSPACE

mkdir build
cd build

cmake \
    -D CMAKE_BUILD_TYPE=$cmake_build_type \
    -D ENABLE_UNIT_TESTS=True \
    -D ENABLE_APP_TESTS=True \
    -D ENABLE_MPI=True \
    -D ENABLE_JENKINS_OUTPUT=True \
    -D NGC_INCLUDE_DIR:FILEPATH=$ngc_include_dir \
    -D Jali_DIR:FILEPATH=$jali_install_dir/lib \
    -D LAPACKE_DIR:FILEPATH=$lapacke_dir \
    ..
make -j2
ctest --output-on-failure
