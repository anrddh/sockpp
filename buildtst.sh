#!/bin/bash
#
# Test the build with a few compilers.
#

for COMPILER in g++-5 g++-6 g++-7 g++-8 g++-9 clang++-4.0 clang++-5.0 clang++-6.0
do
  if [ -z "$(which ${COMPILER})" ]; then
    printf "Compiler not found: %s\n" "${COMPILER}"
  else
    printf "Testing: %s\n" "${COMPILER}"
    rm -rf build/
    mkdir build ; pushd build
    if ! CXX=${COMPILER} cmake -DSOCKPP_BUILD_EXAMPLES=ON .. ; then
      printf "\nCMake configuration failed for %s\n" "${COMPILER}"
	  exit 1
    fi
    if ! make -j8 ; then
	  printf "\nCompilation failed for %s\n" "${COMPILER}"
	  exit 1
	fi
    popd
  fi
  printf "\n"
done

rm -rf build/*
exit 0


