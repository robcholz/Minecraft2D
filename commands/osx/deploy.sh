# variables
SHELL_ROOT=$(pwd)
PROJECT_ROOT="$(dirname "$(dirname "$SHELL_ROOT)")")"
TARGET_ROOT="$PROJECT_ROOT"/targets/osx.app/Contents


# create app bundle
mkdir -p -- "$TARGET_ROOT"
mkdir -p -- "$TARGET_ROOT"/MacOS
mkdir -p -- "$TARGET_ROOT"/Frameworks
mkdir -p -- "$TARGET_ROOT"/Resources
echo >"$TARGET_ROOT"/Info.plist

# copy binary file to the bundle
if [ -x "$PROJECT_ROOT"/bin/MinecraftLauncher ]; then
  cp -R "$PROJECT_ROOT"/bin/MinecraftLauncher "$TARGET_ROOT"/MacOS
else
  echo You have not build the executable yet!
fi

# copy resources

if [ -d "$PROJECT_ROOT"/assets ]; then
  cp -R "$PROJECT_ROOT"/assets "$TARGET_ROOT"/Resources
else
  echo Cannot find assets foder!
fi

# configure plist file
command python3 scripts/icon.py
command python3 scripts/info_plist.py

# copy frameworks

# copy libraries

# final
mv "$PROJECT_ROOT"/targets/osx.app "$PROJECT_ROOT"/targets/MinecraftLauncher.app
