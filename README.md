[![Downloads](https://img.shields.io/github/downloads/Escartem/AmongUsCheatClient/total?color=%2303fc0f&logo=github&logoColor=%23fff&style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/releases)
[![Release](https://img.shields.io/github/v/release/Escartem/AmongUsCheatClient?color=%2342aaf5&logo=github&logoColor=%23fff&style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/releases)
[![Commits](https://img.shields.io/github/commits-since/Escartem/AmongUsCheatClient/0.0.1?color=%2342f58d&logo=github&logoColor=%23fff&style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/commits/master)
[![Discord](https://img.shields.io/discord/503554429648371712?style=for-the-badge)](https://discord.gg/fzRdtVh)
[![Issues](https://img.shields.io/github/issues/Escartem/AmongUsCheatClient?style=for-the-badge)](https://github.com/Escartem/AmongUsCheatClient/issues)

## Among Us Cheat Client
> ### This is a simple cheat client for Among Us

## :warning: Make sure you use the correct version of the client depending of the version of your game
## Supported versions :
| Version of the game | Version of the client required | Statut of the client |
| ---- | ---- | ---- |
| 2020.9.9s |  0.0.2 | :heavy_check_mark: Everything working |
| 2020.9.22s | 0.0.3-beta | :warning: Most features not working |
| 2020.10.8i | Not supported | :x: Not supported yet |

## Features:
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

## Usage
### If you have the version v2020.9.22s of the game you need to download the beta version of the client [here](https://github.com/Escartem/AmongUsCheatClient/releases/tag/0.0.3-beta2)
### Otherwise, download the latest release from the [release page](https://github.com/Escartem/AmongUsCheatClient/releases/latest)

Start AmongUs

Run the `AmongUsCheatClient.exe` file

A console should spawn, if not something is wrong and you should create an issue

Press Delete in-game to pull up the menu

## Command Line & Build
If you want to build the dll file by yourself, download the Visual Studio Project and compile the project as **Release x86**
Than, to use the injector manually, open a command prompt in the directory of the injector and run 
```bash
Injector.exe --process-name "Among Us.exe" --inject DLL_PATH
```
DLL_PATH being the absolute path of the dll file, for example `F:\Docs\AUCC.dll`

If you placed the visual studio in the same place as the project, you can also run `python main.py --dev` to be more quick

## Credits
I did not made this by myself and I used some work of others people, here is the list :
- [ExtraConcentratedJuice](https://github.com/ExtraConcentratedJuice) - Original client
  - [void0p](https://github.com/v0idp) - Radar cheat
- [nefarius](https://github.com/nefarius) - DLL Injector
  - [Cypher](http://www.raptorfactor.com/) - Injection classes
