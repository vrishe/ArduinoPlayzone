#!/usr/bin/python

import sys, getopt, io, os
import imghdr

from PIL import Image

def iter_frames(im, offset, count):
    try:
        imframe = im.copy()
        palette = imframe.getpalette()

        while count != 0:
            i = im.tell()

            if i >= offset:
                if i != 0:
                    imframe = im.copy()
                    imframe.putpalette(palette)

                if count > 0:
                    count -= 1

                yield imframe

            im.seek(i + 1)
    except EOFError:
        pass



def main(argv):
    mode   = 'rgb'

    offset =  0
    count  = -1

    imagePath = argv[-1]


    if not (imghdr.what(imagePath) in ('gif', 'GIF')):
        raise TypeError("%s must be a GIF file." % imagePath)

    opts, args = getopt.getopt(argv,"m:o:c:", ["mode=", "offset=", "count="])

    for opt, arg in opts:
        if opt in ('-m', '--mode'):
            mode = arg
        elif opt in ('-o', '--offset'):
            offset = max(int(arg), 0)
        elif opt in ('-c', '--count'):
            count = int(arg)

            if count == 0:
                count = -1

    frames = enumerate(iter_frames(Image.open(imagePath), offset, count))

    if mode.lower() == 'rgb':
        with open (os.path.splitext(os.path.basename(argv[-1]))[0] + ".rgb", "wb") as fout:    
            for i, frame in frames:
                for pixel in frame.convert('RGB').getdata():
                    fout.write(bytearray(pixel))
    else:
        for i, frame in frames:
            frame.save(os.path.splitext(os.path.basename(argv[-1]))[0] + "%d.%s" % (i + offset + 1, mode), mode)


if __name__ == "__main__":
   sys.exit(main(sys.argv[1:]))