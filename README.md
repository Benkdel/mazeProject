# Maze Porject using SDL and RayCasting, code in C++


# command to make file: mingw32-make.exe

# instructions to use this repo (for now working only in windows):

1) clone it.
2) set up a hidden folder.vscode with c_cpp_properties.json. in this file be sure to set the configurations to include
the path where your mingW3 libs will be:
    Something like this:
```
            "includePath": ["${workspaceFolder}/**",
                    "C:/Users/username/dev/MinGW_Libs/include/SDL2"
``` 
3) download the libraries from http://libsdl.org/download-2.0.php, this is using 32bits windows
4) add the path where these libs are in the PATH (windows, system, adv config, env variables), should be the same as what you put in 2)
5) install mingw32-make.exe and add route to path
6) download the SDL_Image libs and add them to where you put the other libs
7) copy the zlib1.dll to where your executable is going to be
8) go to where the makefile is, in the src/ folder, and run ming32-make.exe

Implemented:

1 2D raycasting with DDD
2 boring triangle as player
3 player scaling, rotation and translation (rotation still on arrow keys, tbu)
4 simple map

Todo:
1 collision detection
2 render walls on world port based on rays
3 implement textures
4 fix outer walls
5 implement models (such as guns, enemies, etc)
6 implement mouse movement