# E - A Suckless VT100 Compatible TXT Editor

## Requirements
A C99 compiler

## Installation
1. Edit `config.mk` for local build settings (E is installed into the `/usr/local` folder by default)
2. Edit `config.def.h` for customizations
3. Run the following as root:

```sh
# E is installed to /usr/local by default
sudo make clean install
```

## Manual
E uses traditional man pages to document it's theory of operation (man section 7) and user-facing interactions (man section 1).  After installation these can be accessed with:

```
# User manual
man e

# Inner workings and research
man 7 e
```

These pages are written in the traditional man/groff syntax but are mostly human-readable and are located in the `man/` directory of this repository.
