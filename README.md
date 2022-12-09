**About**</br>
**Sapper** is a logic puzzle video game genre generally played on personal computers. The game features a grid of clickable squares(+), with hidden **bombs** scattered throughout the board. 
The objective is to clear the board without detonating any bombs, with help from clues about the number of neighboring mines in each field. 

**Abilities**</br>
Game has **points** and **lifes** </br>
* Points - you get the points after marking cell with bombs(key F)
* Lifes - after getting the points aliquot by 8, your got +1 life.
Your have unlimited **marks** for marking a cell, that in your opinion contain a bomb.

![game](https://scontent.fdnk3-1.fna.fbcdn.net/v/t1.15752-9/315529502_466691502065126_8870202669835017942_n.png?_nc_cat=110&ccb=1-7&_nc_sid=ae9488&_nc_ohc=MMbz7E1iRAoAX-XJjad&tn=DsFMSa2ED_fTzdfJ&_nc_ht=scontent.fdnk3-1.fna&oh=03_AdR8CjdEHQglRwDITD8vBPlaqRvjHeF80ECwEvB989CCfg&oe=63BAD36C)

**Game field**</br>
In my implementations your game field work on chunks. Where every chunk contain the 10 bombs. It is make possible generate huge field with a lot of chunks:
```
class GameModel : public stf::smv::BaseModel
{
public:
    /* Now field is present the square 5x5 chunks */
    static constexpr int Width  = 5;    // } \
                                        //     Change this value with your, if you need more chunks
    static constexpr int Height = Width;// } /
}
```

**Information console**</br>
Also you have information console, with info about loaded junks, used memory, cursor position, etc. To enable, disable this press '~'.

![console](https://scontent.fdnk3-2.fna.fbcdn.net/v/t39.30808-6/318678519_171088468874355_4022065156379539719_n.jpg?_nc_cat=106&ccb=1-7&_nc_sid=730e14&_nc_ohc=-exa0Z1AZJYAX_He5e6&_nc_ht=scontent.fdnk3-2.fna&oh=00_AfCZER_gg2978ccSvkD5cMDs6X4qc3-azDgE5-v8_obTbw&oe=6397FD3F) 

**Controls**
* WASD - cursor movement
* ~ - informations console
* F - mark a cell
* SPACE - press on cell

*Copyright (C) 2022 Viktor Nekrutenko*
