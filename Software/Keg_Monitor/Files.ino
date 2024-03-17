/* Read the Configuration File */
boolean ReadConfigFile() {
   File file = LittleFS.open(ConfigFN, "r");
   if(!file){
    Serial.println("No Saved Data!");
    return false;
   }
   while(file.available()){
     String s = file.readString();
     Serial.println("Config file data "); Serial.println (s);
     for (int j = 0; j < 5; j++) {
       EmptyKW[j] = SubString(s).toFloat();
     }
     for (int j = 0; j < 5; j++) {
       FullKegWeight[j] = SubString(s).toFloat();
     }
     for (int j = 0; j < 5; j++) {
       FullKeg[j] = SubString(s).toInt();
     }
     for (int j = 0; j < 5; j++) {
       EmptyKeg[j] = SubString(s).toInt();
     }
     for (int j = 0; j < 5; j++) {
       KeggedDate[j] = SubString(s);
     }
     for (int j = 0; j < 5; j++) {
       Alcohol[j] = SubString(s).toFloat();
     }
     for (int j = 0; j < 5; j++) {
       BeerNames[j] = SubString(s);
     }
     TempBias[1] = SubString(s).toFloat();
     TempBias[2] = SubString(s).toFloat();
     TempBias[3] = SubString(s).toFloat();
     Tempgoal = SubString(s).toInt();
     ClearTempStatistics();
     int i = SubString(s).toInt();
     if (i == 19772) return true; else return false;
   }   
   file.close();
}

void WriteConfigFile (void) {
  String s ="Empty_Keg_Weight:    ";
  int spaces = 11;
  for (int j = 0; j < 5; j++) {
    s = s + FSLRJ(String(EmptyKW[j], 2), spaces) + ",";
  }
  s = s + "\nFull_Keg_Weight:     ";
  for (int j = 0; j < 5; j++) {
    s = s + FSLRJ(String(FullKegWeight[j], 1), spaces) + ",";
  }
  s = s + "\nFull_Keg_Raw:        ";
  for (int j = 0; j < 5; j++) {
    s = s + FSLRJ(String(FullKeg[j]), spaces) + ",";  
  }
  s = s + "\nEmpty_Keg_Raw:       ";
  for (int j = 0; j < 5; j++) {
    s = s + FSLRJ(String(EmptyKeg[j]), spaces) + ",";  
  } 
  s = s + "\nDate_Kegged:         ";
  for (int j = 0; j < 5; j++) {
    s = s + FSLRJ(String(KeggedDate[j]), spaces) + ",";  
  }
  s = s + "\nAlcohol:             ";
  for (int j = 0; j < 5; j++) {
    s = s + FSLRJ(String(Alcohol[j], 1), spaces) + ",";  
  }
  s = s + "\nTitles:              ";
  for (int j = 0; j < 5; j++) {
    s = s + BeerNames[j] + ", "; 
  }
  s = s + "\nTemp_Bias_goal:       ";
  s = s + String(TempBias[1], 1) + ", " + String(TempBias[2], 1) + ", " + String(TempBias[3], 1) + ", ";
  s = s + String(Tempgoal) + ", ";
  s = s + "\nValidation:         19772, ";
  s = s + "\n";
  //LittleFS.remove(ConfigFN); 
  File file = LittleFS.open(ConfigFN, "w");
  file.print(s);
  Serial.println ("Data to be Written to Config File");
  Serial.println(s);
  file.close();
  return;
}
/* Display Config file data */
void PrintConfigData (void) {
   Serial.println ("Variable Values:");
   Serial.print ("Empty_Keg_Weight: ");
   for (int i = 0; i<5; i++) {
     Serial.print ("    "); Serial.print (EmptyKW[i]); Serial.print (",  ");
   }
   Serial.println ();  

   Serial.print ("Keg_Weight:       ");
   for (int i = 0; i<5; i++) {
     Serial.print ("    "); Serial.print (FullKegWeight[i]); Serial.print (",  ");
   }
   Serial.println ();  

   Serial.print ("Full_Keg_Raw:        ");
   for (int i = 0; i<5; i++) {
     Serial.print (FullKeg[i]); Serial.print (",  ");
   }
   Serial.println ();  

   Serial.print ("Empty_Keg_Raw:       ");
   for (int i = 0; i<5; i++) {
     Serial.print (EmptyKeg[i]); Serial.print (",  ");
   }
   Serial.println ();  
   Serial.print ("Date_Kegged:         ");
   for (int i = 0; i<5; i++) {
     Serial.print (KeggedDate[i]); Serial.print (",  ");
   }
   Serial.println ();  
   Serial.print ("Alcohol:             ");
   for (int i = 0; i<5; i++) {
     Serial.print (Alcohol[i]); Serial.print (",  ");
   }
   Serial.println ();  
   Serial.print ("Keg_Titles:          ");
   for (int i = 0; i<5; i++) {
     Serial.print (BeerNames[i]); Serial.print (",  ");
   }
   Serial.println (); 
   Serial.print("TemperatureBias:     "); Serial.print(TempBias[1]); Serial.print (",  ");
   Serial.print(TempBias[2]);Serial.print(",  ");
   Serial.print(TempBias[3]);Serial.print(",  ");
   Serial.print(Tempgoal); Serial.println(",  ");
}
/* List files */
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
/* Find the Sub String */
String SubString (String s) {
  static int ptr = 0;
  
   int start = 0; int last = 0; int i; boolean flag = true;
   //Serial.print("ptr ");Serial.print(ptr); Serial.print(" s.length "); Serial.println(s.length());
   for (i = ptr; i < s.length(); i++) {
     //Serial.print(s[i]);
     if ((s[i] != ' ') && (flag == true)) {
       start = i; flag = false;
       //Serial.print("Start "); Serial.print(s[i]); Serial.print("  "); Serial.println(start);
     }
     if (s[i] == ':')  {
       flag = true;  
       //Serial.println("flag reset");
     }
     if (s[i] == ',') {
       last = i;
       ptr = last + 1;
       //Serial.print("End "); Serial.println(last);
       break;
     } 
   } 
   String v = s.substring(start, last); 
   //Serial.print("Returned String: "); Serial.println(v);
   return v;
}
/* Fixed String Length Right Justified*/
String FSLRJ (String v, int len) {
  String blank = "                           ";
  if (v.length() >= len) {                                          /* check if string is too long */
    Serial.print("String is too long "); Serial.print(v); Serial.print(" Length "); Serial.println(v.length());
    v = v.substring(0, len - 1);                                    /* cut off end of string */
    return v;                                                       /* return truncated string */
  } 
  v = blank.substring(0, len - v.length()) + v;                     /* Pad spaces in front */
  return v;                                                         /* return fixed string */
}
