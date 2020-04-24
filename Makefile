#CC := /usr/local/bin/gcc
#GCOV := /usr/local/bin/gcov

EXECUTABLE=cmock_test
ROOT_DIR ?= $(shell pwd)

CONFIGURATION_DIR ?= ${ROOT_DIR}/tools/configuration

3RDPARTY ?= ${ROOT_DIR}/tools/3rdparty

CMOCK_DIR ?= ${3RDPARTY}/CMock
CMOCK_SRC_DIR ?= ${CMOCK_DIR}/src
CMOCK_INCLUDE_DIR ?= ${CMOCK_SRC_DIR}
CMOCK_LIB_DIR ?= ${CMOCK_DIR}/lib

UNITY_DIR ?= ${CMOCK_DIR}/vendor/unity
UNITY_SRC_DIR ?= ${CMOCK_DIR}/vendor/unity/src
UNITY_INCLUDE_DIR ?= ${UNITY_SRC_DIR}
UNITY_BIN_DIR ?= ${UNITY_DIR}/auto

BUILD_DIR ?= ${ROOT_DIR}/build
LIB_DIR ?= ${ROOT_DIR}/build/lib
BIN_DIR ?= ${ROOT_DIR}/build/bin
MOCKS_DIR ?= ${ROOT_DIR}/build/mocks
GEN_DIR ?= ${ROOT_DIR}/build/generated

SRC_DIR ?= ${ROOT_DIR}/src

FREE_RTOS_DIR ?= ${SRC_DIR}/FreeRTOS
LWIP_DIR ?= ${SRC_DIR}/lwip
TEST_DIR ?= test

INCLUDE_DIR ?= -I ${CMOCK_INCLUDE_DIR} -I ${UNITY_INCLUDE_DIR} -I ${FREE_RTOS_DIR} -I ${MOCKS_DIR} -I ${LWIP_DIR}
CODE_COVERAGE ?= -fprofile-arcs -ftest-coverage -fprofile-generate

.PHONY: all clean directories mocks coverage run

all:${LIB_DIR}/libcmock.so  ${LIB_DIR}/libunity.so directories mocks ${BIN_DIR}/${EXECUTABLE} ${LIB_DIR}/libsys_arch.so

directories:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${MOCKS_DIR}
	mkdir -p ${LIB_DIR}
	mkdir -p ${BIN_DIR}
	mkdir -p ${GEN_DIR}

mocks: directories
	cd ${BUILD_DIR} && ruby  ${CMOCK_LIB_DIR}/cmock.rb -o${CONFIGURATION_DIR}/project.yml ${FREE_RTOS_DIR}/task.h ${FREE_RTOS_DIR}/queue.h

${LIB_DIR}/libcmock.so: ${CMOCK_SRC_DIR}/cmock.c ${CMOCK_SRC_DIR}/cmock.h Makefile ${LIB_DIR}/libunity.so
	${CC} -o $@ -shared -fPIC  $<  ${INCLUDE_DIR}

${LIB_DIR}/libunity.so: ${UNITY_SRC_DIR}/unity.c ${CMOCK_SRC_DIR}/cmock.h Makefile directories
	${CC} -o $@ -shared -fPIC  $< -I ${UNITY_INCLUDE_DIR}

${LIB_DIR}/mock_queue.o : ${MOCKS_DIR}/mock_queue.c mocks Makefile
	${CC} -c $< -o $@  ${INCLUDE_DIR} -fPIC

${LIB_DIR}/mock_task.o :  ${MOCKS_DIR}/mock_task.c mocks Makefile
	${CC} -c $< -o $@ ${INCLUDE_DIR} -fPIC

${LIB_DIR}/libsys_arch.so: ${LWIP_DIR}/sys_arch.c ${LIB_DIR}/mock_task.o ${LIB_DIR}/mock_queue.o
	${CC} -o $@  -shared -fPIC $+  ${INCLUDE_DIR}  ${CODE_COVERAGE}  -lgcov

${GEN_DIR}/${EXECUTABLE}_test_runner.c : mocks Makefile
	ruby ${UNITY_BIN_DIR}/generate_test_runner.rb   ${CONFIGURATION_DIR}/project.yml ${TEST_DIR}/lwip_test.c ${GEN_DIR}/${EXECUTABLE}_test_runner.c

${BIN_DIR}/${EXECUTABLE}: ${GEN_DIR}/${EXECUTABLE}_test_runner.c ${TEST_DIR}/lwip_test.c ${LIB_DIR}/libsys_arch.so
	${CC} -o $@  $+  ${INCLUDE_DIR} -L ${LIB_DIR} -Wl,-rpath,${LIB_DIR} -lunity -lcmock -lsys_arch

clean:
	-rm -rf build
	-rm sys_arch.gcda  sys_arch.gcno
run:
	${BIN_DIR}/${EXECUTABLE}

coverage: run
	lcov --base-directory . --directory . -c --rc lcov_branch_coverage=1 --rc genhtml_branch_coverage=1  -o build/cmock_test.info
	genhtml build/cmock_test.info --branch-coverage --output-directory build/coverage_html
#lcov --remove cmock_test.info "/usr*" --o cmock_test.info 
