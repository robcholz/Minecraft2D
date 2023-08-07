# variables
SHELL_ROOT=$(pwd)
PROJECT_ROOT="$(dirname "$(dirname "$SHELL_ROOT)")")"

# build client
cmake -B "$PROJECT_ROOT"/build/client -G "Unix Makefiles" -S "$PROJECT_ROOT"/minecraft
make

# build server

# build launcher



