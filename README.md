# Seam Carving

## Preface

I wanted to help one of my friends with one of their homework assignments in C++, and as a part of that, I somehow ended up getting into a race with him to see who could implement it more quickly. Because he was limited as to the libraries he could use for submission, essentially limiting him to iostream and the other header files in this repo. Because of this, my code is very ugly and is by no means a representation of the kind of readable code I can write.

More than anything, this is one of the random things I wanted to be able to implement by myself, and this was a really nice challenge and a great way to reacquaint myself with C++.

## How it works

The premise of this project lies in being able to shrink and grow an image without losing its "significant" elements. This is acheived in a very cute way in my opinion:

First, each pixel is assigned an energy score, which is computed by the square difference of its adjacent pixel values. This is exmplained in further detail elsewhere so I'lla void miring in the minutia. However, it is important to note that the energy score assigns higher scores to groups of pixels with more edginess.

Next, using this score, the approach to finding a seam is much like that of a pathfinding algorithm. The approach is similar for finding both a vertical or horizontal seam. where you iterate through the energy array in such a way that you assign an optimal score to each pixel, and following that, the most optimal path is explored from the other side of the image starting at the pixel with the lowest score.

This is an example of dynamic programming, which is a field I am probably the least comfortable in compared to the other most schemes for technical interview patterns. The recursive approach, which I won't go into detail about, becomes intractible too quickly.

## Running the Program

Compilation is simple, and you can use g++ or clang interchangeably

`g++ -std=c++20 -Wall -Wextra -Werror -pedantic -Iincludes ./src/main.cc ./src/image_ppm.cc ./src/seam_carver.cc -o o`

It definitely includes a few flags that are perhaps unnecessary, but again, these were the submission criteria.
This outputs to a file called o, which is also probably unfavorable, so rename as you like

This program, once compiled (executable file included for MacOS x86), can be run on the terminal with the following arguments:

- input file name (in hpp format ONLY), use imageMagick or Gimp to convert
- one of:
  - `-h <number of rows>`
  - `-v <number of cols>`
    and finally a path and output file name in the form `~/<path>/<filename>`
