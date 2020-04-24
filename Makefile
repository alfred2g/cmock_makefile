ROOT_DIR ?= $(shell pwd)

3RDPARTY ?= tools/3rdparty

CMOCK_DIR ?= ${3RDPARTY}/CMock
CMOCK_SRC_DIR ?= ${CMOCK_DIR}/src
CMOCK_INCLUDE_DIR ?= ${CMOCK_SRC_DIR}
CMOCK_LIB_DIR ?= ${CMOCK_DIR}/lib

UNITY_DIR ?= ${CMOCK_DIR}/vendor/unity
UNITY_SRC_DIR ?= ${CMOCK_DIR}/vendor/unity/src
UNITY_INCLUDE_DIR ?= ${UNITY_SRC_DIR}

BUILD_DIR ?= build
LIB_DIR ?= build/lib
BIN_DIR ?= build/bin
MOCKS_DIR ?= build/mocks
SRC_DIR ?= src
FREE_RTOS_DIR ?= ${SRC_DIR}/FreeRTOS
LWIP_DIR ?= ${SRC_DIR}/lwip
TEST_DIR ?= test

INCLUDE_DIR ?= -I ${CMOCK_INCLUDE_DIR} -I ${UNITY_INCLUDE_DIR}

.PHONY: all ${LIB_DIR}/cmock.so  ${LIB_DIR}/unity.so clean directories

all:${LIB_DIR}/cmock.so  ${LIB_DIR}/unity.so directories mocks

directories:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${MOCKS_DIR}
	mkdir -p ${LIB_DIR}
	mkdir -p ${BIN_DIR}

${LIB_DIR}/cmock.so: ${CMOCK_SRC_DIR}/cmock.c ${CMOCK_SRC_DIR}/cmock.h Makefile ${LIB_DIR}/unity.so
	${CC} -o $@ -shared -fPIC  $< -I ${CMOCK_INCLUDE_DIR} -I ${UNITY_INCLUDE_DIR}

${LIB_DIR}/unity.so: ${UNITY_SRC_DIR}/unity.c ${CMOCK_SRC_DIR}/cmock.h Makefile directories
	${CC} -o $@ -shared -fPIC  $< -I ${UNITY_INCLUDE_DIR}

mocks:
	cd build
	ruby ${CMOCK_LIB_DIR}/cmock.rb ${FREE_RTOS_DIR}/task.h
	cd  ${ROOT_DIR}


clean:
	rm -rf build
