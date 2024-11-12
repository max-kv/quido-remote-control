// Negate key state and create command for Quido
void buttonPushAction(int key)
{
  char cmdBuffer[64];
  memset(packetBuffer, 0, sizeof(packetBuffer));
  if (key == 4){
    killEmAll();
  }
  else{
    if (RemoteStates[key] == false) {
      RemoteStates[key] = true;
      sprintf(cmdBuffer,"*B1OS%dH\r",key);
      sendPacketUDP(cmdBuffer);
    }
    else {
      RemoteStates[key] = false;
      sprintf(cmdBuffer,"*B1OS%dL\r",key);
      sendPacketUDP(cmdBuffer);
    }
  }
}

// If button on keyboard is pressed, then do some action
int getKeyPressed()
{
  for (byte i = 0; i < KEY_NUM; i++)
    keypad_1x4[i].loop();  // MUST call the loop() function first

  for (byte i = 0; i < KEY_NUM; i++) {
    // get key state after debounce
    int key_state = keypad_1x4[i].getState();  // the state after debounce

    if (keypad_1x4[i].isPressed())
      return (i + 1);
  }
  return 0;
}