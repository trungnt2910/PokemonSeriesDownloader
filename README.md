# PokemonSeriesDownloader

Tool to automatically download Pokémon the Series on Windows.

## Introduction:

Finding sources to watch and download high-quality Pokémon episodes is getting more difficult these days. There used to be good videos uploaded on Youtube and Dailymotion, but now they are all low-quality.

This open-source tool fetches data from `blisseyhusband.in`, a site where you can watch, but **CANNOT** download, episodes of Pokemon the Series: Sun and Moon - Ultra Legends (Season 22), then save these episodes to your local disk for offline watching and sharing.

The tool is created to be open to customization, (theoretically) allowing you to adjust and download Pokémon episodes from other hosts.

## How to use:

Compile `Downloader.cpp` with your favorite compiler. (Tested on MinGW64 9.2.0).

The source comes with a `Dev C++` project, so compiling it there is easiest.

> Note: Be sure to include --std=c++11 (for C++11 standards), and link to libgdi32 and liburlmon.

Edit `Variables.dat` using `Notepad` or other text editors to change your download links and other information.

Run the `.exe` file to start downloading!

That's all!

## Features:

### Current:

- Automated download of Pokémon episodes.
- Download path configuration.
- Progress bar showing current episode's download status.

### Coming soon:

- Portablity to UNIX systems.
- Automatic configuration.
- Exclude downloaded episodes.

## Known issues:

Current tool depends on `blisseyhusband.in`, and Blissey's husband does not seem to be open with his contents. Videos that play properly on your web browser are guaranteed to be detected by our tool. However, some issues have been detected during our testing phase:

- Tool hangs while trying to download some episode: This is because the episode exists, the link obtained is valid, however, Blissey's husband has forbidden all access. This means the video cannot be played even on his official web site.

- Tool hangs while trying to extracting a URL from the latest episode: Blissey's husband have recently been experimenting with using Amazon's Cloudfront streaming service, which streams videos using `.m3u8` files instead of linking `.mp4` files. We are working to extract `.m3u8` links and use `ffmpeg` tool to get the real video.

Before opening an issue, please open the video in your web browser to check whether it actually plays or not.