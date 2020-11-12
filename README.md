[![Downloads](https://img.shields.io/github/downloads/Escartem/AmongUsCheatClient/total?color=%2303fc0f&logo=github&logoColor=%23fff&style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/releases)
[![Release](https://img.shields.io/github/v/release/Escartem/AmongUsCheatClient?color=%2342aaf5&logo=github&logoColor=%23fff&style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/releases)
[![Commits](https://img.shields.io/github/commits-since/Escartem/AmongUsCheatClient/0.0.3-beta2?color=%2342f58d&logo=github&logoColor=%23fff&style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/commits/master)
[![Discord](https://img.shields.io/discord/503554429648371712?style=for-the-badge)](https://discord.gg/fzRdtVh)
[![Issues](https://img.shields.io/github/issues/Escartem/AmongUsCheatClient?style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/issues)

---

## Among Us Cheat Client
> ### This is a simple cheat client for Among Us made in C++ and python

# :warning: The devs of among us updated the anticheat of the game, most of the cheats are not working anymore. I'm working on a way to find new cheats and fix the old ones.

---
## Table of content

- [About the client](https://github.com/Escartem/AmongUsCheatClient#About-the-client)
- [Supported versions](https://github.com/Escartem/AmongUsCheatClient#Supported-versions)
- [Features](https://github.com/Escartem/AmongUsCheatClient#Features)
- [Downloading the client](https://github.com/Escartem/AmongUsCheatClient#Downloading-the-client)
- [Using the client](https://github.com/Escartem/AmongUsCheatClient#Using-the-client)
- [Building the dll file](https://github.com/Escartem/AmongUsCheatClient#Building-the-dll-file)
- [Using the command line](https://github.com/Escartem/AmongUsCheatClient#Using-the-command-line)
- [Credits](https://github.com/Escartem/AmongUsCheatClient#Credits)

---

## About the client

## If you want to contribute to the project, look up the CONTRIBUTING.md file for more infos

## :warning: This client is NOT a virus, it is detected as one for most of antivirus because of the injector and the methods used in the files. If you don't trust me read the code by yourself.

## :warning: Make sure you use the correct version of the client depending of the version of your game.

## :warning: Post issues about features not working ONLY if they are not listed as 'not working' in the features part.

---

## Supported versions

| Version of the game | Version of the client required | Statut of the client |
| ---- | ---- | ---- |
| 2020.9.9s |  0.0.2 | :heavy_check_mark: Everything working |
| 2020.9.22s | 0.0.3-beta | :warning: Most features not working |
| 2020.10.8i & later | Not supported | :x: Not supported yet |

---

## Features

# :warning: This list might change because of the new anti cheat system

| Features | Working 2020.9.9s | Working 2020.9.22s |
| ---- | ---- | ---- |
| Force call meeting | :x: | :heavy_check_mark: |
| Noclip | :heavy_check_mark: | :x: |
| Mark Imposters | :heavy_check_mark: | :heavy_check_mark: |
| Radar | :heavy_check_mark: | :heavy_check_mark: |
| Reset ban points | :heavy_check_mark: | :heavy_check_mark: |
| Spam chat | :heavy_check_mark: | :x: |
| Make everyone spam chat | :heavy_check_mark: | :x: |
| Murder anyone even if not impostor | :heavy_check_mark: | :x: |
| Teleport to anyone | :heavy_check_mark: | :heavy_check_mark: |
| Vote off anyone | :heavy_check_mark: | :x: |
| Kick anyone | :heavy_check_mark: | :x: |
| Close doors | :heavy_check_mark: | :x: |
| Sabotage even if not impostor | :heavy_check_mark: | :heavy_check_mark: |
| Repair | :heavy_minus_sign: | :heavy_minus_sign: |
| Instantly complete all tasks | :heavy_check_mark: | :heavy_check_mark: |

---

## Downloading the client

### Before downloading, make sure you looked the [Supported Versions](https://github.com/Escartem/AmongUsCheatClient#Supported-versions) to know what to download

#### Latest versions

- [0.0.3-beta2](https://github.com/Escartem/AmongUsCheatClient/releases/tag/0.0.3-beta2)
- [0.0.2](https://github.com/Escartem/AmongUsCheatClient/releases/tag/0.0.2)

#### Please note that releases are not created as often as dll build, if you want to get the latests features, I reccomend downloading the dll file and injecting it yourself

---

## Using the client

1. Extract zip file in a folder

2. Start AmongUs

3. Run the `AmongUsCheatClient.exe` file

4. A console should spawn saying further instructions

#### If you get an error, make sure to follow the steps that could fix the error before posting an issue

---

## Building the dll file

The client was created in Visual Studio 2019, to edit and compile your own version, get the project located in **AmongUsCheatClient-22s**.
Then compile the project as **Release - x86**

---

## Using the command line

The injector and the main file support command line.

### For the injector, here is the cmd instructions

```bash
Injector.exe --process-name "Among Us.exe" --inject DLL_PATH
```

DLL_PATH being the absolute path of the dll file, for example `F:\Docs\AUCC.dll`

### For the main file, you can use this

```bash
python main.py --dev
```

This will start the client using the dll file located in **AmongUsCheatClient-22s\Release\AUCC.dll** assuming the project is in the same location as the master branch

---

## Credits
I did not made this by myself and I used some work of others people, here is the list :
- [ExtraConcentratedJuice](https://github.com/ExtraConcentratedJuice) - Original client
  - [void0p](https://github.com/v0idp) - Radar cheat
- [nefarius](https://github.com/nefarius) - DLL Injector
  - [Cypher](http://www.raptorfactor.com/) - Injection classes
