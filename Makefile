.PHONY: all clean
APP_1:=stream
APP_2:=stream_playback

SRC_DIR:=.
CAP_SRCS:=$(APP_1).c
PLAY_SRCS:=$(APP_2).c

C_DIR:=../common
C_SRCS:=an-utils.c

STREAM_DIR:=../ics-session
STREAM_SRCS:=ics-stream.c

Q_DIR:=../concurrent_queue
Q_SRCS:=queue.c

ICS_DIR:=../ics
ICS_SRCS:=ics.c ics-event.c ics-command.c

O_DIR:=../object-pool
O_SRCS:=object-pool.c


CFLAGS:=$(shell pkg-config --cflags libpjproject) -Werror
CFLAGS+= -I$(STREAM_DIR)/include
CFLAGS+= -I$(C_DIR)/include
CFLAGS+=-I$(ICS_DIR)/include -I$(Q_DIR)/include -I$(O_DIR)/include
LIBS:=$(shell pkg-config --libs libpjproject)

all: $(APP_1) $(APP_2)

$(APP_1): $(CAP_SRCS:.c=.o) $(STREAM_SRCS:.c=.o) $(ICS_SRCS:.c=.o) $(Q_SRCS:.c=.o) $(O_SRCS:.c=.o)
	gcc -o $@ $^ $(LIBS)

$(APP_2): $(PLAY_SRCS:.c=.o) $(STREAM_SRCS:.c=.o)
	gcc -o $@ $^ $(LIBS)

$(Q_SRCS:.c=.o): %.o: $(Q_DIR)/src/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(O_SRCS:.c=.o): %.o: $(O_DIR)/src/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(ICS_SRCS:.c=.o): %.o: $(ICS_DIR)/src/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(STREAM_SRCS:.c=.o): %.o: $(STREAM_DIR)/src/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(CAP_SRCS:.c=.o): %.o: $(SRC_DIR)/test/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(PLAY_SRCS:.c=.o): %.o: $(SRC_DIR)/test/%.c
	gcc -c -o $@ $< $(CFLAGS)

$(C_SRCS:.c=.o): %.o: $(C_DIR)/src/%.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -fr *.o $(APP_1) $(APP_2)

