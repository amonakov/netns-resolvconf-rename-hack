rename-hack.so: rename-hack.c
	$(CC) $(CFLAGS) $^ -fPIC -shared -o $@ -ldl
