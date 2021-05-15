# Flood-It
A simple clone of the Flood-It game - made with [raylib 3.5](https://www.raylib.com/index.html) <br>

There are three scripts:<br>
`flood-it-basic.c` - Contains the simple version with only 200 lines of code <br>
`flood-it-advanced.c` - Contains an advanced version with color themes and the possibility to adjust the size<br>
`flood-it-final.c` - Contains the latest version with a rating system and small improvements<br>

The game can be compiled with:
> gcc flood-it-basic.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

## Web: <br>
The web version can be found in [`/web`](https://github.com/zeloszb/Flood-It/tree/main/web) and to compile raylib for web, follow [this](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)) tutorial.<br>
There are already precompiled files in there, but in case of changes the whole thing has to be compiled again. <br>

***The whole game can be played [here](https://zeloszb.pythonanywhere.com/)*** <br>

## Preview: <br>

![final-example](https://user-images.githubusercontent.com/79079495/118356285-b7d5ad80-b574-11eb-989b-89b90d70786c.gif)

