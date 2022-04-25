# WordPerfect Filter for Windows Search

This is a simple filter to let Windows Search index WordPerfect files. If you
use WordPerfect for DOS on a modern Windows machine, then this will let
you find your documents when you seach in Windows Explorer.

# How?

Using libwpd, the same library LibreOffice uses to open WordPerfect files.

# Why?

There are still people actively using WordPerfect for DOS.

I use it because it is the most complete terminal-based word processing
software.

## Official Filter

- Isn't there an official WordPerfect filter?

Yes, but it doesn't read WP6 DOS files (even though they claim it does).

[https://www.wordperfect.com/en/pages/items/8800095.html]

# Installation

Put the DLL into `C:\Windows\System32`, then register it. Open a command prompt
as Administrator, and then type:

`regsvr32 c:\Windows\System32\wpdfilt64.dll`

You probably need to restart the Windows Search service, or just reboot. If
that worked, go to your Documents folder and try searching for something in
explorer. It should now be able to see the contents of your `.wpd` files.

# Uninstallation

If you don't like it, open a command prompt as Administrator:

`regsvr32 /u c:\Windows\System32\wpdfilt64.dll`

Then delete the DLL file. You might need to reboot.
