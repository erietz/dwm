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
