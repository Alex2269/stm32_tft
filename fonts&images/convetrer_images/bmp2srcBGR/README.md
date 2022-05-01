
# bmp2src

```

slightly modified code borrowed from:
https://github.com/cbm80amiga/bmp2src

thank you very much to the author

```

### convert svg to bmp:

```

  formats 1 2 3:
  bmp:format=bmp1  -- 32 bit
  bmp:format=bmp2  -- 24 bit
  bmp:format=bmp3  -- 24 bit

  convert RGBY.svg  -scale 50% -define bmp:format=bmp3 RGBY.bmp

  convert lion.png -resize "163x117"! -define bmp:format=bmp3 lion.png

  convert -define bmp:format=bmp3 rgb.svg rgb.bmp
  convert -define bmp:format=bmp3 lion.png lion.bmp
  convert -scale 50% -define bmp:format=bmp3 mainecoon.jpeg mainecoon.bmp

```

```

  make
  gcc -std=gnu11 -o bmp2src bmp2src.c
 ./bin/bmp2src RGBY.bmp       >RGBY.h

```
