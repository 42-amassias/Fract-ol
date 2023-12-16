# FRACT-OL
---

## Index

* [Introduction](#introduction)
	* [About the author](#about-the-author)
	* [The goal](#the-goal)
	* [Problems](#problems)
	* [Solutions](#solutions)
* [The command line](#the-command-line)
	* [The print command](#the-print-command)
	* [The set command](#the-set-command)
* [What is a fractal](#what-is-a-fractal)
* [The core](#the-core)
	* [What is a rendering kernel](#what-is-a-rendering-kernel)
		* [Example of a rendering kernel](#example-of-a-rendering-kernel)
		* [Rules on how to write a rendering kernel](#rules-on-how-to-write-a-rendering-kernel)

---

## Introduction

This is [my](#about-the-author) take on the project `fract-ol` of `School 42` !

### The goal

The goal of this project is to make a [program](#the-core) that will render [fractals](#what-is-a-fractal) and allow the user to [play](#the-command-line) with them.

### Problems

However, I was not happy with a few things :

- Rendering the [fractals](#what-is-a-fractal) on the `CPU` is a very slow task.
- Having to restart the program each time you want change the [fractal](#what-is-a-fractal) that is being rendered on the screen.
- Not having the possibility to change the parameters in real time and see their effects.
- Having to recompile your whole `fract-ol` each time you want to add a new [fractal](#what-is-a-fractal) to render.

### Solutions

Having so much time to waste, I've decided to improve on the project:

- I made the renderer on the `GPU` with [OpenCL](https://www.khronos.org/opencl/).
- I implemented [a way](#the-command-line) to switch what fractal is being rendered on the screen.
- I implemented [a way](#the-command-line) to play with the parameters for each [fractal](#what-is-a-fractal)
- I implemented a [command line tool](#the-command-line) to manage everything in the renderer.

---

## What is a fractal

WIP

---

## The core

This project is more of a framework than anything else.

All the [fractal](#what-is-a-fractal) logic reside within [rendering kernels](#what-is-a-rendering-kernel) writen in [OpenCL](https://www.khronos.org/opencl/).

### What is a rendering kernel

Rendering kernels are special functions in a source code file written in C that will be executed on the graphics card.

To ease the use of every one, I've laid outsome ground rules on how to make a rendering kernel :

#### Example of a rendering kernel

```c
#define DEFAULT_COLOR 0x00000000

#define GET_RED(x) (((x) & 0xFF0000) >> 16)
#define GET_GREEN(x) (((x) & 0x00FF00) >> 8)
#define GET_BLUE(x) ((x) & 0x0000FF)

#define MAKE_COLOR(r, g, b) (((r) << 16) | ((g) << 8) | (b))

__kernel void   my_awesome_kernel(
                    // (1) The mandatory arguments.
                    __global int *screen,
                    uint width,
                    uint height,
                    double dx,
                    double dy,
                    double zoom,
                    // (2) Maybe even more parameters !
                    uchar g)
{
    int x;
    int y;
    int r;
    int b;
    int my_awesome_pixel_color;

    // (4) This is how you get the coordinate of a pixel.
    x = get_global_id(0);
    y = get_global_id(1);

    // The logic to compute my_awesome_pixel_color.
    r = 0;
    if (x % 2 == 0)
        r = 0xFF;
    b = 0;
    if (y % 2 == 0)
        b = 0xFF;
    my_awesome_pixel_color = MAKE_COLOR(r, g, b);

    // (3, 6) This is how you modify a pixel.
    screen[x + width * y] = my_awesome_pixel_color;
}
```

#### Rules on how to write a rendering kernel

1. There is mandatory arguments. These arguments need to always be in the same order, have the same types, the same names and the same type qualifiers !
	* `screen` is the back screen buffer. __It cannot be modified by the user.__
	* `width` is the width of the back screen buffer. __It cannot be modified by the user.__
	* `height` is the height of the back screen buffer. __It cannot be modified by the user.__
	* `dx` represents the x cordinate of the "camera position".
	* `dy` represents the y cordinate of the "camera position".
	* `zoom` represents the ammount of zoom on the scene.
2. After the mandatory arguments, you can have as many arguments as you want ! The only restrictions are that they need to if type qualifier `__private` and also be of the types:
	* `char` `uchar`
	* `short` `ushort`
	* `int` `uint`
	* `long` `ulong`
	* `float`
	* `double`
3. A rendering kernel will always render one and only one pixel !! If not, a [datarace](https://en.wikipedia.org/wiki/Race_condition#Data_race) may occur and the result won't be pleasing.
4. The coordinates of the pixel to render are `(x, y)` (check the [code snippet](#example-of-a-rendering-kernel)).
5. A pixel on encoded as `ARGB`. The `alpha` channel is __not__ used.
6. To change the color of a pixel, use the last line of the [code snippet](#example-of-a-rendering-kernel).

---

## The command line

The command line is a simple yet powerful tool !

To get a prompt, press `c` while on the window then check back on you terminal.

The command line has 3 main commands:

### The `print` command

It is used to show general informations about the current state of the program.

* [x] `print kernels`
	- Prints the list of currently available [rendering kernels](#what-is-a-rendering-kernel).
* [x] `print current`
	- Prints the currently active [rendering kernel](#what-is-a-rendering-kernel).
	- If no [rendering kernel](#what-is-a-rendering-kernel) is currently active, will print `None.`.
* [x] `print params`
	- Prints all the parameters of the currently active [rendering kernel](#what-is-a-rendering-kernel).
	- If no [rendering kernel](#what-is-a-rendering-kernel) is currently active, will print `None.`.
* [ ] `print param [PARAM_NAME]`
	- Prints the type, the name and the value of a parameter in the form `[TYPE] [NAME] = [VALUE]`.
	- If no [rendering kernel](#what-is-a-rendering-kernel) is active, `No kernel is currently active` will be printed.
	- If `[PARAM_NAME]` is not a valid parameter, `Error : [PARAM_NAME] is not a parameter of the current kernel.` will be printed.

_Note: You can use the command `p` as a short hand for `print`._

### The `set` command

It is used to set properties of the engine.

* [x] `set kernel [KERNEL_NAME]`
	- Sets the currently active [rendering kernel](#what-is-a-rendering-kernel) to `[KERNEL_NAME]`
	- If `[KERNEL_NAME]` is not a valid [rendering kernel](#what-is-a-rendering-kernel) name, the currently active [rendering kernel](#what-is-a-rendering-kernel) will stay the same and the message `Unknown kernel "[KERNEL_NAME]".` will be printed.
* [x] `set param [PARAM_NAME]`
	- Changes the value of `[PARAM_NAME]`.
	- You will be given a prompt in the form of `[TYPE] [PARAM_NAME] = ` to input the value you want to give to the parameter.
	- If the value that you enter is erronous (say `.1` instead of `1` for an integer), `[PARAM_NAME]` will become `0`.
	- If there is no currently active [rendering kernel](#what-is-a-rendering-kernel), `No kernel is currently active.` will be printed.
	- If `[PARAM_NAME]` is not a valid parameter of the currently active [rendering kernel](#what-is-a-rendering-kernel), `Unknown parameter [PARAM_NAME].` will be printed.

_Note: You can use the command `s` as a short hand for `set`._

---

## About the author

Hello everyone !

Thanks you for checking out my project !!

I'm Antoine Massias (`@amassias` on the [42 intra](https://intra.42.fr)), these are some basic infos on me at the time of making this project (december 2023) :
* 21 years old
* Studying at [42 le havre](https://www.42lehavre.fr/en/home/)
* Previously studied 3 years in [uni (in Rouen)](https://sciences-techniques.univ-rouen.fr/)
