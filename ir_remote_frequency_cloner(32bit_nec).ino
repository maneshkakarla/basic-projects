#include <IRremote.hpp>

// IR Pins
#define receive_pin 15  // reciever pin
#define sende_pin 4     // IR transmitter pin
#define FEEDBACK 19   // LED feedback pin
#define ledfeedback 2
// Trigger Pins
#define enable 13     // enable pin
#define button_1 12        
#define button_2 14        
#define button_3 27       
#define button_4 26        
#define button_5 25        
#define button_6 33      
#define button_7 32  //     itor

uint32_t code1, code2, code3, code4, code5, code6, code7;

void blinkLED(int times, int interval) {
  for(int i = 0; i < times; i++) {
    digitalWrite(ledfeedback, HIGH);
    delay(interval);
    digitalWrite(ledfeedback, LOW);
    if(i < times - 1) delay(interval); // No delay after last blink
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);  // Wait for Serial Mon


  // Initialize IR Receiver
  IrReceiver.begin(receive_pin, ENABLE_LED_FEEDBACK);
  
  // Initialize IR Sender
  IrSender.begin(sende_pin, true, FEEDBACK);

  // Set trigger pins as INPUT_PULLUP (default HIGH)
  pinMode(enable, INPUT_PULLUP);
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);
  pinMode(button_3, INPUT_PULLUP);
  pinMode(button_4, INPUT_PULLUP);
  pinMode(button_5, INPUT_PULLUP);
  pinMode(button_6, INPUT_PULLUP);
  pinMode(button_7, INPUT_PULLUP);
  pinMode(ledfeedback, OUTPUT);

  Serial.println("IR remote is ready");
}

void loop() {
  // *** RECIEVR CODE ***/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.decodedRawData != 0) {
      uint32_t receivedCode = IrReceiver.decodedIRData.decodedRawData;
      bool codeStored = false;

      // Check which pin combination is pressed and store the code
      if (digitalRead(enable) == LOW && digitalRead(button_1) == LOW) {
        code1 = receivedCode;
        Serial.print("Stored 1st: 0x");
        Serial.println(code1, HEX);
        codeStored = true;
      }
      else if (digitalRead(enable) == LOW && digitalRead(button_2) == LOW) {
        code2 = receivedCode;
        Serial.print("Stored 2nd: 0x");
        Serial.println(code2, HEX);
        codeStored = true;
      }
      else if (digitalRead(enable) == LOW && digitalRead(button_3) == LOW) {
        code3 = receivedCode;
        Serial.print("Stored 3rd: 0x");
        Serial.println(code3, HEX);
        codeStored = true;
      }
      else if (digitalRead(enable) == LOW && digitalRead(button_4) == LOW) {
        code4 = receivedCode;
        Serial.print("Stored 4th: 0x");
        Serial.println(code4, HEX);
        codeStored = true;
      }
      else if (digitalRead(enable) == LOW && digitalRead(button_5) == LOW) {
        code5 = receivedCode;
        Serial.print("Stored 5th: 0x");
        Serial.println(code5, HEX);
        codeStored = true;
      }
      else if (digitalRead(enable) == LOW && digitalRead(button_6) == LOW) {
        code6 = receivedCode;
        Serial.print("Stored 6th: 0x");
        Serial.println(code6, HEX);
        codeStored = true;
      }
      else if (digitalRead(enable) == LOW && digitalRead(button_7) == LOW) {
        code7 = receivedCode;
        Serial.print("Stored 7th: 0x");
        Serial.println(code7, HEX);
        codeStored = true;
      }

      if (codeStored) {
        blinkLED(4, 100); // Blink 4 times with 250ms interval when code is stored
      }
    }
    IrReceiver.resume();  // Ready for next signal
  }
//...............................................................................................................................................................................................
  //  // *** SENDER CODE ***
  if (digitalRead(button_1) == LOW && digitalRead(enable) == HIGH) {
    if (code1 != 0) {
      Serial.print("Sending 1st: 0x");
      Serial.println(code1, HEX);
      IrSender.sendNECRaw(code1, 0);
      blinkLED(1, 50); // Single blink when code is sent
      delay(200);
    }
  }
  
  if (digitalRead(button_2) == LOW && digitalRead(enable) == HIGH) {
    if (code2 != 0) {
      Serial.print("Sending 2nd: 0x");
      Serial.println(code2, HEX);
      IrSender.sendNECRaw(code2, 0);
      blinkLED(1, 50);
      delay(200);
    }
  }
  
  if (digitalRead(button_3) == LOW && digitalRead(enable) == HIGH) {
    if (code3 != 0) {
      Serial.print("Sending 3rd: 0x");
      Serial.println(code3, HEX);
      IrSender.sendNECRaw(code3, 0);
      blinkLED(1, 50);
      delay(200);
    }
  }
  
  if (digitalRead(button_4) == LOW && digitalRead(enable) == HIGH) {
    if (code4 != 0) {
      Serial.print("Sending 4th: 0x");
      Serial.println(code4, HEX);
      IrSender.sendNECRaw(code4, 0);
      blinkLED(1, 50);
      delay(200);
    }
  }
  
  if (digitalRead(button_5) == LOW && digitalRead(enable) == HIGH) {
    if (code5 != 0) {
      Serial.print("Sending 5th: 0x");
      Serial.println(code5, HEX);
      IrSender.sendNECRaw(code5, 0);
      blinkLED(1, 50);
      delay(200);
    }
  }
  
  if (digitalRead(button_6) == LOW && digitalRead(enable) == HIGH) {
    if (code6 != 0) {
      Serial.print("Sending 6th: 0x");
      Serial.println(code6, HEX);
      IrSender.sendNECRaw(code6, 0);
      blinkLED(1, 50);
      delay(200);
    }
  }
  
  if (digitalRead(button_7) == LOW && digitalRead(enable) == HIGH) {
    if (code7 != 0) {
      Serial.print("Sending 7th: 0x");
      Serial.println(code7, HEX);
      IrSender.sendNECRaw(code7, 0);
      blinkLED(1, 50);
      delay(200);
    }
  }
}