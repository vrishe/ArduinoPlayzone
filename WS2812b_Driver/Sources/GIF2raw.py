
#!/usr/bin/python

import sys, getopt, io, os
import re, struct, binascii

from PIL import Image

def iter_frames(im):
    try:
        i= 0
        while 1:
            im.seek(i)
            imframe = im.copy()
            if i == 0: 
                palette = imframe.getpalette()
            else:
                imframe.putpalette(palette)
            yield imframe
            i += 1
    except EOFError:
        pass



def main(argv):
    im = Image.open(argv[-1])

    with open (os.path.basename(argv[-1]) + ".RAW", "wb") as fout:    
        for i, frame in enumerate(iter_frames(im)):
            for pixel in frame.convert('RGB').getdata():
                fout.write(bytearray(pixel))



if __name__ == "__main__":
   sys.exit(main(sys.argv[1:]))
