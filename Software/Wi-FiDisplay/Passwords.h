/* Network varialbes */
const char* ssid     = "network ssid";
const char* password = "password";  
#define AUTHOR_EMAIL "email@gmail.com"
#define AUTHOR_PASSWORD "password"                     //App Password
#define RECIPIENT_EMAIL1 "email@hotmail.com"
#define RECIPIENT_EMAIL2 "phonenumber@tmomail.net"      //If you are a T-Mobile user
const char* newhostname = "KegMon";                     //DNS name

/* Time Variables */
const char* ntpServer = "pool.ntp.org";                     // ntp server 1 
const char* ntpServer2 = "time.nist.gov";                   // ntp server 2
const long  gmtOffset_sec = -8*3600;                        // Los Angeles Time Zone 8 hours off GMT
const int   daylightOffset_sec = 3600;                      // 1 hour change if you have day ligt savings time
