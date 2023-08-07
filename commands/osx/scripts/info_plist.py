import os
from pathlib import Path
import plistlib

PLIST_ROOT = Path(os.getcwd()).parent.parent / "targets/osx.app/Contents/Info.plist"

properties = {
    "Institute": "Nanchang NO.3 High School, Jiangxi Province, China",
    "CFBundleDevelopmentRegion": "English",
    "CFBundleExecutable": "MinecraftLauncher",
    "CFBundleName": "Minecraft Launcher",
    "CFBundleIconFile": "favicon",
    "CFBundleIdentifier": "com.github.robcholz",
    "CFBundleInfoDictionaryVersion": "6.0",
    "CFBundleShortVersionString": "1.0",
    "CFBundlePackageType": "APPL",
    "CFBundleSignature": "????",
    "LSApplicationCategoryType": "public.app-category.games",
    "LSMinimumSystemVersion": "10.10.0",
    "NSHighResolutionCapable": True,
    "NSRequiresAquaSystemAppearance": False,
}
fileName = open(PLIST_ROOT, 'wb')
plistlib.dump(properties, fileName)
fileName.close()
