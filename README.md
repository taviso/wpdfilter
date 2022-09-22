# WordPerfect Filter for Windows Search

This is a simple filter to let Windows Search index WordPerfect files.

If you use WordPerfect for UNIX or DOS on a modern Windows machine, then this will
let you find your documents when you search for them in Windows Explorer or
the Start Menu.

![Screenshot 2022-04-25 131532](https://user-images.githubusercontent.com/123814/165167694-899558d7-e5e6-44ad-a460-d55e03ed81ce.png)

# How?

Using [libwpd](http://libwpd.sourceforge.net/), the same library LibreOffice
uses to open WordPerfect files.

# Why?

I use [WordPerfect for UNIX](https://wpunix.com) it because it is the most complete terminal-based
word processoe.

## Official Filter

- Isn't there an official WordPerfect search filter?

Yes, but it doesn't read WordPerfect 5.x/6.x files (even though they claim it does).

https://www.wordperfect.com/en/pages/items/8800095.html

# Installation

Put the DLL into `C:\Windows\System32`, then register it. Open a command prompt
as Administrator, and then type:

`regsvr32 c:\Windows\System32\wpdfilt64.dll`

You should get a message that registration was successful.

You probably need to restart the Windows Search service, or just reboot. If
that worked, go to your Documents folder and try searching for something in
explorer. It should now be able to see the contents of your `.wp` files.

# Uninstallation

If you don't like it, open a command prompt as Administrator:

`regsvr32 /u c:\Windows\System32\wpdfilt64.dll`

Then delete the DLL file. You might need to reboot.
