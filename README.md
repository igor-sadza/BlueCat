# BlueCat

Examples (freeactionscripts), a simple game, flash player.

## How to bypass flash time stamp?

### Without ocx registration.

Use the Windows SDK mt.exe to export the manifest from the exe file. Edit it and import back.

* [Stackoverflow - reading an application's manifest file?](https://stackoverflow.com/questions/420852/reading-an-applications-manifest-file)

* [Stackoverflow - Use Flash ActiveX as registration free](https://stackoverflow.com/questions/20192480/use-flash-activex-as-registration-free)

Older versions of flash player do not have a timestamp.

### With ocx registration.

Simply download [flash11e.ocx](https://github.com/igor-sadza/flash-repo/blob/d47fb791e1950fcd64a28c1d3c0d3f13429fb6e2/flash-player/FlashPlayer-executable/Connection%20Instrument%20SDK%20v6.8.0/Samples/SampleFlashPlayer/Flash11e.ocx). Run CMD as administrator and hit ```regsrv32 flash11e.ocx```
