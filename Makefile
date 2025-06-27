# Compilador e flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude
LDFLAGS := -pthread

# Diretórios
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := tests

# Arquivos fonte principais
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Arquivos de teste
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/$(TEST_DIR)/%.o,$(TEST_SRCS))

# Executáveis
MAIN_EXEC := $(BIN_DIR)/twitter_cli
TEST_EXEC := $(BIN_DIR)/twitter_tests

# Alvo principal
all: $(MAIN_EXEC)

# Regra para o executável principal
$(MAIN_EXEC): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@

# Regra para objetos principais
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Testes Unitários ---
test: $(TEST_EXEC)
	@./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJS) $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

# --- Utilitários ---
run: $(MAIN_EXEC)
	@./$(MAIN_EXEC)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all test run clean
