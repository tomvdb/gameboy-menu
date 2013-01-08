lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o menu_test.o menu_test.c
lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o menu_test.gb menu_test.o
echo Done.