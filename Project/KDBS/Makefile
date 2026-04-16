# ============================================================
# KDBS — versioned directory build
#
# Public commands:
#   make        -> release build
#   make debug  -> debug build
#   make clean  -> remove latest out-K/vN.N.N
# ============================================================

SHELL := /bin/sh

# -------------------------
# Project
# -------------------------
NAME        := KDBS
SRC_DIR     := src-K
MOD_DIR     := $(SRC_DIR)/mod
INC_DIR     := lib-K
OUT_DIR     := out-K

MAIN_SRC    := $(SRC_DIR)/main.c
MOD_SRCS    := $(shell find $(MOD_DIR) -type f -name '*.c' 2>/dev/null)

# -------------------------
# Toolchain
# -------------------------
CC          := gcc

# -------------------------
# Flags
# -------------------------
CPPFLAGS    := -I$(INC_DIR) -MMD -MP
CSTD        := -std=c17

WARN_STRICT := \
	-Wall -Wextra -Wpedantic \
	-Wshadow -Wformat=2 \
	-Wstrict-prototypes -Wold-style-definition \
	-Wcast-qual -Wwrite-strings \
	-Wundef -Wnull-dereference \
	-Wdouble-promotion -Wimplicit-fallthrough

CFLAGS_RELEASE := $(CSTD) $(WARN_STRICT) \
	-O3 -march=native -mtune=native \
	-flto -fomit-frame-pointer \
	-ffunction-sections -fdata-sections \
	-DNDEBUG

LDFLAGS_RELEASE := \
	-flto -Wl,--gc-sections -Wl,-O1

CFLAGS_DEBUG := $(CSTD) $(WARN_STRICT) \
	-O0 -g3 -ggdb \
	-fno-omit-frame-pointer \
	-fno-inline

LDFLAGS_DEBUG :=

# -------------------------
# Version discovery
# Finds highest existing:
#   out-K/vN.N.N
# Then increments patch with carry:
#   0.0.9 -> 0.1.0
#   0.9.9 -> 1.0.0
# If none exist: 0.0.0
# -------------------------
LAST_VER := $(shell \
	if [ -d "$(OUT_DIR)" ]; then \
		find "$(OUT_DIR)" -maxdepth 1 -mindepth 1 -type d -name 'v[0-9]*.[0-9]*.[0-9]*' 2>/dev/null | \
		sed 's|.*/v||' | \
		sort -V | tail -n 1; \
	fi \
)

NEXT_VER := $(shell \
	v='$(LAST_VER)'; \
	if [ -z "$$v" ]; then \
		printf '0.0.0\n'; \
	else \
		IFS=. set -- $$v; \
		ma=$$1; mi=$$2; pa=$$3; \
		pa=$$((pa + 1)); \
		if [ $$pa -ge 10 ]; then pa=0; mi=$$((mi + 1)); fi; \
		if [ $$mi -ge 10 ]; then mi=0; ma=$$((ma + 1)); fi; \
		printf '%d.%d.%d\n' "$$ma" "$$mi" "$$pa"; \
	fi \
)

VER_DIR      := $(OUT_DIR)/v$(NEXT_VER)
LNK_DIR      := $(VER_DIR)/lnk
EXE_TARGET   := $(VER_DIR)/$(NAME)

LATEST_DIR   := $(if $(LAST_VER),$(OUT_DIR)/v$(LAST_VER),)

# -------------------------
# Objects
# -------------------------
MAIN_OBJ     := $(LNK_DIR)/main.o
MOD_OBJS     := $(patsubst $(MOD_DIR)/%.c,$(LNK_DIR)/mod/%.o,$(MOD_SRCS))

EXE_OBJS     := $(MAIN_OBJ) $(MOD_OBJS)
DEPS         := $(EXE_OBJS:.o=.d)

# -------------------------
# Public targets
# -------------------------
.PHONY: all debug clean

all: CFLAGS  := $(CFLAGS_RELEASE)
all: LDFLAGS := $(LDFLAGS_RELEASE)
all: $(EXE_TARGET)
	@printf 'Built release: %s\n' "$(EXE_TARGET)"

debug: CFLAGS  := $(CFLAGS_DEBUG)
debug: LDFLAGS := $(LDFLAGS_DEBUG)
debug: $(EXE_TARGET)
	@printf 'Built debug: %s\n' "$(EXE_TARGET)"

clean:
	@if [ -n "$(LAST_VER)" ] && [ -d "$(LATEST_DIR)" ]; then \
		printf 'Removing latest build: %s\n' "$(LATEST_DIR)"; \
		rm -rf "$(LATEST_DIR)"; \
	else \
		printf 'No versioned build directory found in %s\n' "$(OUT_DIR)"; \
	fi

# -------------------------
# Final output
# -------------------------
$(EXE_TARGET): $(EXE_OBJS) | $(VER_DIR)
	@printf 'LD %s\n' "$@"
	@$(CC) $(EXE_OBJS) -o "$@" $(LDFLAGS)

# -------------------------
# Compile rules
# -------------------------
$(LNK_DIR)/main.o: $(MAIN_SRC) | $(LNK_DIR)
	@printf 'CC %s\n' "$<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c "$<" -o "$@"

$(LNK_DIR)/mod/%.o: $(MOD_DIR)/%.c | $(LNK_DIR)/mod
	@mkdir -p "$(dir $@)"
	@printf 'CC %s\n' "$<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c "$<" -o "$@"

# -------------------------
# Directories
# -------------------------
$(VER_DIR):
	@mkdir -p "$(VER_DIR)"

$(LNK_DIR): | $(VER_DIR)
	@mkdir -p "$(LNK_DIR)"

$(LNK_DIR)/mod: | $(LNK_DIR)
	@mkdir -p "$(LNK_DIR)/mod"

# -------------------------
# Auto dependency include
# -------------------------
-include $(DEPS)