cpuid-g: cmd/main.c common/cpuid-g.o
	gcc -Werror $^ -o cpuid-g -Icommon

common/cpuid-g.o: common/cpuid-g.c
	gcc -Werror -c $< -o $@

clean:
	rm cpuid-g
