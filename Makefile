APP = bin/app
SRC_DIR = src
OBJ_DIR = obj
PKGCONF = conan
INCLUDE_DIR = include

SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

CFLAGS := -I$(INCLUDE_DIR) $(shell PKG_CONFIG_PATH=$(PKGCONF) pkg-config --cflags cjson raylib 2>/dev/null)
LDFLAGS := $(shell PKG_CONFIG_PATH=$(PKGCONF) pkg-config --libs cjson raylib 2>/dev/null)

CONAN_ENV = . $(PKGCONF)/conanbuild.sh

all: $(APP)

$(APP): $(OBJ)
	@mkdir -p $(dir $(APP))
	@echo "==> Linking $(APP)..."
	@$(CONAN_ENV) && gcc $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(APP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "==> Compiling $<"
	@$(CONAN_ENV) && gcc -c $< $(CFLAGS) -o $@

run: all
	@$(CONAN_ENV) && ./$(APP)

clean:
	rm -f $(APP) $(OBJ_DIR)/*.o