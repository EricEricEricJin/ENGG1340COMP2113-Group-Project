# [Box head game]
> HKU ENGG1340 / COMP2113 Group Project

<div align=left>Group members: Jin Yushang (3035855064), Lu Haoze (3035951519)</div>

Description of the game:
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

## Installation
### Install to default location
By default, the binary is installed in directory `$HOME/.local/bin/` and resource folder is `$HOME/.boxhead/`. Note that resource folder is created while installation, user does not need to create it.

To install at default location, execute
```bash
make
make install
```

### Install to customized location
If want to specify install location, execute
```bash
make RESOURCE_DIR=your_resource_folder INSTALL_DIR=your_binary_folder
make install RESOURCE_DIR=your_resource_folder INSTALL_DIR=your_binary_folder
```

Example: if user wants to install the binary under `$HOME/bh_ins_test` and want to install the resource folder as `$HOME/bh_ins_test/hama/`, the user should execute
```bash
make RESOURCE_DIR=$HOME/bh_ins_test/hama INSTALL_DIR=$HOME/bh_ins_test
make install RESOURCE_DIR=$HOME/bh_ins_test/hama INSTALL_DIR=$HOME/bh_ins_test
```
Then the program and resources are installed at following locations:
```
/home/eric/bh_ins_test/
├── boxhead
└── hama
    ├── resource
    │   ├── bullet
    │   │   ├── 9mm.json
    │   │   └── rpg.json
    │   └── map
    │       ├── 0.json
    │       ├── bad_map.json
    │       ├── map_2.json
    │       └── map_3.json
    └── saving
```

### Uninstall from default location
To uninstall, if it is installed in default location, execute
```bash
make uninstall
```

### Uninstall from customized location
If it is installed under user-speficied location, execute
```bash
make uninstall RESOURCE_DIR=your_resource_folder INSTALL_DIR=your_binary_folder
```

__WARNING: When uninstalling, all files under RESOURCE_DIR will be removed. Please type correct path if you use customized location, and backup your savings and resources.__

## User configure file
User can configure config file under RESOURCE_DIR. Its name is `init.bh` and is in JSON format.

Following are the keys can be added to configure file
| Key | Type | Description | Example | Default value |
| --- | ---- | -----------| --- | --- |
| theme | string | dark or light, bord or thin | "dark, bord" | "dark, thin"
| resource_path | string | directory of resource | "/home/eric/hama/" | $HOME/.boxhead/resource/| 
| saving_path | string | directory of savings | "/home/eric/mysavings/" | $HOME/.boxhead/saving |
| Clock frequency | integer | Frequency of the game loop | 8 | 12 |
| player_keyset | map | Keyset to move, shoot | {"up": 107, "down": 106} | {"up": 119, "down": 115, "left": 97, "right": 100, "stop": 101, "fire": 32} |
| ui_keyset | map | Keyset to move in the menu | {"up": 119, "down": 115} | {"up": 107, "down": 106, "enter": 10, "quit": 113} |

For the two key sets, user can also specify part of the keys (unspecified keys will follow the default) 

A full Example of user configure file:
```json
{
    "theme": "light, bord",
    "resource_path": "/home/eric/hama/",
    "saving_path": "/home/eric/mysavings",
    "clock_frequency": 8,
    "player_keyset": {
        "fire": 122
    },
    "ui_keyset": {
        "enter": 111,
        "quit": 112
    }
}
```