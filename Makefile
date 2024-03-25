CC = g++

CFLAGS = -g -ansi -std=gnu++2a -Wc++11-compat -Wc++14-compat -Wc++17-compat -Wall -Wextra -Weffc++ \
         -Walloca -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wctor-dtor-privacy     \
		 -Wdangling-else -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 \
		 -Winline -Wlarger-than=8192 -Wmissing-declarations -Wnon-virtual-dtor -Woverloaded-virtual   \
		 -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=2          \
		 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused          \
		 -Wvariadic-macros -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs     \
		 -Walloca -Wdangling-else -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow        \
		 -fno-omit-frame-pointer
 

IFLAGS = -I

PROJ_NAME = main_

AKIN_SRC_DIR = akinator
STK_SRC_DIR = stack
TREE_SRC_DIR = tree

OBJ_DIR = obj

AKIN_SRC = $(wildcard $(AKIN_SRC_DIR)/*.cpp)
STK_SRC = $(wildcard $(STK_SRC_DIR)/*.cpp)
TREE_SRC = $(wildcard $(TREE_SRC_DIR)/*.cpp)

AKIN_OBJ = $(patsubst $(AKIN_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(AKIN_SRC))
STK_OBJ = $(patsubst $(STK_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(STK_SRC))
TREE_OBJ = $(patsubst $(TREE_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TREE_SRC))

dif: $(AKIN_OBJ) $(OBJ_DIR)/main.o $(STK_OBJ) $(TREE_OBJ)
	$(CC) $^ -o $(PROJ_NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm $(PROJ_NAME)

$(OBJ_DIR)/%.o : $(AKIN_SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/%.o : $(STK_SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/%.o : $(TREE_SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(IFLAGS) $(CFLAGS) -c $^ -o $@
