# ENGG1340COMP2113 Group Project Proposal
Group No.: 106  
Group members: Jin Yushang (3035855064), Lu Haoze (3035951519)  
Topic: Boxhead  
Link: [https://github.com/EricEricEricJin/ENGG1340COMP2113-Group-Project](https://github.com/EricEricEricJin/ENGG1340COMP2113-Group-Project)

## Project description
We will make a Boxhead game that can run in Linux terminal.

The Box head game is a fast paced zombie shooting game in which the player act as the main character whose aim is to shoot as much zombies as possible(getting more points) before the main character dies. The number of zombies is changing(probably increasing) as the game proceeds, all the zombies are trying to get closer to the main character.

Rules and actions:
The player has to use keyboard to control the movement and action of the main character. The movements includes the regular up, down, left and right, the actions include shooting with a gun and using a bomb. The player should control the main character to move away from the approaching zombies while killing the zombies by using shooting or bomb, because the main character will die if approached by the zombies and points will be accumulated for being able to kill the zombies.

Features planning to implement:
1. Controlling the movement of main character(up, down, left and right)
2. Controlling shooting and using bomb action of main character(keyboard)
3. Accumulating marks for the player when killing zombies
4. Making zombies approaching main character
5. Determining whether main character or zombie is killed
6. Control the appearing and number of zombies throughout the game

[Know more about Boxhead](https://boxhead.fandom.com/wiki/Boxhead:_2Play_Rooms)

## Complexity markers
| Complexity marker              | Purpose                              |
| ------------------------------ | ------------------------------------ |
| File IO                        | Extract and record user settings     |
| Random function                | Decide the actions of zombies        |
| Screen Oriented User Interface | Display the game and read user input |
| ...                            | ...                                  |


## Design
__THE PSEUDOCODE ONLY INDENDS TO SHOW THE LOGIC OF THIS GAME. IMPLEMENTATIONS WOULD NOT COMPLETELY FOLLOW THIS.__
### Pseudocode of the game
```
INPUT: username

IF username not in configure file THEN 
    check font // Check if user's terminal can display unicode and / or nerdfont normally
    // This will be recorded so that user will be about to see characters displayed in the game 
    check sound // whether beep() works in user's terminal
    Enter player name
    Record this in file
ENDIF

Extract player file
INPUT: continue or restart 

IF continue THEN
    Remake
ELSE
    Load saving
ENDIF

Enter game main loop.
```

### Pseudocode of game main loop
```
// Player move and shoot
For each player
    IF up key pressed THEN
        Player.Y --
    ELSE IF down key pressed THEN
        Player.Y ++
    ENDIF

    IF left key pressed THEN
        Player.X --
    ELSE IF right key pressed THEN
        Player.X ++
    ENDIF

    IF Player.shoot THEN
        New instances of bullet(s) or booms or walls or barrel
        Instance(s) run
    ENDIF
ENDFOR

// Player die
FOR each player
    FOR each zombie
        IF player.xy == zombie.xy THEN
            player die
        ENDIF


// Add zombies
LOOP (Some_value - num_of_zombies_in_the_map) TIMES
    New a zombie as zombie_new
    Let zombie_new enter map from random position
ENDLOOP

// Zombie die
FOR each zombie
    Do graph search, find shortest path to player
    Move following this path

    FOR each bullet 
        IF bullet.xy == zombie.xy THEN
            zombie.hp -=  bullet.damage
            zombie.damage[bullet.owner] -= bullet damage
            // TODO: add uid to bullet
            // Zombie record uid so that one zombie will not be hurt by one bullet for many times 
        ENDIF
    ENDFOR

    FOR each bomb
        IF bomb.boom() AND bomb.xy - zombie.xy < bomb.range() THEN
            zombie.hp -= bomb.damage(bomb.xy - zombie.xy)
            zombie.damage[bullet.owner] -= bomb damage
            // TODO: move bomb outside this loop
        ENDIF
    ENDFOR

    IF zombie.hp <= 0 THEN
        FOR each player
            player.score += zombie.damage[player] 
        ENDFOR
        zombie.distroy
    ENDIF
ENDFOR

// Other things...
// Refresh screen, detect manu key, ...

// Bullets / bombs / walls deployed by the player will be implemented into object.
... 
```

## Libraries used
- `ncurses` lib is used to implement screen oriented user interface.
- `json.hpp` is used to read and write json format configure file.
- ...

## Record of tasks
| Planned Action                                                          | Completion date (estimated) | Completed？ |
| ----------------------------------------------------------------------- | --------------------------- | ----------- |
| Initialize git repo                                                     | Mar. 08                     | X           |
| Test ncurses lib and write Makefile that compile programs using the lib | Mar. 10                     | X           |
| Test json.hpp                                                           | Mar. 20                     |             |
| Draw complete UML and flowcharts of the game                            | Mar. 30                     |             |
| Develop game main loop and conduct unit test                            | Apr.10                      |             |
| Develop the user interface and do unit test                             | Apr.25                      |             |
| Finish development and conduct end user testing                         | Apr. 30                     |             |
| Finish documents                                                        | May. 2                      |             |
| ...                                                                     | ...                         | ...         |