![version](https://img.shields.io/github/v/release/Potato-git13/kosmos?include_prereleases&style=for-the-badge)
![platforms](https://img.shields.io/badge/platforms-linux-brightgreen?style=for-the-badge)
![made-with-c](https://img.shields.io/badge/made%20with-C-red?style=for-the-badge)
# THE KOSMOS SHELL
A simple linux shell written in C

# Installing
```
git clone https://github.com/Potato-git13/kosmos.git
cd kosmos/
make compile install
```

# Customizing
To add aliases and set environemnt varaibles there are buitlin functions for that. Example:

```
alias ls "ls ---color"
export PATH /bin
```

When the shell is started it will read from the resource file located at ```/home/USER/.kosmosrc``` and execute the commands given in it

By editing the ```HISTSIZE``` and ```SAVEHIST``` variables in the ```src/headers/defvars.h``` file you can customize how many lines are saved to history and how many lines are wrote to the history file located at ```~/.kosmos_history```.

After editing the file you will have to recompile the shell.

# Dependencies
- readline
- gcc

To install those on ubuntu-based distros run ```sudo apt install libreadline-dev gcc```