cmake_minimum_required(VERSION 2.8)

# 交叉编译
SET (CMAKE_SYSTEM_NAME              Linux)
SET (CMAKE_SYSTEM_PROCESSOR         armv61)
SET (ARCH_NAME                      hisiv200)
SET (CMAKE_C_COMPILER               arm-hisiv200-linux-gcc)
SET (CMAKE_CXX_COMPILER             arm-hisiv200-linux-g++)
SET (CMAKE_AR                       arm-hisiv200-linux-ar CACHE FILEPATH "Archiver") #需要加上 CACHE FILEPATH "Archiver"， 才能被识别到，下面几个还未测试，估计也要加上
SET (CMAKE_LINKER                   arm-hisiv200-linux-ld)
SET (CMAKE_NM                       arm-hisiv200-linux-nm)
SET (CMAKE_OBJDUMP                  arm-hisiv200-linux-objdump)
SET (CMAKE_RANLIB                   arm-hisiv200-linux-ranlib)
SET (CMAKE_C_FLAGS                  "${CMAKE_C_FLAGS} -std=gnu99")