# Wi-Fi Display Program

The Wi-Fi Display Program interfaces to the Keg Monitor program and the
Wi-Fi Network. The Wi-Fi program in installed on an ESP32 D1 R32 board.
The Arduino configuration for this board is defined on the first line of
the main program. The Wi-Fi display programs uses an Asynchronous Web
Server to host the web page display. This program also gets the time of
day from a Network Time Protocol (NTP) server, sends emails, provides a
Domain Name System (DNS) server, (so you don’t have to type in IP
addresses), provides an Over the Air (OTA) program update capability and
provides a Web Serial Interface for debugging. The following figure is a
sample of what the main web page looks like.

![](media\image1.jpg)

The first line is the title of the web page. The second line is the day
of week, day/month/year date followed by the time of day (local time).
The third line is the average temperature from the beer keg monitor
program. The next lines provide information on the five taps. The
information is the number of 12-ounce glasses left, the number of days
the keg has been in the freezer, the alcohol percentage and the title or
style of beer. The next shows if this program is receiving information
from the Keg Monitor program.

The next line shows the Received Signal Strength Indicator (RSSI) in
dBm. A level above -60 is very good. The signal normally needs to be
above -80 dBm to connect to the Wi-Fi. The Page Refresh button just
refreshes the page to get the latest information.

The next line displays the temperature sensor values in the freezer.
Depending on how many you have installed this line will either have none
or three values. The next line is the IP Address. This is a reserved
address that I have forwarded to the internet for access away from home.
If this address changes I know there is something wrong with my router.

The next line is the software build data and time. The next three lines
show alarms if they have occurred. The last two lines show how to upload
a new program or look at the web serial interface. The Keg Monitor main
display is accessed by typing in KegMonitor.local in your web browser.
If you want to upload a program your type KegMonitor.local/update to get
to the update program web page. You can also access main page by typing
in 192.168.0.201.

## Passwords

**You must edit the “Passwords.h” file for the program to work.** This
file contains network names, network passwords, email addresses, email
passwords, DNS name and GMT offset. The file contains comments for all
the required entries.

## KegMonitor.html

This file contains the Hyper Text Markup Language (HTML). This file is
saved as KegMonitor.h to include the HTML instructions in code. When you
edit the HTML file save a copy as KegMonitor.h. I like to use Notepad++
as my HTML editor. I include the HTML web page in my code instead of as
a separate file because when you make changes you only must upload the
firmware and not separate files. The top line is “const char
indx_html\[\] = R"rawliteral(“ tells Arduino compiler to include the
following text in a constant array named indx_html. The last line
“)rawliteral";” tells the complier that this is the end of the character
array. Do not remove either of these lines.

The HTML file has special pointers designated as ‘%’XX. These are used
by the software to know where to insert information to be displayed on
the web page.

## HTML.ino

The HTML module handles building the web page. During setup the constant
character array indx_html is copied to a dynamic array named HTML Page.
During the copy process pointers are saved in an integer array named
indx. Every time the web page is accessed pointers are replaced by text
information and variable values. The build page function performs this
action. Every time the refresh button is pressed the latest information
is displayed.

## Sever.ino

This module handles the server requests. I used an asynchronous server
library because the WebSerial library only works with an asynchronous
server. Asynchronous servers uses interrupts to process server requests.
So, if you use delay functions in your code the server requests will
still be processed. The server is fast, and I have not had any problems.
The /update and /webserial page requests are handled in the respective
libraries.

## Functions.ino

This module has general purpose functions including Initialize Wi-Fi and
time of day.

I used the Arduino standard \<TimeLib.h\> library for time of day
because it gives you more control over time-of-day handling. The time of
day is read from an NTP server during setup. The software accesses two
different NTP servers to get the time of day. When the time of day is
received the onboard Real Time Clock is set to maintain the time. The
software checks the time accuracy every day in the main loop.

The initialize Wi-Fi function connects to the local network. This
function is where you can set the MAC address. Since Wi-Fi connection is
the most important part of this program, the board will try to connect
to the Wi-Fi for 5 minutes. The Wi-Fi connection is checked in the main
loop every 5 minutes. If the connection has been lost the board attempts
to reconnect to the Wi-Fi.

## Serial.ino

The module handles the serial interface between this board and the Keg
Monitor board. The web serial interface is also handled in the module.

## Email and Text Messages

To send an email with this application you need to use an app password
on Gmail. An app password is a 16-digit passcode that gives a less
secure app or device permission to access your Google Account. App
passwords can only be used with accounts that have 2-Step Verification
turned on.

I recommend generating a new Gmail account for emails from your Arduino
devices. You can set up a rule to forward these emails to your main
email account and/or to send them to generate a text message.

My phone service provider is T-Mobile, so to send a text message from an
email you type in the phone number and send it to tmomail.net
(<7145361998@tmomail.net>).

When you get an email, the subject is “Alert from Keg Monitor”. Emails
and text messages are sent for the following reasons.

"Keg Monitor: Temperature is too hot, above 40 degrees F.

"Keg Monitor: Temperature is too cold, below 30 degrees F.

"No communication with Keg Freezer for one hour"

## Port Forwarding

You should be able to do an internet search to help you do port
forwarding using your model router/modem. Basically, there are just two
steps: 1) Set a static or reserved IP address for your device based upon
the MAC address, 2) Forward the reserved IP address.

If you must replace your board, you can set the MAC address, so you
don’t have to redo your router settings. The normal port for a web page
is 80. But this did not work with my internet provider, so I used 8081
for the Keg Monitor.

To access your board, you use the internet service provider IP address
and 8081 port number. For example, <http://174.67.214.107:8081/>. To
find out your IP address, go to website https://www.myip.com/.
