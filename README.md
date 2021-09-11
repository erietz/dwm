# About

This branch is my personal fork of dwm. I created it by:

```sh
git clone git://git.suckless.org/dwm
cd dwm
git remote rename origin upstream
git remote add origin <url of your repo>
git push -u origin master
```

To pull updates from suckless

```sh
git pull upstream/master
```

To push updates to my fork

```sh
git push origin ewr
```

## Installation

`make`

`sudo make install`

Create a file `dwm.desktop` file at `/usr/share/xsessions/dwm.desktop`
containing the following

```conf
[Desktop Entry]
Encoding=UTF-8
Name=Dwm
Comment=Dynamic window manager
Exec=dwm
Icon=dwm
Type=XSession
```

## Recompiling

Since some of the patches that have been applied modify `config.def.h`, I have
added `config.h` to the `clean` rule in the `Makefile`. `config.h` is
regenerated after running `make`.

To recompile the software run `sudo make clean install`
