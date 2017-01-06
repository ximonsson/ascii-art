BIN=bin
BUILD=build
SRCDIR=src
SRC=main.c ascii_art.c fonts.c
OBJ=$(addprefix $(BUILD)/, $(SRC:.c=.o))
EXEC=$(BIN)/ascii

CFLAGS += -Wall -g3

LDFLAGS +=

INCLUDES += -I./include\
 		  $(shell freetype-config --cflags)

LIBS += -L./libs\
	    $(shell freetype-config --libs)\
		-lm


$(BUILD)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(EXEC): $(OBJ)
	@mkdir -p $(BIN)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

all: $(EXEC)

clean:
	rm -rf $(BUILD) $(BIN)
