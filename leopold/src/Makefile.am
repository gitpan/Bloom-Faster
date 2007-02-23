lib_LTLIBRARIES = libbloom.la
libbloom_la_SOURCES = bloom.h hashes.h bloom.c jenkins.c suggestions.c
libbloom_la_LDFLAGS = -lm
libbloom_la_CFLAGS = -Wall -pedantic

bin_PROGRAMS = bloom bloom_longtest
bloom_SOURCES = bloom.h main.c
bloom_longtest_SOURCES = bloom.h longtest.c
bloom_LDFLAGS = -lbloom
bloom_longtest_LDFLAGS = -lbloom
bloom_CFLAGS = -Wall -pedantic

