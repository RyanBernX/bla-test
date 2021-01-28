CC=gcc
CXX=g++
MATLAB_ROOT ?= /opt/MATLAB/R2020b
CXXOPTFLAGS=-march=native -O3 -DNDEBUG
CXXFLAGS=${CXXOPTFLAGS} -I../include -isystem /usr/include/eigen3 -m64 -fopenmp
MKLLIBS=-L${MKLROOT}/lib/intel64 -lmkl_rt -Wl,--no-as-needed -lpthread -lm -ldl
#MKLLIBS= -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl
BLASLIBS=-L${OPENBLAS_DIR}/lib -lopenblas
MATLAB_CXXFLAGS=-I${MATLAB_ROOT}/extern/include -DMATLAB_DEFAULT_RELEASE=R2017b -DMATLAB_MEX_FILE
MATLAB_LIBS=-Wl,-rpath,${MATLAB_ROOT}/bin/glnxa64 -L${MATLAB_ROOT}/bin/glnxa64 -lmex -lmx -lmat

.PHONY: all clean

all: bla_native bla_mkl bla_oblas bla_mkl_direct bla_oblas_direct bla_mkl_direct_c readmat spbla_test

matlab.o: matlab.cpp
	${CXX} -c -o $@ ${CXXFLAGS} ${MATLAB_CXXFLAGS} $^

bla_native: blas_test.cpp
	${CXX} -o $@ ${CXXFLAGS} $^

bla_mkl: blas_test.cpp
	${CXX} -o $@ -DUSE_MKL ${CXXFLAGS} $^ ${MKLLIBS}

bla_mkl_direct: blas_test.cpp
	${CXX} -o $@ -DUSE_MKL -DUSE_BLAS_DIRECT -I${MKLROOT}/include ${CXXFLAGS} $^ ${MKLLIBS}

bla_oblas: blas_test.cpp
	${CXX} -o $@ -DUSE_BLAS_LAPACKE ${CXXFLAGS} $^ ${BLASLIBS}

bla_oblas_direct: blas_test.cpp
	${CXX} -o $@ -DUSE_BLAS_LAPACKE -DUSE_BLAS_DIRECT -I${OPENBLAS_DIR}/include ${CXXFLAGS} $^ ${BLASLIBS}

bla_mkl_direct_c: blas_test.c
	${CC} -o $@ -DUSE_MKL_MALLOC -I${MKLROOT}/include ${CXXFLAGS} $^ ${MKLLIBS} -liomp5

readmat: readmat.cpp matlab.o
	${CXX} -o $@ $^ ${CXXFLAGS} ${MATLAB_CXXFLAGS} ${MATLAB_LIBS}
	
spbla_test: spblas_test.cpp matlab.o
	${CXX} -o $@ $^ ${CXXFLAGS} -I${MKLROOT}/include ${MATLAB_CXXFLAGS} ${MKLLIBS} ${MATLAB_LIBS}

clean:
	-rm -f *.o bla_native bla_mkl bla_oblas bla_mkl_direct bla_oblas_direct bla_mkl_direct_c \
		readmat spbla_test
