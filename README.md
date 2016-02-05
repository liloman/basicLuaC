
### Info

It's the bare minimum to load a C lib from lua without third party libs alike tolua et al.

### Use

```bash
make
gcc -c -O2 -Wall -fPIC -I/usr/include/lua5.3   -c -o src/basic.o src/basic.c
gcc src/basic.o -o bin/basic.so -shared -L/usr/lib64/  
Calling foo from lua = 12.3
Calling foo from lua = 50.43

```

### Dependencies

lua-devel

### TODO

- [ ] Revolve local path installation 
- [ ] Use cmake and pkg


