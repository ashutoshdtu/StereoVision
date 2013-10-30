DRWN_PATH := /home/ashutosh/Documents/darwin.1.6.1

#######################################################################
# define project parameters here
#######################################################################

PROJ_NAME = StereoVision
PROJ_MAJORVER = 1
PROJ_MINORVER = 0.0

APP_SRC = stereoVision.cpp
APP_BIN = stereoVision

-include $(shell pwd)/make.mk

# maxflow library
ifeq ($(wildcard $(EXT_PATH)/maxflow-v3.01.src), $(EXT_PATH)/maxflow-v3.01.src)
  CFLAGS += -I$(EXT_PATH)/maxflow-v3.01.src -DUSE_EXTERNAL_MAXFLOW
  LFLAGS += $(EXT_PATH)/maxflow-v3.01.src/maxflow.a \
	$(EXT_PATH)/maxflow-v3.01.src/maxflow.o \
	$(EXT_PATH)/maxflow-v3.01.src/graph.o
endif

#######################################################################

PROJ_VER = ${PROJ_MAJORVER}.${PROJ_MINORVER}
APP_SRC_DIR = ${shell pwd}
APP_BIN_DIR = ${shell pwd}/bin

#######################################################################

main:
	${CCC} ${APP_SRC_DIR}/${APP_SRC} -o ${APP_BIN_DIR}/${APP_BIN} ${CFLAGS} ${LFLAGS} 
