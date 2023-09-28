#include "BlueFunctions.h"


void handleFrameSave(char charBuffer[20], bool animation) {
  char nameString[7] = "";
  strcpy(nameString, &charBuffer[1]);
  String fileName;
  String dirName;
  if (!SD.exists("/anims/" + String(nameString)) && animation) { //Handle animation
    dirName = "/anims/" + String(nameString);
    fileName = "1.TXT";
    // Create a new folder
    if (SD.mkdir(dirName)) {
      Serial.println(String(nameString) + " created at: " + dirName);
    } else {
      Serial.println("Folder creation failed.");
      pCharacteristic->setValue("failed");
      pCharacteristic->notify();
      return;
    }
  } else if (animation) {
    dirName = "/anims/" + String(nameString);
    fileName = String(readFileCount(dirName) + 1) + ".TXT";
    Serial.println("File saved to " + dirName + "/" + fileName);
  } else { //Handle single drawing
    fileName = String(nameString) + ".TXT"; 
    dirName = "/drawings";
    Serial.print("New File: ");
    Serial.println(fileName);
  }

  File file = SD.open(dirName + "/" + fileName, FILE_WRITE);
  if (!file) {
    Serial.println("File creation failed!");
    pCharacteristic->setValue("failed");
    pCharacteristic->notify();
    return;
  }
  for (int i = 0; i < 256; i++) {
    CRGB color = leds[i];
    uint32_t hexColor = ((uint32_t)color.r << 16) | ((uint32_t)color.g << 8) | (uint32_t)color.b;
    file.write((uint8_t*)&hexColor, sizeof(hexColor));
  }
  file.close();
  Serial.println("Color data saved to SD card.");
  pCharacteristic->setValue("success");
  pCharacteristic->notify();
}


void handleFrameDelete(char charBuffer[20]) {
  char nameString[7] = "";
  strcpy(nameString, &charBuffer[1]);
  


  if (SD.remove("/drawings/" + String(nameString) + ".TXT")) {
    Serial.println("File deleted: " + String(nameString));
      pCharacteristic->setValue("success");
      pCharacteristic->notify();
    return;
  }
  pCharacteristic->setValue("failed");
  pCharacteristic->notify();
}



void handleAnimDelete(char charBuffer[20]) {
  char nameString[7] = "";
  strcpy(nameString, &charBuffer[1]);
  File dir = SD.open("/ANIMS/" + String(nameString));

  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }
    String entryName = entry.name();
    entry.close();
    SD.remove("/ANIMS/" + String(nameString) + "/" + entryName);
  }
  dir.close();
  if (SD.rmdir("/ANIMS/" + String(nameString))) {
    Serial.println("Folder deleted: " + String(nameString));
    pCharacteristic->setValue("success");
    pCharacteristic->notify();
    return;
  }
  Serial.println("Folder delete failed: " + String(nameString));
  pCharacteristic->setValue("failed");
  pCharacteristic->notify();
}



int readFileCount(String directory) {
  int result = 0;
  File dir = SD.open(directory);
  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      break; // No more files
    }
    result++;
    file.close();
  }
  dir.close();
  return result;
}




void sendFileNames(String dir, bool anims) {
  File root = SD.open(dir);
  while (true) {
      File file = root.openNextFile();
      if (!file) {
        break; // No more files
      }
      char fileName[11]; // maximum length of file name + extension
      strcpy(fileName, file.name()); // copy the file name to a new char array
      int extensionIndex = strlen(fileName) - 4; // find the position of the start of the extension
      if (!anims) {
        fileName[extensionIndex] = '\0'; // add a null terminator to the end of the new string
      }
      if (anims) {
        // bluetooth.print("." + String(fileName) + ",");
      } else {
        // bluetooth.print(String(fileName) + ",");
      }
      Serial.println(fileName);
      file.close();
    }
    root.close();
}