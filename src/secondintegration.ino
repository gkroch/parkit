#include "SparkJson/SparkJson.h"
//#include ""

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
int old_count;
bool web_flag;

Timer timer(10000,pushToWeb);

void setup() {
    Serial.begin(9600);
    
    // Write HIGH to turn these segments on
    // Left LED
    pinMode(C0, OUTPUT); // DP
    pinMode(C1, OUTPUT); // B
    pinMode(C2, OUTPUT); // C
    pinMode(C4, OUTPUT); // D
    pinMode(C3, OUTPUT); // E
    pinMode(C5, OUTPUT); // G
    pinMode(D0, OUTPUT); // F
    pinMode(D1, OUTPUT); // A
    
    // RIGHT LED
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
    
    ledNumber(0, true);
    ledNumber(0, false);
    timer.start();
    old_count = 0;
    web_flag=false;
    Serial.println("Completes setup");
}

    
void pushToWeb(){
    Serial.println("Set Web Flag");
    web_flag = true;
}  
    

// // Sample and determine if event occurred
void loop() {
    if(web_flag){
        Serial.println("Begin Publish");
        String eventData = String(count-old_count);
        Particle.publish("sensor_event", eventData, PRIVATE);
        old_count = count;
        web_flag = false;
        Serial.println("End Publish");
    }
     // Integrated code
    //Serial.println("Begin Loop");
    // Initialize vars
    int numReading = 0;
    int total = 0;
    int total2 = 0;
    detectmax = 0;
    detectmax2 = 0;
    
    //Serial.printf("Current Time: %d \n",millis());
    while (millis() < 8000) {
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
    
    while (sensorValue > averageValue + 30) {  //MONITOR SENSOR FOR A HIT
        timercount = millis(); //  Print out the timing instance (will reset after 50 days).  Use to compare and account for direction.
        Serial.println("in while loop");
        if(sensorValue > detectmax){
            detectmax = sensorValue;
        }
        
        // digitalWrite(ledPin, HIGH);
        delay(35);
        sensorValue = analogRead(sensorPin);
        //Serial.println(detectmax);
        //Serial.printf("Average Value: %f \n",averageValue);
        //Serial.printf("Sensor Value: %d \n",sensorValue);
    }
    
    if (detectmax > 0) {
        Serial.print("maxval");
        Serial.println(detectmax);
    }
    
    // digitalWrite(ledPin, LOW);
    
    // READ SENSOR2 VALUE
    sensorValue2 = analogRead(sensorPin2);
    float voltage2 = sensorValue2 * (5.0 / 1023.0);
    
    while (sensorValue2 > averageValue2 + 30) { //MONITOR SENSOR2 FOR A HIT
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
    //count++;
    //Serial.println("End Loop");
    //delay(3000);
    
}

// Handle the subscribe event
void myHandler(const char *event, const char *data) {
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
    
    // Set LEDs
    ledNumber(lotTotal % 10, true); // Right LED (LSD)
    ledNumber(lotTotal / 10, false); // Left LED (MSD)
}

// Write to the LEDs
void ledNumber(int value, bool left) {
    int p0;
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
    int p6;
    int p7;
    if(left) {
        p0 = C0;
        p1 = C1;
        p2 = C2;
        p3 = C3;
        p4 = C4;
        p5 = C5;
        p6 = D0;
        p7 = D1;
    }
    else {
        p0 = B0;
        p1 = B1;
        p2 = B2;
        p3 = B3;
        p4 = B4;
        p5 = B5;
        p6 = A0;
        p7 = A1;
    }
    
    switch (value) {
        case 0:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, HIGH);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, LOW);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, HIGH);
            break;
        case 1:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, LOW);
            digitalWrite(p4, LOW);
            digitalWrite(p5, LOW);
            digitalWrite(p6, LOW);
            digitalWrite(p7, LOW);
            break;
        case 2:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, LOW);
            digitalWrite(p3, HIGH);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p7, LOW);
            digitalWrite(p7, HIGH);
            break;
        case 3:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, LOW);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, LOW);
            digitalWrite(p7, HIGH);
            break;
        case 4:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, LOW);
            digitalWrite(p4, LOW);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, LOW);
            break;
        case 5:
            digitalWrite(p0, LOW);
            digitalWrite(p1, LOW);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, LOW);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, HIGH);
            break;
        case 6:
            digitalWrite(p0, LOW);
            digitalWrite(p1, LOW);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, HIGH);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, HIGH);
            break;
        case 7:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, LOW);
            digitalWrite(p4, LOW);
            digitalWrite(p5, LOW);
            digitalWrite(p6, LOW);
            digitalWrite(p7, HIGH);
            break;
        case 8:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, HIGH);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, HIGH);
            break;
        case 9:
            digitalWrite(p0, LOW);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, LOW);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, HIGH);
            break;
        default:
            digitalWrite(p0, HIGH);
            digitalWrite(p1, HIGH);
            digitalWrite(p2, HIGH);
            digitalWrite(p3, HIGH);
            digitalWrite(p4, HIGH);
            digitalWrite(p5, HIGH);
            digitalWrite(p6, HIGH);
            digitalWrite(p7, HIGH);
    }
}
