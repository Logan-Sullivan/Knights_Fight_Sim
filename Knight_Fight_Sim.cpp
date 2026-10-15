//Knights_Fight_Sim 
//Author:Logan Sullivan
//Date: Fall 2026
#include<Windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>
#include<iostream>
#include<iomanip>
using namespace std;

class Player {
private:
	//need to add Opponent Variable
	//Name of player will be displayed above Health Bar and within Bracket
	string Name;
	//Health of player only modifiable by adjudication
	float health = 1.0;
	//full match wins only modifiable by adjudication
	int matchWins = 0;
	//rounds won in current round resets after match
	int roundWins = 0;
	//hold moves performed in the previous 3 turns with lastMoves[0] being the most recent move this is updated by adjudication
	int lastMoves[3] = { 0,0,0 };
	//this holds your strategy function given by the constructor
	int (*StrategyFunc)(void);
	
	
public:
	
	
	
	//this executes your strategy function
	int doStrat(void) {
		return StrategyFunc();
	}
	//Constructor takes your name and a function to use for strategy that returns an int;
	Player(string Name, int (*func)(void)) : Name(Name), StrategyFunc(func) {};
	//returns Player's current health
	float getHealth(void) {
		return health;
	}
	//damages your player only to be used by adjudication
	void takedamage(float dmg) {
		health -= dmg;
		return;
	}
	string getName(void) {
		return Name;
	}
	

};
//This function will be used to implement your strategy and will return the index of the move to be performed on your next turn
//Uniquly name your Strategy Function please so that implementation will be easier!! (:
int StrategyFunc();
int Test() {
	cout << "DoFunc" << endl;
	return 25;
}
int main(int argc, char** argv) {
	
	Player p1("Meow", Test);
	cout << p1.getName() << endl;
	cout << p1.doStrat() << endl;
	return 0;
}