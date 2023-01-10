const int relay1Pin = 18;   // pin for control signal of first relay gate
const int relay2Pin = 19;   // pin for control signal of second relay gate
const int outputPin = 15;   // pin for output of Relay Based NAND gate

int relayDelay = 100; //Determines how long the script pauses after setting the control signals for the relays

// NAND function that returns the output of the NAND gate
// based on the inputs a and b
int NAND(int a, int b)
{
  digitalWrite(relay1Pin, a);   // set control signal for first relay
  delay(relayDelay);   // delay for 100 ms
  digitalWrite(relay2Pin, b);   // set control signal for second relay
  delay(relayDelay);   // delay for 100 ms
  return digitalRead(outputPin);   // read and return output of NAND gate
}

int OR(int a, int b) {
  return NAND(NAND(a, a), NAND(b, b));
}


int AND(int a, int b) {
  int tempVar0 = NAND(a,b);
  return NAND(tempVar0, tempVar0);
}

int NOR(int a, int b) {
  int tempVar0 = NAND(a, a);
  int tempVar1 = NAND(b, b);
  tempVar0 = NAND(tempVar0, tempVar1);
  return NAND(tempVar0, tempVar0);    
}

int XOR(int a, int b) {
  int firstNAND = NAND(a,b);
  int secondNAND0 = NAND(firstNAND, a);
  int secondNAND1 = NAND(secondNAND0, b);
  return NAND(secondNAND0, secondNAND1);
}

int NOT(int a) {
  return NAND(a, a);
}

int* halfAdder(int a, int b) {
  int* results = new int[2]; // allocate memory for the array
  results[0] = AND(a, b);
  results[1] = XOR(a, b);    
  return results;  
}

int* fullAdder(int a, int b, int carryIn) {
  int* halfAdderResults0 = halfAdder(a, b); //index 0 is sum, index 1 is carr
  Serial.println("HalfAdderResults0");
  int* halfAdderResults1 = halfAdder(halfAdderResults0[0], carryIn);
  Serial.println("HalfAdderResults1");
  int carryOut = OR(halfAdderResults0[1],halfAdderResults1[1]);
  int sum = halfAdderResults1[0];
  delete[] halfAdderResults0;
  delete[] halfAdderResults1;
  int* results = new int[2];
  results[0] = sum;
  results[1] = carryOut;
return results;
}

int* rippleAdder(int* a, int* b, int numBits) {
  int* result = new int[numBits + 1];
  int carry = 0;
  for (int i = numBits - 1; i >= 0; i--) {
    int* fullAdderResult = fullAdder(a[i], b[i], carry);
    result[i + 1] = fullAdderResult[0];
    carry = fullAdderResult[1];
    delete[] fullAdderResult;
  }
  result[0] = carry;
  return result;
}


void setup()
{
  Serial.begin(9600);
  pinMode(relay1Pin, OUTPUT);   // set relay1Pin as output
  pinMode(relay2Pin, OUTPUT);   // set relay2Pin as output
  pinMode(outputPin, INPUT_PULLDOWN);   // set outputPin as input
}

void testRippleAdder() {
  int numBits = 8; // Assume 8-bit integers

  // Test cases
  int testCases[][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {127, 128}, {255, 255}};
  for (int i = 0; i < 6; i++) {
    int a = testCases[i][0];
    int b = testCases[i][1];
    int* aBits = new int[numBits];
    int* bBits = new int[numBits];

    // Convert a and b to arrays of bits
    for (int j = numBits - 1; j >= 0; j--) {
      aBits[j] = a % 2;
      bBits[j] = b % 2;
      a /= 2;
      b /= 2;
    }

    int* result = rippleAdder(aBits, bBits, numBits);
    Serial.print("Test case ");
    Serial.print(i + 1);
    Serial.print(": a = ");
    Serial.print(testCases[i][0]);
    Serial.print(", b = ");
    Serial.println(testCases[i][1]);
    for (int j = 0; j < numBits + 1; j++) {
      Serial.println(result[j]);
    }

    delete[] aBits;
    delete[] bBits;
    delete[] result;
  }
}

void loop()
{
int a = 10;
int b = 5;
testRippleAdder();
}
