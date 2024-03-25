void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

String command = "efxiii-0-59-100-0000ff";
String FXID = "";
String start = "";
String end = "";
String brightness = "";
String color1 = "";

void readCommand();
String __readCommandUntilTerminator(String commandToRead, uint8_t startFromWhere, char terminator = '-');

void loop() {
  // put your main code here, to run repeatedly:
  // readCommand();
  unsigned long hexValue = strtoul(color1.c_str(), NULL, 16);
  uint32_t finalValue = hexValue;
  FXID = __readCommandUntilTerminator(command, 0);
  Serial.print(FXID);
  Serial.print(" ");
  Serial.print(start);
  Serial.print(" ");
  Serial.print(end);
  Serial.print(" ");
  Serial.print(brightness);
  Serial.print(" ");
  Serial.print(color1);
  Serial.print("\t");
  Serial.println(finalValue);

  delay(1000);
}

String __readCommandUntilTerminator(String commandToRead, uint8_t startFromWhere, char terminator)
{
    uint8_t iterate;
    String  commandToReturn;

    for (iterate = startFromWhere;
         iterate < commandToRead.length();
         iterate++)
    {
        if (commandToRead[iterate] == terminator)
            break;
        commandToReturn += commandToRead[iterate];
    }

    return commandToReturn;
}

void readCommand()
{
  int iterate = 0;
  FXID = "";
  start = "";
  end = "";
  brightness = "";
  color1 = "";
  // for (iterate; iterate < command.length(); iterate++)
  // {
  //   if (command[iterate] == '-')
  //     break;
  //   FXID += command[iterate];
  // }
  // iterate++;
  // for (iterate; iterate < command.length(); iterate++)
  // {
  //   if (command[iterate] == '-')
  //     break;
  //   start += command[iterate];
  // }
  // iterate++;
  // for (iterate; iterate < command.length(); iterate++)
  // {
  //   if (command[iterate] == '-')
  //     break;
  //   end += command[iterate];
  // }
  // iterate++;
  // for (iterate; iterate < command.length(); iterate++)
  // {
  //   if (command[iterate] == '-')
  //     break;
  //   brightness += command[iterate];
  // }
  // iterate++;
  // for (iterate; iterate < command.length(); iterate++)
  // {
  //   if (command[iterate] == '-')
  //     break;
  //   color1 += command[iterate];
  // }
  // iterate++;
}