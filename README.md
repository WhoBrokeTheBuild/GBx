# GBx

A Toy GameBoy Emulator 

## Roms

On Linux/OSX you can run the following to download a set of roms to test with.
Note: Some roms are commented out, because they take a long time to download.

```
./roms/download.sh
````

## Verbose Levels

Level 1, `-v`

* Cartridge Information
* Library Versions
* File I/O

Level 2, `-vv`

* Hardware Register Changed
* Memory Bank Controller Changed
* Timer Rollover
* Interrupt Handled

Level 3, `-vvv`

* Instruction disassembly
* LCD Mode Changed

Level 4, `-vvvv`

* Memory Read/Write Operations
* SDL Audio Callback
`