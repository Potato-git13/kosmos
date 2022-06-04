![version](https://img.shields.io/github/v/release/Potato-git13/kosmos?include_prereleases&style=for-the-badge)
![platforms](https://img.shields.io/badge/platforms-linux-brightgreen?style=for-the-badge)
![made-with-c](https://img.shields.io/badge/made%20with-C-red?style=for-the-badge)
# THE KOSMOS SHELL
A simple linux shell written in C

# Installing
```
git clone https://github.com/Potato-git13/kosmos.git
cd kosmos/
make compile push-to-global-bin
```

# Customizing
If you want to customize the aliases you can edit the ```src/config.h``` file and add an entry like so:
```
const struct Aliases aliases[] = {
    {"ls", "ls --color"},
    {"grep", "grep --color"},
    ...
    ...
};
```

By editing the ```HISTSIZE``` and ```SAVEHIST``` variables you can customize how many lines are saved to history and how many lines are wrote to the history file located at ```~/.kosmos_history```.

After editing the file you will have to recompile the shell.

# Dependencies
- readline
- gcc

To install those on ubuntu-based distros run ```sudo apt install libreadline-dev gcc```
