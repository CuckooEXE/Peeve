# Peeve

Various utilities meant to peeve users in Windows. All are non-destructive.


**License:** MIT License

## Capabilities

 - **Dimmer:** Dims the user's screen slowly
 - **GetDown:** Minimizes all windows
 - **IconShuffle:** Shuffles all the Desktop Icon positions
 - **RandomCAPS:** Turns on CAPS Lock
 - **VolumeDown:** Turns down the System Volume slowly
 - **Peeve:** A utility that combines all of the above and randomly schedules them


## Building

All of these projects build in MSVC without any issues.

_Note:_ IconShuffle must be built for x64 targets, unless the victim computer is 32-bit. This is because the process has to hook into the Desktop process that 
is running in whatever bit-size the host computer is.


## Usage

Double-click!


## Resources

Made an extensive use of the following resources:

 - [CodeProject - Save and Restore Icon Positions on Desktop](https://www.codeproject.com/Articles/639486/Save-and-Restore-Icon-Positions-on-Desktop)
 - [CodeProject - Stealing Program's Memory](https://www.codeproject.com/Articles/5570/Stealing-Program-s-Memory)
 - [StackOverflow - How do I get the window handle of the desktop?](https://stackoverflow.com/a/5691808/10280970)
 - [StackOverflow - How to set and get system volume in Windows?](https://stackoverflow.com/questions/50722026/how-to-get-and-set-system-volume-in-windows)
