CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@ -lm

test_ppm : test_ppm.c read_ppm.c
	$(CC) -g -Wall -Wvla -Werror test_ppm.c read_ppm.c -o $@ 

glitch : glitch.c read_ppm.c
	$(CC) -g -Wall -Wvla -Werror glitch.c read_ppm.c -o $@
