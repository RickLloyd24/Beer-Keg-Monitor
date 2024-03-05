/* Send an eMail */
/* flag: 1 = email, 2 = text message, 3 = both */
void eMail(String textMsg, int flag) {
  #define SMTP_PORT esp_mail_smtp_port_465 // port 465 is not available for Outlook.com
  #define SMTP_HOST "smtp.gmail.com"
  #define SUBJECT "Alert from Keg Monitor"
//  #define AUTHOR_EMAIL ""
//  #define AUTHOR_PASSWORD ""                  //App Password
//  #define RECIPIENT_EMAIL1 ""
//  #define RECIPIENT_EMAIL2 ""

  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(0);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  /** Assign your host name or you public IPv4 or IPv6 only
   * as this is the part of EHLO/HELO command to identify the client system
   * to prevent connection rejection.
   * If host name or public IP is not available, ignore this or
   * use generic host "mydomain.net".
   *
   * Assign any text to this option may cause the connection rejection.
   */
  config.login.user_domain = F("mydomain.net");

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  /* The full message sending logs can now save to file */
  /* Since v3.0.4, the sent logs stored in smtp.sendingResult will store only the latest message logs */
  // config.sentLogs.filename = "/path/to/log/file";
  // config.sentLogs.storage_type = esp_mail_file_storage_type_flash;

  /** In ESP32, timezone environment will not keep after wake up boot from sleep.
   * The local time will equal to GMT time.
   *
   * To sync or set time with NTP server with the valid local time after wake up boot,
   * set both gmt and day light offsets to 0 and assign the timezone environment string e.g.

     config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
     config.time.gmt_offset = 0;
     config.time.day_light_offset = 0;
     config.time.timezone_env_string = "JST-9"; // for Tokyo

   * The library will get (sync) the time from NTP server without GMT time offset adjustment
   * and set the timezone environment variable later.
   *
   * This timezone environment string will be stored to flash or SD file named "/tze.txt"
   * which set via config.time.timezone_file.
   *
   * See the timezone environment string list from
   * https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
   *
   */

  /* Declare the message class */
  //SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("ESP Mail");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F(SUBJECT);
  if (flag == 1) {
    message.addRecipient(F("Wemos"), RECIPIENT_EMAIL1);
  }  
  else if (flag == 2)  {
    message.addRecipient(F("SMS"), RECIPIENT_EMAIL2);
  }
  else {
    message.addRecipient(F("Wemos"), RECIPIENT_EMAIL1);  
    message.addRecipient(F("SMS"), RECIPIENT_EMAIL2);
  }

  message.text.content = textMsg;

  /** If the message to send is a large string, to reduce the memory used from internal copying  while sending,
   * you can assign string to message.text.blob by cast your string to uint8_t array like this
   *
   * String myBigString = "..... ......";
   * message.text.blob.data = (uint8_t *)myBigString.c_str();
   * message.text.blob.size = myBigString.length();
   *
   * or assign string to message.text.nonCopyContent, like this
   *
   * message.text.nonCopyContent = myBigString.c_str();
   *
   * Only base64 encoding is supported for content transfer encoding in this case.
   */

  /** The Plain text message character set e.g.
   * us-ascii
   * utf-8
   * utf-7
   * The default value is utf-8
   */
  message.text.charSet = F("us-ascii");

  /** The content transfer encoding e.g.
   * enc_7bit or "7bit" (not encoded)
   * enc_qp or "quoted-printable" (encoded)
   * enc_base64 or "base64" (encoded)
   * enc_binary or "binary" (not encoded)
   * enc_8bit or "8bit" (not encoded)
   * The default value is "7bit"
   */
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  // If this is a reply message
  // message.in_reply_to = "<parent message id>";
  // message.references = "<parent references> <parent message id>";

  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
   */
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

  // message.response.reply_to = "someone@somemail.com";
  // message.response.return_path = "someone@somemail.com";

  /** The Delivery Status Notifications e.g.
   * esp_mail_smtp_notify_never
   * esp_mail_smtp_notify_success
   * esp_mail_smtp_notify_failure
   * esp_mail_smtp_notify_delay
   * The default value is esp_mail_smtp_notify_never
   */
  // message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  //message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

  // For Root CA certificate verification (ESP8266 and ESP32 only)
  // config.certificate.cert_data = rootCACert;
  // or
  // config.certificate.cert_file = "/path/to/der/file";
  // config.certificate.cert_file_storage_type = esp_mail_file_storage_type_flash; // esp_mail_file_storage_type_sd
  // config.certificate.verify = true;

  // The WiFiNINA firmware the Root CA certification can be added via the option in Firmware update tool in Arduino IDE

  /* Connect to server with the session config */

  // Library will be trying to sync the time with NTP server if time is never sync or set.
  // This is 10 seconds blocking process.
  // If time reading was timed out, the error "NTP server time reading timed out" will show via debug and callback function.
  // You can manually sync time by yourself with NTP library or calling configTime in ESP32 and ESP8266.
  // Time can be set manually with provided timestamp to function smtp.setSystemTime.

  /* Connect to the server */
  if (!smtp.connect(&config))
  {
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  /** Or connect without log in and log in later

     if (!smtp.connect(&config, false))
       return;

     if (!smtp.loginWithPassword(AUTHOR_EMAIL, AUTHOR_PASSWORD))
       return;
  */

  if (!smtp.isLoggedIn())
  {
    Serial.println("\nNot yet logged in.");
  }
  else
  {
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

  // to clear sending result log
  // smtp.sendingResult.clear();}
}
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)
      
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}
