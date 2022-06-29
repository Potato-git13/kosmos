![Version](https://img.shields.io/github/v/release/Potato-git13/kosmos?include_prereleases)
![Platforms](https://img.shields.io/badge/platforms-linux-brightgreen)
![Made-with-c](https://img.shields.io/badge/made%20with-C-red)
[![Downloads](https://img.shields.io/github/downloads/potato-git13/kosmos/total.svg?maxAge=2592001)](https://github.com/potato-git13/kosmos/releases/)

# The Kosmos Shell for Linux

## A Simple Linux Shell Written in C

This is a simple shell made for fun. It is written in the C programming language and is following the gnu17 standard. Use it to interact with your Linux system. It is configured with a resource file at ```~/.kosmosrc```. The shell supports basic autocompletion of filenames. Any program can be run in it, including some built-in functions.

# Installing

Install the shell with these simple steps. This shell is for Linux systems only!
## For Users

Install kosmos by running these commands, or download it from the release page.
```
git clone https://github.com/Potato-git13/kosmos.git
cd kosmos/
make compile install
```
It is recommended to download from the repository to get the latest updates and fixes. If you download the shell from the release page, you will have to manually copy the binary to ```/bin```.

## For Developers

Run these commands to clone the repository and to change the current directory to it:
```
git clone https://github.com/Potato-git13/kosmos.git
cd kosmos/
```
Program files are located in the ```src/``` directory. Most functions are defined in their separate header file located in the ```headers/``` subdirectory.

Compile the project with ```make compile-debug``` for more information when debugging.

## Dependencies

- readline
- gcc
- make

To install those on ubuntu-based distros, run: ```sudo apt install libreadline-dev gcc make```.

# Usage

If you already installed it, run ```kosmos``` from the terminal, otherwise you can find the compiled binary in the ```bin/``` directory. This will drop you directly to the shell. If you want more information about the program usage, run ```kosmos -h```.

## Customizing

There are built-in functions to add aliases and set environment variables. Example:

```
alias ls "ls ---color"
export PATH /bin:$PATH
```

By editing the ```HISTSIZE``` and ```SAVEHIST``` variables in the ```src/headers/defvars.h``` file, you can customize how many lines are saved to history and how many lines are written to the history file located at ```~/.kosmos_history```.

After editing the file, you will have to recompile the shell.

## Resource File

The resource file is located at ```~/.kosmosrc``` and is read on shell startup. The commands are executed line by line. Any errors will be written to standard error (stderr).

## History File

The history file is located at ```~/.kosmos_history```. Everything entered into the shell is logged here, besides the empty inputs.

# Contributing

Contributing is as easy as:
- creating a fork of the repository,
- adding your changes,
- opening a pull request.

## Guidelines

These are some loose guidelines for contributing to make it easier for everyone.

### Comment the Code

Please add comments to your code, so everyone can understand what's happening.

### Explain your Pull Request

In your pull request, explain the issue you fixed/what you added. If you are fixing an issue, reference the issue number in the pull request message.

### Be Civil

Be kind and civil when communicating with others.