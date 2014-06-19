OBJ_DIR=build
SRC_DIR=.

all: $(OBJ_DIR) $(OBJ_DIR)/bpd $(OBJ_DIR)/bp.o send recv \
	$(OBJ_DIR)/libhash.a \
	$(OBJ_DIR)/testhash32 $(OBJ_DIR)/testhash64 \
	$(OBJ_DIR)/testdllhash32 $(OBJ_DIR)/testdllhash64

send: send.c
	gcc -g -o send send.c $(OBJ_DIR)/bp.o \
		$(OBJ_DIR)/uri.o

recv: recv.c
	gcc -g -o recv recv.c $(OBJ_DIR)/bp.o \
		$(OBJ_DIR)/uri.o

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/bpd: bpd.c bp.h $(OBJ_DIR)/bpd_bind_list.o \
	$(OBJ_DIR)/bpd_bundle_list.o\
	$(OBJ_DIR)/bpd_forward_table.o \
	$(OBJ_DIR)/uri.o $(OBJ_DIR)/bundle.o \
	$(OBJ_DIR)/libhash.a
	gcc -g -o $(OBJ_DIR)/bpd bpd.c $(OBJ_DIR)/bpd_bind_list.o \
		$(OBJ_DIR)/bpd_bundle_list.o \
		$(OBJ_DIR)/bpd_forward_table.o \
		$(OBJ_DIR)/uri.o $(OBJ_DIR)/bundle.o \
		$(OBJ_DIR)/libhash.a \
		-lpthread -lrt

$(OBJ_DIR)/bpd_bind_list.o: bpd_bind_list.c \
	bpd_bind_list.h bp.h
	gcc -g -c -o $(OBJ_DIR)/bpd_bind_list.o \
		bpd_bind_list.c

$(OBJ_DIR)/bpd_bundle_list.o: bpd_bundle_list.c bpd_bundle_list.h bp.h
	gcc -g -c -o $(OBJ_DIR)/bpd_bundle_list.o bpd_bundle_list.c

$(OBJ_DIR)/bpd_forward_table.o: bpd_forward_table.c bpd_forward_table.h bp.h
	gcc -g -c -o $(OBJ_DIR)/bpd_forward_table.o bpd_forward_table.c

$(OBJ_DIR)/bp.o: bp.c bp.h
	gcc -g -c -o $(OBJ_DIR)/bp.o bp.c

$(OBJ_DIR)/uri.o: uri.c uri.h
	gcc -g -c -o $(OBJ_DIR)/uri.o uri.c
		
$(OBJ_DIR)/bundle.o: bundle.c bundle.h
	gcc -g -c -o $(OBJ_DIR)/bundle.o bundle.c

$(OBJ_DIR)/hash_32a.o: fnv/hash_32a.c fnv/fnv.h fnv/longlong.h
	gcc -g -c -o $(OBJ_DIR)/hash_32a.o fnv/hash_32a.c

$(OBJ_DIR)/hashtable.o: hashtable.c hashtable.h
	gcc -g -c -o $(OBJ_DIR)/hashtable.o hashtable.c

$(OBJ_DIR)/dll.o: dll.c dll.h
	gcc -g -c -o $(OBJ_DIR)/dll.o dll.c

$(OBJ_DIR)/dllhashtable.o: dllhashtable.c dllhashtable.h
	gcc -g -c -o $(OBJ_DIR)/dllhashtable.o dllhashtable.c

$(OBJ_DIR)/libhash.a: $(OBJ_DIR)/hash_32a.o \
	$(OBJ_DIR)/hashtable.o \
	$(OBJ_DIR)/dll.o \
	$(OBJ_DIR)/dllhashtable.o 
	ar cr $(OBJ_DIR)/libhash.a \
		$(OBJ_DIR)/hash_32a.o \
		$(OBJ_DIR)/hashtable.o \
		$(OBJ_DIR)/dll.o \
		$(OBJ_DIR)/dllhashtable.o


$(OBJ_DIR)/testhash32: testhash32.c $(OBJ_DIR)/libhash.a
	gcc -g -O2 -o $(OBJ_DIR)/testhash32 testhash32.c $(OBJ_DIR)/libhash.a

$(OBJ_DIR)/testhash64: testhash64.c $(OBJ_DIR)/libhash.a
	gcc -g -O2 -o $(OBJ_DIR)/testhash64 testhash64.c $(OBJ_DIR)/libhash.a

$(OBJ_DIR)/testdllhash32: testdllhash32.c $(OBJ_DIR)/libhash.a
	gcc -g -O2 -o $(OBJ_DIR)/testdllhash32 testdllhash32.c \
		$(OBJ_DIR)/libhash.a

$(OBJ_DIR)/testdllhash64: testdllhash64.c $(OBJ_DIR)/libhash.a
	gcc -g -O2 -o $(OBJ_DIR)/testdllhash64 testdllhash64.c \
		$(OBJ_DIR)/libhash.a


clean: 
	rm -rf $(OBJ_DIR); \
	rm -f send recv

