import subprocess
import os
import shutil
from pathlib import Path
from PIL import Image, ImageEnhance

ICON_ROOT = Path(os.getcwd()).parent.parent / "application/icon.png"
CACHE_ROOT = Path(os.getcwd()).parent.parent / "cache/icon.iconset"
ICON_TARGET_ROOT = Path(os.getcwd()).parent.parent / "targets/osx.app/Contents/Resources"


def icon_export(image, w, h):
    image = Image.open(image)
    resized = image.resize((w, h), 0)  # nearest resampling
    if not os.path.exists(CACHE_ROOT):
        os.makedirs(CACHE_ROOT)
    enhancer = ImageEnhance.Brightness(resized)
    resized = enhancer.enhance(0.95)
    resized.save(CACHE_ROOT / "icon_{0}x{1}.png".format(w, h))


def icon_process(src, dest):
    if not os.path.exists(src):
        os.makedirs(src)
    subprocess.run(["iconutil", "-c", "icns", src, "-o", dest / "favicon.icns"])
    shutil.rmtree(src)


icon_export(ICON_ROOT, 256, 256)
icon_export(ICON_ROOT, 128, 128)
icon_export(ICON_ROOT, 64, 64)
icon_export(ICON_ROOT, 32, 32)
icon_export(ICON_ROOT, 16, 16)
icon_process(CACHE_ROOT, ICON_TARGET_ROOT)
