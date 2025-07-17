#include <painlessMesh.h>

Scheduler userScheduler;
painlessMesh mesh;

String message = "Secret123";
uint32_t receiverId = 123456;
long mod_exp(long base, long exp, long mod) {
  long result = 1;
  while (exp > 0) {
    if (exp % 2 == 1)
      result = (result * base) % mod;
    exp = exp >> 1;
    base = (base * base) % mod;
  }
  return result;
}

void sendMessage() {
  String encryptedMsg = "";
  for (char c : message) {
    encryptedMsg += String(mod_exp((int)c, 17, 3233)) + " ";
  }
  mesh.sendSingle(receiverId, encryptedMsg);
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u msg=%s
", from, msg.c_str());
  // Decrypt logic would go here
}

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
}

void loop() {
  mesh.update();
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 10000) {
    sendMessage();
    lastTime = millis();
  }
}