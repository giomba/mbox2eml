# mbox2eml

## What is mbox2eml
Do you need to import some *.eml* email with your graphical client but the only thing you have is a huge *mbox* backup file?
```mbox2eml``` is a KISS (Keep it simple stupid) one-file app, which extracts one *.eml* file for every message contained in your *mbox* file.
It uses C++ STL and some POSIX primitives.

## Build
```
cd src
make
make install
```

## Usage
```
./mbox2eml < MBoxFile
```
produces one file per every email contained in MBoxFile

## Known bugs
- Further runs of this program will overwrite older files
- Does not care about text encoding
- Could break inline-PGP

## License
This program is free software and is distributed under the terms of GNU GPL version 3 or any other further version.
