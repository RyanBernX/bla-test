CC=icc
CXX=icpc
CXXFLAGS=-O3 -I../include -march=native -m64
MKLLIBS=-Wl,--no-as-needed -L${MKLROOT}/lib/intel64 -lmkl_rt -lpthread -lm -ldl
BLASLIBS=-L${OPENBLAS_DIR}/lib -lopenblas

.PHONY: all clean

all: bla_native bla_mkl bla_oblas bla_mkl_direct bla_oblas_direct bla_mkl_direct_c

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

clean:
	-rm -f *.o bla_native bla_mkl bla_oblas bla_mkl_direct bla_oblas_direct bla_mkl_direct_c
