
png2c

```

slightly modified code borrowed from:
https://github.com/oleg-codaio/png2c

thank you very much to the author

```

```
===================
Program to convert a PNG file to an C header as an array of hexadecimal unsigned short integers in either RGB565 or RGB5A1 format. This is useful for embedding pixmaps to display.

**Usage:** png2c pngfile [-a]

If -a is specified then the output format will be RGB5A1.

Compiled on Linux, but should work on any platform with libpng installed.
For example output, see example.png and example.h.

  convert 3-2-lion-png.png -resize "163x117"! -define bmp:format=bmp3 lion.png

  convert -define bmp:format=bmp1 rgb.svg rgb1.bmp
  convert -define bmp:format=bmp2 rgb.svg rgb2.bmp
  convert -define bmp:format=bmp3 3-2-lion-png.png lion.bmp

  convert 3-2-lion-png.png -resize 40% -define bmp:format=bmp1 lion1.bmp
  convert 3-2-lion-png.png -resize 40% -define bmp:format=bmp2 lion2.bmp
  convert 3-2-lion-png.png -resize 40% -define bmp:format=bmp3 lion3.bmp

 ./png2c lion.png

  convert svg.png -resize 35% -define bmp:format=bmp3 tiger.png
 ./png2c tiger.png

```
