int led = 15;
void setup()
{
  pinMode(15, OUTPUT);
}
void loop() 
{
  digitalWrite(15,1);
  delay(2000);// Milliseconds
  digitalWrite(15,0);
  delay(1000);
}