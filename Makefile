ifndef CONTIKI
  ${error CONTIKI not defined! Do not forget to source /upb/groups/fg-ccs/public/share/nes/env.sh}
endif

ifndef TARGET
TARGET=sky
endif

all: hello.sky

upload: hello.upload

sim: broadcast-example.csc broadcast-example.c
	java -jar $(CONTIKI)/tools/cooja/dist/cooja.jar -quickstart=broadcast-example.csc -contiki=$(CONTIKI)
simulation:
	java -jar $(CONTIKI)/tools/cooja/dist/cooja.jar -contiki=$(CONTIKI)

CONTIKI_WITH_IPV4 = 1
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
