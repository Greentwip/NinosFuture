from PIL import Image, ImageEnhance # pip install pillow
from os import mkdir, makedirs
import os
import json

import sys

from natsort import natsorted # pip install natsort


slice_path = None

grid_step = 256

def split_image_to_tiles(im):
    #This treats the image `im` as a square grid of images.
    w, h = im.size

    w_step = int(w / grid_step)
    h_step = int(h / grid_step)

    tiles = []
    for y in range(0, h_step):
        for x in range(0, w_step):
            x1 = x * grid_step
            y1 = y * grid_step
            x2 = x1 + grid_step
            y2 = y1 + grid_step
            t = im.crop((x1, y1, x2, y2))
            tiles.append((x, y, t))

    return tiles


def get_args(name, level_name, spritesheet, layer):
    return level_name, spritesheet, layer

'''
    Usage: python slicer.py sheriffman path_to_full_texture 0
'''

def main():
    level_name, spritesheet, layer = get_args(*sys.argv)
    slice_master_path =  level_name + "/" + "__slices"
    slice_path = level_name + "/" + "__slices" + "/" + "layer_" + str(layer)

    if not os.path.exists(slice_path):
        makedirs(slice_path)

    sheet = Image.open(spritesheet)


    tiles = split_image_to_tiles(sheet)

    layer = {
        "order": int(layer),
        "basename": "layer_" + str(layer),
        "tile_x": grid_step,
        "tile_y": grid_step,
        "tiles": []
    }

    for tile in tiles:
        base_name = slice_path + "/" + level_name
        coordinates = str(tile[1]) + "_" + str(tile[0])
        tile[2].save(base_name + "_" + coordinates + ".png", quality=100, optimize=True)

        tile = {
            "x": tile[0],
            "y": tile[1],
            "path": level_name + "_" + coordinates + ".png"
        }
        layer["tiles"].append(tile)


    output = {
        "layers": []
    }

    output["layers"].append(layer)

    #for directory in os.listdir(slice_master_path):
        #layer = {
            #"level": directory,
            #"tiles": []
        #}
        #for root in os.walk(slice_master_path):
            #for subdir in root:
                #if os.path.isdir(str(subdir)):
                    #continue
                #else:
                    #subdir = natsorted(subdir)
                    #for file in subdir:
                        #if not os.path.isdir(os.path.join(slice_master_path, file)):
                            #layer["tiles"].append(file)
        #layers.append(layer)

    definitions_file = open(level_name + "/" + "definitions.json", "w")

    definitions_file.write(json.dumps(output, indent=4, sort_keys=True))
    definitions_file.close()

    #for root, dirs, files in os.walk(slice_master_path):
        #for file in dirs:
            #print(os.path.join(file)) # will print path of directories




if __name__ == '__main__':
  main()
