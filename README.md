# image-convolution
Image processing using a kernel (convolution matrix) for blurring, sharpening, embossing, edge detection, and more.

# Usage
1) download the following files into a directory: conv.c, conv.h, makefile, main_conv.c, edge
2) use `make` to create the `./filter` command
3) use `./filter {inputfile.ppm} {kernel} {outputfile.ppm}` to create new image
4) open image :)

# Result
Before Processing:<img alt="Flowers" src="https://github.com/hafezissa/image-convolution/blob/master/flowers.jpg" align="right">

After Processing:<img alt="Flowers" src="https://github.com/hafezissa/image-convolution/blob/master/flowersME.jpg">

# Replicate Results
After downloading all files necessary, proceed to downloading the flowers.ppm file. Follow the steps instructed:
1) in the same directory as the files, run `make`
2) run `./filter flowers.ppm edge flowersEdge.ppm`
3) open flowersEdge.ppm :)
  
  
Thank you
