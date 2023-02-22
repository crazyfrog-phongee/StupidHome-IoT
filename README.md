# SmartHome-IoT
# Network and IoT Protocols Assignment

# PlatformIO for compiling my project

[![Join the chat at https://gitter.im/formulahendry/vscode-platformio](https://badges.gitter.im/formulahendry/vscode-platformio.svg)](https://gitter.im/formulahendry/vscode-platformio?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge) [![Marketplace Version](https://vsmarketplacebadge.apphb.com/version-short/formulahendry.platformio.svg)](https://marketplace.visualstudio.com/items?itemName=formulahendry.platformio) [![Installs](https://vsmarketplacebadge.apphb.com/installs-short/formulahendry.platformio.svg)](https://marketplace.visualstudio.com/items?itemName=formulahendry.platformio) [![Rating](https://vsmarketplacebadge.apphb.com/rating-short/formulahendry.platformio.svg)](https://marketplace.visualstudio.com/items?itemName=formulahendry.platformio) [![Build Status](https://travis-ci.org/formulahendry/vscode-platformio.svg?branch=master)](https://travis-ci.org/formulahendry/vscode-platformio)

Integrate PlatformIO into Visual Studio Code on top of [PlatformIO Core](http://docs.platformio.org/en/stable/core.html).

## Features

* Build PlatformIO project specified in [Project Configuration File platformio.ini](http://docs.platformio.org/en/stable/projectconf.html#projectconf)
* Upload firmware to devices specified in [Project Configuration File platformio.ini](http://docs.platformio.org/en/stable/projectconf.html#projectconf)
* Open Serial Monitor

## Prerequisites

* Install [PlatformIO Core](http://docs.platformio.org/en/stable/installation.html)

![install](references/image/readme-install.png)

* After the download is complete, you still need Reload

![reload](references/image/readme-reload.png)

* After Reload, you will be prompted to install PlatformIO Core, which takes a little time.

* After the installation is successful, you need to reload again. After the Reload, the PlatformIO is installed.

* In the lower left corner of the VSCode, you can see that there’s one more icon, please see the picture(①), which is the PlatformIO plugin, and then click ② Open Project to open the project.

![open](references/image/readme-open-prj.png)

## Setup Project

### Cloning an existing repository.

* Determine your HTTP clone url. On Github it's probably something like ***https://github.com/crazyfrog-phongee/StupidHome-IoT.git***. Should be on the project's page somewhere.

		cd ~/Desktop
		git clone {{the link you just copied}} Project
Example: 
        git clone https://github.com/crazyfrog-phongee/StupidHome-IoT.git

* This creates a directory named "Project", clones the repository there and adds a remote named "origin" back to the source.

* Use existing project

  If you have an existing PlatformIO project, open the project folder directly in VS Code

* Create new project

  In terminal, run `platformio init --board <your_board_identifier>` to initialize a new PlatformIO project, then open the project folder in VS Code. Refer to [User Guide](http://docs.platformio.org/en/stable/userguide/cmd_init.html) for `platformio init` command. For how to find Board Identifier, you could refer to [this](http://docs.platformio.org/en/stable/quickstart.html#board-identifier).*

## Usage

* **Build PlatformIO project**: use shortcut `Ctrl+Alt+B`, or press `F1` and then select/type `PlatformIO: Build`, or right click the Text Editor and then click `PlatformIO: Build` in context menu

![build](references/image/readme-build.png)

* **Upload firmware to devices**: use shortcut `Ctrl+Alt+u`, or press `F1` and then select/type `PlatformIO: Upload`, or right click the Text Editor and then click `PlatformIO: Upload` in context menu

![upload](references/image/readme-upload.png)

* **Open Serial Monitor**: use shortcut `Ctrl+Alt+S`, or press `F1` and then select/type `PlatformIO: Open Serial Monitor`, or right click the Text Editor and then click `PlatformIO: Open Serial Monitor` in context menu

![openSerialMonitor](references/image/readme-serial.png)

## Edit Serial Monitor Frequency

By default, the serial monitor frequency is rate 9600Hz. To change it, update the platform.ini as below:
```json
{
    "monitor_speed": <value>
}
```

## Issues

Currently, the extension is in the very initial phase. If you find any bug or have any suggestion/feature request, please join the chat on [Linkedin](https://www.linkedin.com/in/phong-nguy%E1%BB%85n-tr%E1%BB%8Dng-94726a246/)
