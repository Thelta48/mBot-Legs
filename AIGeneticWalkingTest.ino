#include <Arduino.h>
#include <EEPROM.h>

// Definitions for motor control pins
const int motorAdir = 7; 
const int motorApwm = 6; 
const int motorBdir = 4; 
const int motorBpwm = 5; 

// Genetic Algorithm Parameters
const int populationSize = 10;
const int geneLength = 42; // 14 codons * 3 ints each
const int maxGenerations = 10000;

//A class describing an individual. Each individual has a "gene" array and a float describing the fitness, or how good the gene sequence is
struct Individual {
    float genes[geneLength]; 
    float fitness;
};

//An array of individuals describing the population
Individual population[populationSize];


float maxim = 255;//the maximum value of a float in the sequence
//Initializing the population
void initializePopulation() {
    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < geneLength; j++) {
            population[i].genes[j] = random(0, maxim); // Random value for each part of each codon
        }
        population[i].fitness = 0;
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(motorAdir, OUTPUT);
  pinMode(motorApwm, OUTPUT);
  pinMode(motorBdir, OUTPUT);
  pinMode(motorBpwm, OUTPUT);
  analogWrite(motorApwm, 0);
  analogWrite(motorBpwm, 0);
  initializePopulation();
  //The delay is so that when uploading the code to get EEPROM values to test, you have time to upload before it overwrites the EEPROM value
  delay(20000);
}

//Getting the distance from the ultrasonic sensor
float getdR(){
  unsigned long now = millis();
  int sigR = 10;
  pinMode(sigR, OUTPUT);
  digitalWrite(sigR, LOW);
  delayMicroseconds(2);
  digitalWrite(sigR, HIGH);
  delayMicroseconds(10);
  digitalWrite(sigR, LOW);
  //pinMode(sigR, LOW);
  pinMode(sigR, INPUT);
  float dR = (pulseIn(sigR, HIGH)*0.343)/2;//from 0.0343 to 0.343
  return dR;
}

boolean compass = false;//A boolean value giving direction to the walking. This is to distinguish between the robot walking in one direction consistently
//and the robot just jiggling around. When uploading to both mBots, one mBot must have this value as false and the other must have it as true.

//The below code is to control the motors based on the gene sequence.
void controlMotors(int indiv) {
    float gene[geneLength];
    for(int CRISPR = 0; CRISPR < geneLength; CRISPR++){
      gene[CRISPR] = population[indiv].genes[CRISPR]; 
    }
    float old_dist = getdR();//To measure the change in distance
    for(int i = 0; i < geneLength; i++){//for each float in the gene
      int rem = i%6;//Each gene is made up 7 groups of 6. The first half of the 6 describes one motor, the second half describes the other
      //In each half, the first number describes if the motor moves or not, the second describes which direction the motor turns, the third describes
      //how long it turns
      switch(rem){
        case 0://if motor A turns at all
          if(gene[i] > maxim/2.0){
            analogWrite(motorApwm, 255);
          }else{
            analogWrite(motorApwm, 0);
          }
          break;
        case 1://which direction motor A turns in
          if(gene[i] > maxim/2.0){
            digitalWrite(motorAdir, HIGH);
          }else{
            digitalWrite(motorAdir, LOW);
          }
          break;
        case 2://how long motor A turns(if it does turn)
          delay(gene[i]);
          analogWrite(motorApwm, 0);
          delay(maxim - gene[i]);
          break;
        case 3://if motor B turns at all
          if(gene[i] > maxim/2.0){
            analogWrite(motorBpwm, 255);
          }else{
            analogWrite(motorBpwm, 0);
          }
          break;
        case 4://which direction motor B turns in
          if(gene[i] > maxim/2.0){
            digitalWrite(motorBdir, HIGH);
          }else{
            digitalWrite(motorBdir, LOW);
          }
          break;
        case 5://how long motor B turns(if it does turn)
          delay(gene[i]);
          analogWrite(motorBpwm, 0);
          delay(maxim - gene[i]);
          break;
      }
    }
    float new_dist = getdR();
    if((new_dist - old_dist > 0) == compass){//This is where the boolean variable "compass" comes in. It determines if moving further from a wall is desirable 
    //or not
       population[indiv].fitness = abs(new_dist - old_dist);
    }else{
       population[indiv].fitness = -3;//You can experiment with difference fitness functions. I decided to try just setting it to a hard -3 if undesirable,
       //but this is probably a suboptimal choice
    }
}


//Genetic algorithms by nature involve crossover, where different genes are combined in different parts to try to get "the best of both worlds"
void crossover(float geneA[], float geneB[]){//geneA and geneB are the genes we are combining
  for(int child = 0; child < populationSize; child++){//We want the population to be the same size, for simplicity
    float child_gene[geneLength];
    //We split each gene into three parts and mix and mash these three parts together randomly
    int firstPart = random(2);
    int secondPart = random(2);
    int thirdPart = random(2); 
    int startPoint = 0;
    int endPoint = int(geneLength/3);
    switch(firstPart){
      case 0:
        for(int j = startPoint; j < endPoint; j++){
          child_gene[j] = geneA[j];
        }
        break;
      case 1:
        for(int j = startPoint; j < endPoint; j++){
          child_gene[j] = geneB[j];
        }
        break;
    }
    startPoint = endPoint + 1;
    endPoint = startPoint + int(geneLength/3);
    switch(secondPart){
      case 0:
        for(int j = startPoint; j < endPoint; j++){
          child_gene[j] = geneA[j];
        }
        break;
      case 1:
        for(int j = startPoint; j < endPoint; j++){
          child_gene[j] = geneB[j];
        }
        break;
   }
   startPoint = endPoint + 1;
   endPoint = geneLength;
   switch(thirdPart){
      case 0:
        for(int j = startPoint; j < endPoint; j++){
          child_gene[j] = geneA[j];
        }
        break;
      case 1:
        for(int j = startPoint; j < endPoint; j++){
          child_gene[j] = geneB[j];
        }
        break;
    }
    //replacing one individual's genes with the new gene
    for(int splicer = 0; splicer < geneLength; splicer++){
      population[child].genes[splicer] = child_gene[splicer];
    }
 }
}

//In many implementations of the genetic algorithm, there is something called mutations. This introduces randomness in the process.
float mutation_rate = 0.1;
void mutation(){
  float rand_float = random(100)/100.0;
  if(rand_float < mutation_rate){
    int random_individual = random(populationSize);
    int random_GCAT = random(geneLength);
    population[random_individual].genes[random_GCAT] = random(0, maxim);
  }
}

//This finds the top two fitnesses. The first element of topTwo is the index of the top one, the second element is the fitness of the top, and the 
//third and fourth one is the same for the second-most top
float topTwo[4] = {-1, -1.0, -1, -1.0};
void findTopTwoFitnesses() {
    for (int i = 0; i < populationSize; i++) {
        float the_fit = population[i].fitness;
        if (the_fit > topTwo[1]) {
            topTwo[2] = topTwo[0];  // Move current top to second top
            topTwo[3] = topTwo[1];  // Move current top to second top
            topTwo[1] = the_fit;
            topTwo[0] = (float) i;
        } else if (the_fit > topTwo[3]) {
            topTwo[2] = (float) i;
            topTwo[3] = the_fit;
        }
    }
}


int gen = 0;//generation number
void loop() {
  delay(1000);
  for(int ind = 0; ind < populationSize; ind++){//trying out all the individuals' genes
    controlMotors(ind);
  }
  findTopTwoFitnesses();
  float top_fit = topTwo[1];
  if(gen <= maxGenerations ){
    //analogWrite(motorApwm, 255);
    //analogWrite(motorBpwm, 255);
    float geneA[geneLength];
    float geneB[geneLength];
    for(int splice = 0; splice < geneLength; splice++){
      geneA[splice] = population[(int)topTwo[0]].genes[splice];
      geneB[splice] = population[(int)topTwo[2]].genes[splice];
    }
    crossover(geneA, geneB);//We want to breed the top two genes together.
    mutation();
  }
  float best_gene[geneLength];
  for(int splice = 0; splice < geneLength; splice++){
    best_gene[splice] = population[(int)topTwo[0]].genes[splice];
  }
  for(int sanger = 0; sanger < geneLength; sanger++){
    EEPROM.write(sanger, int(best_gene[sanger]));//writing to EEPROM. EEPROM is a type of memory which does not erase even if the mBot turns off
  }
  gen++;
}
