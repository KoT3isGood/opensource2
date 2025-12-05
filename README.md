# opensource 2

It is a game base written using Facepunch's fork of Valve's Source 2.

The goal for this project is to recreate game base (entities) from Valve's games using Source 2 fork provided by s&box.
The reason for this fork is bindings for C# (even though we don't use it) allow us to interact with most of the stuff within the engine via `igen_engine`, even using native code, which we are using. The problem is we are using them and not vtables, which doesn't make the code as readable, as you would like

## Installation

### Windows -> Windows

TODO

For now you can use WSL.

### Linux -> Windows

Prerequisites:
- [funny project compiler](https://github.com/KoT3isGood/fpc)
- DOTNET 10

1. `git clone https://github.com/KoT3isGood/opensource2.git --recursive`
2. `cd opensource2/opensource2-sbox`
3. `./pull.sh`
4. `cp `
5. `cd ../`
6. `fpc build`

Now you can run `game/launcher.exe`

### Linux -> Linux 

TODO

s&box supports it but is it worth it?

## Goals

- [ ] Implement entites
  - [ ] CBaseEntity
    - [ ] CBaseModelEntity
    - [ ] CServerOnlyEntity
      - [ ] CLogicalEntity
    - [ ] CPointEntity

You may look here for function names.
- https://moddota.com/api/#!/vscripts

And here for the inheritance.
- https://docs.cssharp.dev/api/CounterStrikeSharp.API.html
