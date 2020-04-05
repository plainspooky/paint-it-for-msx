Paint-it for MSX
===

Copyright 2020, Giovanni Nunes

Licensed under [GPL 3.0](LICENSE).

# Introduction

This is a MSX-DOS version for the [Flood-It](https://unixpapa.com/floodit/) game that I've written using [SDCC](http://sdcc.sourceforge.net/).

# About the game

Just fill the board with a single color.

# How to build

Setup the development environment using [Vangrant](https://www.vagrantup.com/):

```shell
vagrant up
```

It will create and setup a virtual machine running **Ubuntu Linux**, with **SDCC** installed and properly configured to generate **MSX-DOS** executables (by Avelino' Herreras' [backend for MSX-DOS](http://msx.avelinoherrera.com/index_en.html#sdccmsxdos)).

When virtual machine ends, simply do:

``` shell
vagrant ssh
cd /vagrant
make
```

A MSX-DOS "COM" named`paint-it.com` will be created. So copy it to a memory card, floppy disk or even to a disk image.

Optionally, there is a TCL script that helps to import

# How to play

Press «SPACE» to generate a new board, color filling starts from upper left corner and spread to all 

# Screenshots

#### Initial screen

![](pictures/paint-it_1.png)

#### In game playing

![](pictures/paint-it_2.png)

#### Help screen

![](pictures/paint-it_3.png)

#### You win!

![](pictures/paint-it_4.png)



# Toolbox

I've used the follo