CFLAGS = -g -ansi -std=gnu++2a -Wc++11-compat -Wc++14-compat -Wc++17-compat -Wall -Wextra -Weffc++ -Walloca -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wctor-dtor-privacy -Wdangling-else -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wlarger-than=8192 -Wmissing-declarations -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -Walloca -Wdangling-else -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow -fno-omit-frame-pointer

all:
	g++ $(CFLAGS) main.cpp tree.cpp -o main

clean:
	rm -rf *.o main

tests1:
	make
	./main
	make clean

tests2:
	g++ $(CFLAGS) main.cpp tree.cpp akinator.cpp -o main
	./main
	make clean

tests3:
	g++ $(CFLAGS) main.cpp tree.cpp akinator.cpp stack/stack.cpp stack/hash.cpp -o main
	./main
	make clean