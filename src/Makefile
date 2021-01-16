CXX=g++
CXXFLAGS=-O3 -I/usr/include/eigen3 -I../include -march=native -m64
MKLLIBS=-Wl,--no-as-needed -L${MKLROOT}/lib/intel64 -lmkl_rt -lpthread -lm -ldl
BLASLIBS=-llapack -lopenblas

.PHONY: all clean

all: bla_native bla_mkl bla_oblas

bla_native: blas_test.cpp
	${CXX} -o $@ ${CXXFLAGS} $^

bla_mkl: blas_test.cpp
	${CXX} -o $@ -DUSE_MKL ${CXXFLAGS} $^ ${MKLLIBS}

bla_oblas: blas_test.cpp
	${CXX} -o $@ -DUSE_BLAS_LAPACKE ${CXXFLAGS} $^ ${BLASLIBS}

clean:
	-rm -f *.o bla_native bla_mkl bla_oblas
