int num1;
int num2;
char op;
float sum, sub, pro, divi;

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  delay(10);

  String input = Serial.readStringUntil('\n');
  input.trim();



  sscanf(input.c_str(),  "%d %c %d", &num1, &op,&num2);

  Serial.println(num1);
  Serial.println(op);
  Serial.println(num2);

  if (op == '+'){
    sum = (num1  + num2);
    Serial.print("The sum of the two number is:"  );
    Serial.println(sum); }
    else if  (op == '-'){
      sub = (num1 - num2);
      Serial.print("The sub of the two number is:  ");
      Serial.println(sub);
    



    }
    else if (op== '*')
    {
      pro = (num1 * num2);
      Serial.print("The product of two numbers is :  ");
      Serial.println (pro);

    }
    else if (op=='/'){
       if (num2== 0){
        Serial.println("Invalid calculation");
       }else{ 
        divi = (num1/num2);
      Serial.println("The division of two number is :  ");
      Serial.println(divi);
    }}
    else{
      Serial.println("invalid operation");
    }

    Serial.println("enter two numbers");
    }
 


