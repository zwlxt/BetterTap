#!/usr/bin/env python3
from glob import glob
from pathlib import PurePath
import subprocess
from urllib.error import ContentTooShortError

assets_folder = "assets"
asset_files = []
asset_files.extend(glob(assets_folder + "/*.html"))
asset_files.extend(glob(assets_folder + "/*.css"))


def read_file(asset_file):
    result = subprocess.run(["html-minifier", "--collapse-whitespace", "--remove-comments", "--remove-optional-tags", "--remove-redundant-attributes", "--remove-script-type-attributes", "--remove-tag-whitespace", "--use-short-doctype", "--minify-css", "true", "--minify-js", "true", asset_file], stdout=subprocess.PIPE)

    return result.stdout.decode("utf8")

def generate_file(new_file, content):
    var_name = "ASSET_" + PurePath(new_file).name.replace(".", "_").upper()

    with open(f"{new_file}.h", "w+") as f:
        f.write(f"const char {var_name}[] PROGMEM = \"{content}\";\n")

        f.write(f"const unsigned long {var_name}_LEN = sizeof({var_name});\n")


for asset_file in asset_files:
    print(f"compiling {asset_file}")

    content = read_file(asset_file)

    content = content \
        .replace("./style.css", "/css/style.css") \
        .replace("\\", "\\\\") \
        .replace("\"", "\\\"")

    generate_file(asset_file, content)


    

    

    

    
