// Import the library
#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 5, 4, 3, 2); // Define the pins that are used for the display

// Cat
byte cat[8]{B00100,
            B00111,
            B00111,
            B10110,
            B11110,
            B01110,
            B01010,
            B01010};

// Banana
byte banana[8]{B00010,
               B00010,
               B00111,
               B00111,
               B00111,
               B01110,
               B11110,
               B11100};

// Grapes
byte grapes[8]{B00000,
               B00100,
               B00100,
               B11111,
               B11111,
               B01110,
               B01110,
               B00100};

// Apple
byte apple[8]{B00100,
              B00110,
              B00100,
              B11111,
              B11111,
              B11111,
              B11111,
              B11111};

// Fish
byte fish[8]{B00000,
             B11101,
             B10111,
             B11111,
             B01111,
             B11101,
             B00000,
             B00000};

// Hurdle
byte hurdle[8]{B00000,
               B00000,
               B00010,
               B00010,
               B00010,
               B00010,
               B00010,
               B01110};

// Tree (Upper Part)
byte upperTree[8]{B00000,
                  B00000,
                  B00000,
                  B00000,
                  B00000,
                  B00100,
                  B01110,
                  B10101};

// Tree (Lower Part)
byte lowerTree[8]{B00100,
                  B01110,
                  B10101,
                  B00100,
                  B01110,
                  B10101,
                  B00100,
                  B00100};

// Button
int button1 = 6;
int button2 = 7;

// RGB
int Red = 9;
int Blue = 10;
int Green = 11;
int flashTime = 300;

// Objects
int catChar = 3;
int bananaChar = 4;
int grapesChar = 5;
int appleChar = 6;
int fishChar = 7;
int hurdleChar = 8;
int upperTreeChar = 9;
int lowerTreeChar = 10;

boolean playing = false;
boolean catIsOnGround = true;
boolean showGoalLine = false;

int indexLevel = 1; // Store the ">" mark position
int level = 0;      // Store the level that the user chose

// Score
int getFruit = 0;
int getFish = 0;
int collideHurdle = 0;
int totalScore = 0;

// Function (1) - setup
void setup()
{

  Serial.begin(9600); // Add serial monitor

  // Input
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // Output
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  // LCD
  lcd.begin(16, 2); // Initialize the LCD. (16 characters and 2 characters)

  // Create objects design
  lcd.createChar(catChar, cat);
  lcd.createChar(bananaChar, banana);
  lcd.createChar(grapesChar, grapes);
  lcd.createChar(appleChar, apple);
  lcd.createChar(fishChar, fish);
  lcd.createChar(hurdleChar, hurdle);
  lcd.createChar(upperTreeChar, upperTree);
  lcd.createChar(lowerTreeChar, lowerTree);

  lcd.clear(); // Clear the screen
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Fruit Catch");
  lcd.setCursor(12, 1);
  lcd.write(catChar);
  delay(2000);
}

// Function (2) - loop
void loop()
{

  lcd.clear(); // Clear the screen
  lcd.setCursor(0, 0);
  lcd.print("Select Level");
  lcd.setCursor(1, 1);
  lcd.print("1");
  lcd.setCursor(5, 1);
  lcd.print("2");
  lcd.setCursor(9, 1);
  lcd.print("3");
  delay(300);

  while (playing == false)
  {
    selectLevel();
  }
}

// Function (3) - Select level
void selectLevel()
{

  // Level 1
  while (indexLevel == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print(">");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(" ");
    delay(300);

    // Move ">" to next to "2"
    if (digitalRead(button2) == HIGH && indexLevel == 1)
    {
      indexLevel = 2;
      lcd.setCursor(0, 1);
      lcd.print(" ");
    }

    // Start game if the user hits button 1
    if (digitalRead(button1) == HIGH)
    {
      level = 1;
      delay(300);
      break;
    }
  }

  // Level 2
  while (indexLevel == 2)
  {
    lcd.setCursor(4, 1);
    lcd.print(">");
    delay(500);
    lcd.setCursor(4, 1);
    lcd.print(" ");
    delay(300);

    // Move ">" to next to "3"
    if (digitalRead(button2) == HIGH && indexLevel == 2)
    {
      indexLevel = 3;
      lcd.setCursor(4, 1);
      lcd.print(" ");
    }

    // Start game if the user hits button 1
    if (digitalRead(button1) == HIGH)
    {
      level = 2;
      delay(300);
      break;
    }
  }

  // Level 3
  while (indexLevel == 3)
  {
    lcd.setCursor(8, 1);
    lcd.print(">");
    delay(500);
    lcd.setCursor(8, 1);
    lcd.print(" ");
    delay(300);

    // Move ">" to next to "1"
    if (digitalRead(button2) == HIGH && indexLevel == 3)
    {
      indexLevel = 1;
      lcd.setCursor(0, 1);
      lcd.print(" ");
    }

    // Start game if the user hits button 1
    if (digitalRead(button1) == HIGH)
    {
      level = 3;
      delay(300);
      break;
    }
  }

  Serial.print("Level: ");
  Serial.println(level);

  startGame(); // Jump to startGame function
  delay(300);
}

// Function (4) - Start game
void startGame()
{

  playing = true;

  // Repeat generating the items for 5 times
  for (int lap = 1; lap <= 6; lap++)
  {
    Serial.print("Lap: ");
    Serial.println(lap);
    playGame();
    if (lap == 5)
    {
      showGoalLine = true;
    }
  }
}

// Function (5) - Control game
void playGame()
{

  lcd.clear(); // Clear the screen

  // Generate each object between 4 and 8
  // Fruits and fish
  int iSecondPosition = random(4, 9);
  int iThirdPosition = random(4, 9);
  int iFourthPosition = random(4, 9);
  // Hurdle
  int hFirstPosition = random(4, 9);
  int hSecondPosition = random(4, 9);
  int hThirdPosition = random(4, 9);

  // Move distance
  int columnValueToStopMoveItems = -(iSecondPosition + iThirdPosition + iFourthPosition);

  // Keep making fruits and fishes move until others moved
  for (int firstItemPosition = 16; firstItemPosition >= columnValueToStopMoveItems; firstItemPosition--)
  { // Keep generating items for 60 seconds

    defineCatPosition(); // Jump to defineCatPosition function

    // Define the second, third, and fourth items position
    // Fruits and fish
    int secondItemPosition = firstItemPosition + iSecondPosition;
    int thirdItemPosition = secondItemPosition + iThirdPosition;
    int fourthItemPosition = thirdItemPosition + iFourthPosition;
    // Hurdle
    int firstHurdlePosition = firstItemPosition + hFirstPosition;
    int secondHurdlePosition = secondItemPosition + hSecondPosition;
    int thirdHurdlePosition = thirdItemPosition + hThirdPosition;

    // Jump to each generateItem function
    // Level 1 - Fruits and fishes only
    if (level == 1)
    {
      generateItem1(firstItemPosition);
      generateItem2(secondItemPosition);
      generateItem3(thirdItemPosition);
      generateItem4(fourthItemPosition);
      if (showGoalLine == true)
      {
        goalLine(); // Jump to goalLine function and display the goal line when all items were passed
      }
    }
    // Fruits, fishes, and hurdles
    else if (level == 2 || level == 3)
    {
      generateItem1(firstItemPosition);
      generateHurdle(firstHurdlePosition);
      generateItem2(secondItemPosition);
      generateHurdle(secondHurdlePosition);
      generateItem3(thirdItemPosition);
      generateHurdle(thirdHurdlePosition);
      generateItem4(fourthItemPosition);
      if (showGoalLine == true)
      {
        goalLine(); // Jump to goalLine function and display the goal line when all items were passed
      }
    }

    // Calculate score
    // Level 1 - Fruit*10pt, Fish*30pt
    if (level == 1)
    {
      if (catIsOnGround == false)
      {
        if (firstItemPosition == 0 || secondItemPosition == 0 || thirdItemPosition == 0)
        {                // (column, row) = (0, 0)
          getFruit += 1; // Add 10 points if the cat caught a fruit
          // Turn on the yellow light
          analogWrite(Red, 255);
          analogWrite(Green, 255);
          delay(flashTime);
          // Turn off the yellow light
          analogWrite(Red, 0);
          analogWrite(Green, 0);
        }
        else if (fourthItemPosition == 0)
        {
          getFish += 1; // Add 30 points if the cat caught a fish
          // Turn on the light blue light
          analogWrite(Red, 135);
          analogWrite(Green, 206);
          analogWrite(Blue, 250);
          delay(flashTime);
          // Turn off the light blue light
          analogWrite(Red, 0);
          analogWrite(Green, 0);
          analogWrite(Blue, 0);
        }
      }
    }

    // Level 2 - Fruit*10pt, Fish*30pt, Hurdle*-30pt
    else if (level == 2)
    {
      if (catIsOnGround == false)
      {
        if (firstItemPosition == 0 || secondItemPosition == 0 || thirdItemPosition == 0)
        {                // (column, row) = (0, 0)
          getFruit += 1; // Add 10 points if the cat caught a fruit
          // Turn on the yellow light
          analogWrite(Red, 255);
          analogWrite(Green, 255);
          delay(flashTime);
          // Turn off the yellow light
          analogWrite(Red, 0);
          analogWrite(Green, 0);
        }
        else if (fourthItemPosition == 0)
        {
          getFish += 1; // Add 30 points if the cat caught a fish
          // Turn on the light blue light
          analogWrite(Red, 135);
          analogWrite(Green, 206);
          analogWrite(Blue, 250);
          delay(flashTime);
          // Turn off the light blue light
          analogWrite(Red, 0);
          analogWrite(Green, 0);
          analogWrite(Blue, 0);
        }
      }
      else if (catIsOnGround == true)
      {
        if (firstHurdlePosition == 0 || secondHurdlePosition == 0 || thirdHurdlePosition == 0)
        {                          // (column, row) = (0, 1)
          collideHurdle += 1;      // Subtract 30 points if the cat hits a hurdle
          digitalWrite(Red, HIGH); // Turn on the red light
          delay(flashTime);
          digitalWrite(Red, LOW); // Turn off the red light
        }
      }
    }

    // Level 3 - Fruit*20pt, Fish*60pt, Game over if the cat hits any hurdle
    else if (level == 3)
    {
      if (catIsOnGround == false)
      {
        if (firstItemPosition == 0 || secondItemPosition == 0 || thirdItemPosition == 0)
        {                // (column, row) = (0, 0)
          getFruit += 1; // Add 10 * 2 points if the cat caught a fruit
          // Turn on the yellow light
          analogWrite(Red, 255);
          analogWrite(Green, 255);
          delay(flashTime);
          // Turn off the yellow light
          analogWrite(Red, 0);
          analogWrite(Green, 0);
        }
        else if (fourthItemPosition == 0)
        {
          getFish += 1; // Add 30 * 2 points if the cat caught a fish
          // Turn on the light blue light
          analogWrite(Red, 135);
          analogWrite(Green, 206);
          analogWrite(Blue, 250);
          delay(flashTime);
          // Turn off the light blue light
          analogWrite(Red, 0);
          analogWrite(Green, 0);
          analogWrite(Blue, 0);
        }
      }
      else if (catIsOnGround == true)
      {
        if (firstHurdlePosition == 0 || secondHurdlePosition == 0 || thirdHurdlePosition == 0)
        {             // (column, row) = (0, 1)
          gameOver(); // Jump to gameOver function
        }
      }
    }

    // Moving speed
    // Level 1 and 2 - 0.3 seconds
    if (level == 1 || level == 2)
    {
      delay(300);
    }
    // Level 3 - 0.15 seconds
    else if (level == 3)
    {
      delay(150);
    }
  }
}

// Function (6) - Define cat position
void defineCatPosition()
{

  while (level >= 1)
  {
    if (digitalRead(button2) == HIGH)
    {
      putCatOnAir(); // Generate the cat on the 1st line if the user hit the button 2
    }
    else
    {
      putCatOnGround(); // Generate the cat on the 2nd line if the user didn't hit the play button
    }
    break;
  }
}

// Function (7) - Generate the cat on the 1st line
void putCatOnAir()
{

  lcd.setCursor(0, 0); // (column, row) = (0, 0)
  lcd.write(catChar);
  lcd.setCursor(0, 1);
  lcd.print(" ");

  catIsOnGround = false;
}

// Function (8) - Generate the cat on the 2nd line
void putCatOnGround()
{

  lcd.setCursor(0, 0);
  lcd.print(" ");
  lcd.setCursor(0, 1); // (column, row) = (0, 1)
  lcd.write(catChar);

  catIsOnGround = true;
}

// Function (9) - Generate banana
void generateItem1(int itemPosition)
{

  lcd.setCursor(itemPosition, 0);
  lcd.write(bananaChar);

  // Clean the previous position
  lcd.setCursor(itemPosition + 1, 0);
  lcd.print(" ");
}

// Function (10) - Generate grapes
void generateItem2(int itemPosition)
{

  lcd.setCursor(itemPosition, 0);
  lcd.write(grapesChar);

  // Clean the previous position
  lcd.setCursor(itemPosition + 1, 0);
  lcd.print(" ");
}

// Function (11) - Generate apple
void generateItem3(int itemPosition)
{

  lcd.setCursor(itemPosition, 0);
  lcd.write(appleChar);

  // Clean the previous position
  lcd.setCursor(itemPosition + 1, 0);
  lcd.print(" ");
}

// Function (12) - Generate fish
void generateItem4(int itemPosition)
{

  lcd.setCursor(itemPosition, 0);
  lcd.write(fishChar);

  // Clean the previous position
  lcd.setCursor(itemPosition + 1, 0);
  lcd.print(" ");
}

// Function (13) - Generate hurdles
void generateHurdle(int itemPosition)
{

  lcd.setCursor(itemPosition, 1);
  lcd.write(hurdleChar);

  // Clean the previous position
  lcd.setCursor(itemPosition + 1, 1);
  lcd.print(" ");
}

// Function (14) - Game Over
void gameOver()
{

  playing = false;

  lcd.clear(); // Clear the screen

  lcd.setCursor(0, 0);
  lcd.print("Game Over...");

  delay(3000);

  showTotalScore(); // Jump to showTotalScore function
}

// Function (15) - Display the goal line
void goalLine()
{

  playing = false;

  for (int goal = 15; goal >= 1; goal--)
  {

    lcd.setCursor(goal, 0);
    lcd.write(upperTreeChar);
    lcd.setCursor(goal, 1);
    lcd.write(lowerTreeChar);

    lcd.setCursor(goal + 1, 0);
    lcd.print(" ");
    lcd.setCursor(goal + 1, 1);
    lcd.print(" ");

    delay(500);
  }

  showTotalScore(); // Jump to showTotalScore function when the cat hits the goal line
}

// Function (16) - Display the total score
void showTotalScore()
{

  // Calculate the total score
  if (level == 1)
  {
    totalScore = (getFruit * 10) + (getFish * 30);
  }
  else if (level == 2)
  {
    totalScore = (getFruit * 10) + (getFish * 30) + (collideHurdle * -30);
  }
  else if (level == 3)
  {
    totalScore = (getFruit * 20) + (getFish * 60);
  }

  lcd.clear(); // Clear the screen

  // Fruit
  lcd.setCursor(0, 0);
  lcd.write(bananaChar);
  lcd.setCursor(1, 0);
  lcd.print("=");
  lcd.setCursor(2, 0);
  lcd.print(getFruit);

  // Fish
  lcd.setCursor(5, 0);
  lcd.write(fishChar);
  lcd.setCursor(6, 0);
  lcd.print("=");
  lcd.setCursor(7, 0);
  lcd.print(getFish);

  if (level == 2)
  {
    // Hurdle
    lcd.setCursor(10, 0);
    lcd.write(hurdleChar);
    lcd.setCursor(11, 0);
    lcd.print("=");
    lcd.setCursor(12, 0);
    lcd.print(collideHurdle);
  }

  // Total
  lcd.setCursor(0, 1);
  lcd.print("Your Score: ");
  lcd.setCursor(12, 1);
  lcd.print(totalScore);

  // Initialize each variables
  indexLevel = 1;
  level = 0;
  showGoalLine = false;
  getFruit = 0;
  getFish = 0;
  collideHurdle = 0;
  totalScore = 0;

  delay(10000);

  lcd.clear(); // Clear the screen
  loop();      // Back to the select level screen
}

/*****************************************************************************************************************************************************************************************************
* Reference

1. Ebeed, A. (2018, Aug 21). T-Rex Dinosaur Game on Arduino With Tinkercad.PROJECT HUB. https://create.arduino.cc/projecthub/Ahmedebeed555/t-rex-dinosaur-game-on-arduino-with-tinkercad-01298b
*****************************************************************************************************************************************************************************************************/
