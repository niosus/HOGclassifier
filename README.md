HOGclassifier
=============

A program that uses libSVM in order to classify HOG features for visual object detection.
Currently I used it to detect cars, so you may also find the positive training data there.
It is not the final version, so it may lack quality in detection.

Warning! This program is growing and shrinking without any prior notice. However, once my thesis is done, it will shrink in size so that it does only car detection with a monocular camera. Thanks for interest!

Usage
-------
To run this code you will need some external dependencies, such as `OpenMP` and `OpenCV`.
You can download those on respective websites.
The app uses `c++0x`, so a modern compiler would also be of use.

To build this software you just need to:

    cd ./build/`
    cmake ..`
    make`

After these actions you should find an executable in `bin` folder.

Currently (you may kick me for this) the pathes to training and test data are hardcoded.
You may want to change this. If it looks nice afterwards I would be happy to accept the pull request.

The other thing is the purpose of use of this piece of software. I use it to detect cars with a
bumblebee camera. Therefore I also analyze the depth of the car. You may safely skip it.
All relevant code is found in `ResultAnalyzer` class.

WARNING - I only ran the software on `Ubuntu 12.04 x64`, so no guarantees that it will work on other
systems what so ever.

Training Data
------------
You may also find quite a bit of training data in the folder. The `neg` folder is empty,
so will have to fill the negative set by yourself. Every image in `neg_square` has to be `128x128`
and in `neg_rect` -> `128x64`. I have around 8000 negative samples randomly sampled from bigger images
with no cars present on them. This part is actualy crucial.


Credits
--------
This program was inspired by @DaHoC and his code here - https://github.com/DaHoC/trainHOG.
However, even though it started of as quite a clone in the end there is not so much left in common.
Also refer to licences of the third party software used by me (such as OpencV and libSVM).

Licening
--------
![alt text][logo]

[logo]:http://www.gnu.org/graphics/gplv3-127x51.png "Logo Title Text 2"


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see the copy of the licence here: http://www.gnu.org/licenses/.

Contact
-------
You may always leave an issue on GitHub page of the project. Or contact me directly via igor.bogoslavskyi@gmail.com

The first method is preferable.

