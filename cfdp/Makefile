OBJ_DIR=build
SRC_DIR=.
BP_DIR=../bp
BP_OBJ_DIR=../bp/build

all: $(OBJ_DIR) CNAK

CNAK: CNAK.c
	gcc -g -o CNAK CNAK.c encape.c NAK_thread.c recv_thread.c \
		primitive.c rate.c -lrt -lpthread $(BP_OBJ_DIR)/bp.o \
		$(BP_OBJ_DIR)/uri.o 

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


clean: 
	rm -rf $(OBJ_DIR); \
	rm -f send recv CNAK

