#include "SparkJson/SparkJson.h"

int led = D7;  // The on-board LED
int lotTotal = 0;

//CODE TO BE INTEGRATED
int sensorPin = A4; // Analog
int sensorPin2 = A5; // Analog
// int ledPin = 11;
// int ledPin2 = 12;
int sensorValue = 0;
int sensorValue2 = 0;
float averageValue = 0;
float averageValue2 = 0;
int count = 0;
int timercount = 0;
int timercount2 = 0;
int detectmax;
int detectmax2;

void setup() {
    Serial.begin(9600);
    
    // Write HIGH to turn these segments on
    
    // LED 1
    pinMode(C0, OUTPUT); // DP
    pinMode(C1, OUTPUT); // B
    pinMode(C2, OUTPUT); // C
    pinMode(C3, OUTPUT); // D
    pinMode(C4, OUTPUT); // E
    pinMode(C5, OUTPUT); // G
    pinMode(D0, OUTPUT); // F
    pinMode(D1, OUTPUT); // A
    
    // LED 2
    pinMode(B0, OUTPUT); // DP
    pinMode(B1, OUTPUT); // B
    pinMode(B2, OUTPUT); // C
    pinMode(B3, OUTPUT); // D
    pinMode(B4, OUTPUT); // E
    pinMode(B5, OUTPUT); // G
    pinMode(A0, OUTPUT); // F
    pinMode(A1, OUTPUT); // A
    
    // Subscribe to sensor_event stream
    Particle.subscribe("get_sensor_event", myHandler, MY_DEVICES);
    
    
    // Integrated code
    // pinMode(ledPin, OUTPUT);
    digitalWrite(sensorPin, HIGH);  // TODO Ask Sergio
    // pinMode(ledPin2, OUTPUT);
    digitalWrite(sensorPin2, HIGH);
}

      
void loop() {
    
    // Publish sensor_event to stream
    String eventData = "1";
    Particle.publish("sensor_event", eventData, PRIVATE);

    // Integrated code

    // Initialize vars
    int numReading = 0;
    int total = 0;
    int total2 = 0;
    detectmax = 0;
    detectmax2 = 0;
    
    while (millis() < 2000) {
    numReading++;
        sensorValue = analogRead(sensorPin);
        //averageValue += ( (float)sensorValue - averageValue )/10.f; // SET UP FIRST SENSOR
        total += sensorValue;
        averageValue = total / numReading;
        Serial.println(averageValue);
        
        sensorValue2 = analogRead(sensorPin2);
        //averageValue2 +=  ((float)sensorValue2 - averageValue2 )/10.f; // SET UP SECOND SENSOR
        total2 += sensorValue2;
        averageValue2 = total2 / numReading;
        Serial.println(averageValue2);
    }

    // READ SENSOR VALUE
    sensorValue = analogRead(sensorPin);
    float voltage = sensorValue * (5.0 / 1023.0);
    
    while (sensorValue > averageValue + 5) {  //MONITOR SENSOR FOR A HIT
        timercount = millis(); //  Print out the timing instance (will reset after 50 days).  Use to compare and account for direction.
        Serial.println("in while loop");
        if(sensorValue > detectmax){
            detectmax = sensorValue;
        }
        
        // digitalWrite(ledPin, HIGH);
        delay(35);
        sensorValue = analogRead(sensorPin);
        //Serial.println(detectmax);
    }
    
    if (detectmax > 0) {
        Serial.print("maxval");
        Serial.println(detectmax);
    }
    
    // digitalWrite(ledPin, LOW);
    
    // READ SENSOR2 VALUE
    sensorValue2 = analogRead(sensorPin2);
    float voltage2 = sensorValue2 * (5.0 / 1023.0);
    
    while (sensorValue2 > averageValue2 + 5) { //MONITOR SENSOR2 FOR A HIT
        timercount2 = millis(); //  Print out the timing instance (will reset after 50 days).  Use to compare and account for direction.
        if(sensorValue2 > detectmax2){
            detectmax2 = sensorValue2;
        }
        // digitalWrite(ledPin2, HIGH);
        delay(25);
        sensorValue2 = analogRead(sensorPin2);
        //Serial.println(detectmax);
    }
    
    if (detectmax2 > 0) {
        Serial.print("maxval");
        Serial.println(detectmax2);
    }
    
    
    // digitalWrite(ledPin2, LOW);
    
    // Function to compare the two sensor times.
    // Will increment if timercount < timercount2 and decrement when timercount > timercount2
    
    //TO DO, increase robustness, only want time to count if timer & timer2 are != 0, because if 0 means wasnt actually hit?
    if (timercount2 - timercount != 0) {
        if ((timercount2 - timercount) < 3000 && (timercount2 - timercount) > 0) {
          count++;
          Serial.println(count);
          timercount = 0;
          timercount2 = 0;
        }
        else if (3000 > (timercount - timercount2) && (timercount - timercount2) > 0) {
          count--;
          Serial.println(count);
          timercount = 0;
          timercount2 = 0;
        }
    }
}

void myHandler(const char *event, const char *data) {
    // Handle the integration response
    Serial.println(data);
    
    char * jsonData = strdup(data);
  
    // Parse JSON
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(jsonData);
    const char* time   = root["timestamp"];
    int sensorNet      = root["sensor_net"];
    int nodeChange     = root["node_change"];
    lotTotal           = root["lot_total"];
    
    Serial.print("The Lot Total is: ");
    Serial.println(lotTotal);
    
    digitalWrite(D3, HIGH);
    ledNumber(lotTotal % 10);
}

void ledNumber(int value) {
    switch (value) {
        case 0:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, HIGH);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, LOW);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
            break;
        case 1:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, LOW);
            digitalWrite(C4, LOW);
            digitalWrite(C5, LOW);
            digitalWrite(D0, LOW);
            digitalWrite(D1, LOW);
            break;
        case 2:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, LOW);
            digitalWrite(C3, HIGH);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, LOW);
            digitalWrite(D1, HIGH);
            break;
        case 3:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, LOW);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, LOW);
            digitalWrite(D1, HIGH);
            break;
        case 4:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, LOW);
            digitalWrite(C4, LOW);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, LOW);
            break;
        case 5:
            digitalWrite(C0, LOW);
            digitalWrite(C1, LOW);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, LOW);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
            break;
        case 6:
            digitalWrite(C0, LOW);
            digitalWrite(C1, LOW);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, HIGH);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
            break;
        case 7:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, LOW);
            digitalWrite(C4, LOW);
            digitalWrite(C5, LOW);
            digitalWrite(D0, LOW);
            digitalWrite(D1, HIGH);
            break;
        case 8:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, HIGH);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
            break;
        case 9:
            digitalWrite(C0, LOW);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, LOW);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
            break;
        default:
            digitalWrite(C0, HIGH);
            digitalWrite(C1, HIGH);
            digitalWrite(C2, HIGH);
            digitalWrite(C3, HIGH);
            digitalWrite(C4, HIGH);
            digitalWrite(C5, HIGH);
            digitalWrite(D0, HIGH);
            digitalWrite(D1, HIGH);
    }
}
