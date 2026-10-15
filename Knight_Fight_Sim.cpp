
#include<Windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>


#include<iomanip>
//students should only need these two imports as rest are for rendering
#include<iostream>
#include<random>
using namespace std;
/*
Knights_Fight_Sim
Author:Logan Sullivan
Date: Fall 2026
*/
/*Global Variables These can be used anywhere in the program*/

//
/*
if Player1 does move i and Player 2 does move j 
[i][j][0] = p1Damage
[i][j][1] = p2Damage
i/j

0 : Lunge
1 : Thrust
2 : Overhead
3 : Slash Right-High
4 : Slash Left-High
5 : Slash Right-Low
6 : Slash Left-Low
7 : Block Right-High
8 : Block Left-High
9 : Block Right-Low
10 : Block Left-Low
*/
const float pkProb[11][11][2] =
{						/*Lunge   Thrust  OverHead    SRH      SLH        SRL      SLL      BRH        BLH   BRL     BLL*/
	/*Lunge*/           {{.3,.3} ,{.3,.2} ,{.4,.1} ,{.1,.2} ,{.1,.2}  ,{.1,.15},{.1,.15},{.05,.0},{.05,.0},{.3,.0},{.3,.0}},
	/*Thrust*/          {{.2,.3} ,{.3,.3} ,{.3,.1} ,{.3,.05},{.3,.05} ,{.2,.15},{.2,.15},{.3,.0},{.3,.0},{.05,.0},{.05,.0}},
	/*Overhead*/        {{.1,.4} ,{.1,.3} ,{.1,.1} ,{.3,.2} ,{.3,.2}  ,{.3,.1},{.3,.1},{.05,.0},{.05,.0},{.4,.0},{.4,.0} },
	/*Slash Right-High*/{{.2,.1} ,{.05,.3},{.2,.3} ,{.5,.5} ,{.05,.05},{.4,.3},{.4,.3},{.05,.0},{.1,.0},{.5,.0},{.5,.0} },
	/*Slash Left-High*/ {{.2,.1} ,{.05,.3},{.2,.3} ,{.05,.05},{.5,.5} ,{.4,.3},{.4,.3},{.1,.0},{.05,.0},{.5,.0},{.5,.0} },
	/*Slash Right-Low*/ {{.15,.1},{.15,.2},{.1,.3} ,{.3,.4} ,{.3,.4}  ,{.2,.2},{.15,.15},{.2,.0},{.2,.0},{.05,.0},{.1,.0} },
	/*Slash Left-Low*/  {{.15,.1},{.15,.2},{.1,.3} ,{.3,.4} ,{.3,.4}  ,{.15,.15},{.3,.3},{.2,.0},{.2,.0},{.1,.0},{.05,.0} },
	/*Block Right-High*/{{.0,.05},{.0,.3} ,{.0,.05},{.0,.05},{.0,.1}  ,{.0,.2},{.0,.2},{.0,.0},{.0,.0},{.0,.0},{.0,.0} },
	/*Block Left-High*/ {{.0,.05},{.0,.3} ,{.0,.05},{.0,.1} ,{.0,.05} ,{.0,.2},{.0,.2},{.0,.0},{.0,.0},{.0,.0},{.0,.0} },
	/*Block Right-Low*/ {{.0,.3} ,{.0,.05},{.0,.4} ,{.0,.5} ,{.0,.5}  ,{.0,.05},{.0,.1},{.0,.0},{.0,.0},{.0,.0},{.0,.0} },
	/*Block Left-Low*/  {{.0,.3} ,{.0,.05},{.0,.4} ,{.0,.5} ,{.0,.5}  ,{.0,.1},{.0,.05},{.0,.0},{.0,.0},{.0,.0},{.0,.0} }
};


//changed that if you move or stop you cannot move or stop right after to prevent stalling
/*
//[lastmove][availablemoves] 
if [i][j] = 0 move j is not available
if [i][j] = 1 move j is available
0 : Lunge
1 : Thrust
2 : Overhead
3 : Slash Right - High
4 : Slash Left - High
5 : Slash Right - Low
6 : Slash Left - Low
7 : Block Right - High
8 : Block Left - High
9 : Block Right - Low
10: Block Left - Low
11: Move
12: Stop
*/
const int states[13][13] = {
{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1 }, //Lunge
{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, //Thrust
{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1 }, //Overhead
{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1 }, //Slash Right-High
{ 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }, //Slash Left-High
{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1 }, //Slash Right-Low
{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1 }, //Slash Left-Low
{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1 }, //Block Right-High
{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }, //Block Left-High
{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1 }, //Block Right-Low
{ 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1 }, //Block Left-Low
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 }, //Move
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 }  //Stop
};

//class used to prevent misuse of opponents functions
class Adjudicator {
private:

};


class Player {
//All variables within private are only to be used within the Adjudicator class and therefore are inaccessible unless using specified getter methods
private:
	boolean alive = true;
	//Name of player will be displayed above Health Bar and within Bracket
	string Name;
	//Health of player
	float health = 1.0;
	//full match wins
	int matchWins = 0;
	//rounds won in current round resets after match
	int roundWins = 0;
	//hold moves performed in the previous 3 turns with lastMoves[0] being the most recent move this is updated by adjudication
	int lastMoves[3] = { 12,12,12 };
	//this holds your strategy function given by the constructor
	int (*StrategyFunc)(void);
	//this will hold your opponents stats and will be given and updated by the Adjudicator everyRound
	string OppName = "na";float Opphealth = -1.0;int OppmatchWins = -1;int OpproundWins = -1;int OpplastMoves[3] = { -1,-1,-1 };
	//allows Adjudicator class to change Player objects
	friend class Adjudicator;

	//this executes your strategy function
	int doStrat(void) {
		return StrategyFunc();
	}

	void resetStatsAfterFight(void) {
		for (int resetIterator = 0; resetIterator < 3; resetIterator++) {
			lastMoves[resetIterator] = 12;
		}
		health = 1.0;
		alive = true;
		return;
	}
	//damages PlayerObj by dmg amount rounded to 4 decimals in case of floating point left over bits messing up things
	void takedamage(float dmg) {
		health = round((health - dmg) * 10000.0f) / 10000.0f;
		return;
	}
public:
	
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

//Example of how to implement a strategy so that you have a guidline of how to use these functions / parameters
int ExampleStrategy() {

}
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