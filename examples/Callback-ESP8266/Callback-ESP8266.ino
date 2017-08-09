
#include <FS.h>
#include <untar.h>

#undef TAR_VERBOSE

#define PIN D0

#define FILENAME "/test.tar"

Tar<FS> tar(&SPIFFS);
bool write = false;
bool printFile(char* name) {
	Serial.print(name);
	if (strcmp(name, "create.txt")) {
		Serial.println();
		write = true;
		return true;
	}
	Serial.println(" -  SKIP");
	return false;
}
void blinkWrite(char* data, size_t s) {
	if (!write) return;
	digitalWrite(PIN, !digitalRead(PIN));	
}

void eof() {
	write = false;
}

void setup() {
  Serial.begin(74880);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  SPIFFS.begin();
  tar.onFile(printFile);
  tar.onData(blinkWrite);
  tar.onEof(eof);
  File f = SPIFFS.open(FILENAME, "r");
  if (f) {
    tar.stream((Stream*)&f);
    tar.dest("/");
	  tar.extract();
  } else {
    Serial.println("Error open .tar file");
  }
}

void loop() {

}
