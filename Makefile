NVCC = nvcc
NVCCFLAGS += -O3 -std=c++14 --compiler-options -Wall,-Wextra
INC = `pkg-config --cflags libgcrypt`
LIBS = `pkg-config --libs libgcrypt` -lgcrypt -lnvrtc -lcuda

.PHONY: all clean

all: shader1

key_test_sha1.o: key_test_sha1.cu
	$(NVCC) -c -o $@ $(NVCCFLAGS) $(INC) $(EXTRAS) $^

key_test_pattern.o: key_test_pattern.cpp
	$(NVCC) -c -o $@ $(NVCCFLAGS) $(INC) $(EXTRAS) $^

key_test.o: key_test.cpp
	$(NVCC) -c -o $@ $(NVCCFLAGS) $(INC) $(EXTRAS) $^

gpg_helper.o: gpg_helper.cpp
	$(NVCC) -c -o $@ $(NVCCFLAGS) $(INC) $(EXTRAS) $^

shader1: main.cpp key_test.o key_test_sha1.o key_test_pattern.o gpg_helper.o
	$(NVCC) -o $@ $(NVCCFLAGS) $(LIBS) $(INC) $(EXTRAS) $^

clean:
	-rm -f *.o shader1
