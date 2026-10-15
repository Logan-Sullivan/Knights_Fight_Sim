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

//class used to prevent misuse of opponents functions
class Adjudicator {

};


class Player {
//All variables within private are only to be used within the Adjudicator class and therefore are inaccessible unless using specified getter methods
private:
	
	//Name of player will be displayed above Health Bar and within Bracket
	string Name;
	//Health of player
	float health = 1.0;
	//full match wins
	int matchWins = 0;
	//rounds won in current round resets after match
	int roundWins = 0;
	//hold moves performed in the previous 3 turns with lastMoves[0] being the most recent move this is updated by adjudication
	int lastMoves[3] = { 0,0,0 };
	//this holds your strategy function given by the constructor
	int (*StrategyFunc)(void);
	//this will hold your opponents stats and will be given and updated by the Adjudicator everyRound
	string OppName = "na";float Opphealth = -1.0;int OppmatchWins = -1;int OpproundWins = -1;int OpplastMoves[3] = { -1,-1,-1 };
	//allows Adjudicator class to change Player objects
	friend class Adjudicator;

	//damages PlayerObj by dmg amount rounded to 4 decimals in case of floating point left over bits messing up things
	void takedamage(float dmg) {
		health = round((health - dmg) * 10000.0f) / 10000.0f;
		return;
	}
public:
	//this executes your strategy function
	int doStrat(void) {
		return StrategyFunc();
	}
	//Constructor takes your name and a function to use for strategy that returns an int;
	Player(string Name, int (*func)(void)) : Name(Name), StrategyFunc(func) {};

	/*     Self Getters     */
	//returns Player's current health
	float getHealth(void) {
		return health;
	}
	//returns Player's name
	string getName(void) {
		return Name;
	}
	// returns last move at index i
	int getLastMoveI(int i) {
		//checks that i is in range
		if (i < 3 && i >= 0) {
			return lastMoves[i];
		}
	}
	//returns current matche's amount of rounds won
	int getRoundWins(void) {
		return roundWins;
	}
	//returns match wins
	int getMatchWins(void) {
		return matchWins;
	}

	/*     Opponent Getters     */

	float Opp_getHealth(void) {
		return Opphealth;
	}
	string Opp_getName(void) {
		return OppName;
	}
	int Opp_getLastMoveI(int i) {
		//checks that i is in range
		if (i < 3 && i >= 0) {
			return OpplastMoves[i];
		}
	}
	int Opp_getRoundWins(void) {
		return OpproundWins;
	}
	int Opp_getMatchWins(void) {
		return OppmatchWins;
	}

};
/*This function will be used to implement your strategy and will return the index of the move to be performed on your next turn
Uniquly name your Strategy Function please so that implementation will be easier!! (:*/
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