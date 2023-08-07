# variables
SHELL_ROOT=$(pwd)
PROJECT_ROOT="$(dirname "$(dirname "$SHELL_ROOT)")")"
APP_ROOT="$PROJECT_ROOT"/targets/osx.app

# clean
if [ -d "$APP_ROOT" ]; then
  rm -rf "$APP_ROOT"
  echo Removed packaged OSX application!
else
  echo No OSX application to be removed now!
fi