
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
/*
Dict

Round: a collection of turns end of a round is determined by either time limit being reached or death of a fighter
Match: a match is a collection of rounds in which 3 wins by 1 fighter results in the end of a match meaning a max of 5 rounds / score of 2-3

Classes
	Player //holds each fighters strategy name as well as statistics such as health and wins
*/
/*Global Variables These can be used anywhere in the program*/

//
/*
if Player1 does move i and Player 2 does move j 
[i][j][0] = p1Damage
[i][j][1] = p2Damage
p1Health -= [i][j][0]
p2Health -= [i][j][1]

note that if a player gets blocked successfully they take some amount of retaliation damage  
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
example:
player 1 performs a lunge player 2 performs a Block Left-Low
lunge is at index 0 Block Left-Low is at index 10

[0][10] = {.3,.0}
player1 will take .3 damage
player2 will take 0 damage
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
example:
if your last move was a Overhead which is index 2
to perform your next move you first must check states[2]
checking states[2] we see
{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1 }
this means that moves at indexes 2,6,9,10,11,12 are valid as they are a 1
whereas indexes 1,3,4,5,7,8 are 0 and therefore invalid moves
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

/*
Adjudication concept

adjudicator takes 2 fighters from our bracket
if !roundBegun
	startRound(round);


round structure
timer will be handled by animation engine calling our adjudication function every x amount of time until our time limit is reached
on each adjudication call (decided by animation engine timer):
	if timer >= time limit 
		compare each fighters health
		fighter with more health wins round
		if both fighters have same hp tieCounter++
		endRound()

	else:

	fighters decide their next move
	if either fighters move is illegal (isMoveLegal())
		set move for this round to Stop
		damage fighter which performed illegal move by 25% as a penalty

	if fighters move is a damaging move:
		adjust fighters damage taken and damage done stats
		adjust fighters movesDone and moves hit by stats
		adjust fighters health
	if both fighters health <= 0 tieCounter++
	
	if only one fighters health is <= 0
		set fighters health to 0 (prevents rounding / rendering problems)
		set fighters "alive" boolean to false
		increment winners roundsWon
		endRound()

endRound():
	check each fighters roundsWon
	if fighter.roundsWon >= 3
		fighter.wonMatch = true
		endMatch()
	if tieCounter >= 3
		generate random number "r" which ranges from 0 - 0.999 repeating
		if r <= 0.50 fighter1.wonMatch = true
		else fighter2.wonMatch = true
		endMatch()
	reset both fighters round stats health last moves etc
	reset time limit
	round++
*/

//class used to prevent misuse of opponents functions
class Adjudicator {
private:
	//keeps the round count
	int round = 1; 
	//keeps score for match
	int score[2] = { 0,0 };
	/*keeps time of match in seconds if time reaches 0 round ends with winner decided by most hp left
	if fighters have same hp then round is a tie*/
	int timeLimit = 45;
	//counts if fighters tied if fighters tie 3 times in a match win will be decided by "coinflip"
	int tieCounter = 0;
	//holds the fighters that will fight
	Player Fighter1;Player Fighter2;
	//inits basic values that will not change throughout the match beginFight handles rest
	void SetupMatch() {
		
	}
	//starts fight initiates values etc
	void beginFight() {

	}

	//ends round called when either time limit is reached or a fighter is killed
	void endRound(){
		//change score
		//adjust fighters statistics
		//check if end of match if so run endMatch()
		//reset fighters
		//change round
		
	}
	
	//gives opponent stats to each fighter after each turn
	void setOpponentStats() {
		//string OppName = "na";float Opphealth = -1.0;int OppmatchWins = -1;int OpproundWins = -1;int OpplastMoves[3] = { -1,-1,-1 };

	}
	//checks players move choice against states array to verify if move is allowed based on previous move
	boolean isMoveLegal() {

	}
	public:
	
};


class Player {
//All variables within private are only to be used within the Adjudicator class and therefore are inaccessible unless using specified getter methods
private:
	boolean wonMatch = false;
	boolean alive = true;
	//Name of player will be displayed above Health Bar and within Bracket
	string Name;
	//Health of player
	float health = 1.0f;

	//rounds won in current round resets after match
	int roundWins = 0;
	/*hold moves performed in the previous 3 turns with lastMoves[0] being the most recent move this is updated by adjudication
	this is reset on round end not match end
	*/
	int lastMoves[3] = { 12,12,12 };
	//this holds your strategy function given by the constructor
	int (*StrategyFunc)(void);
	//this will hold your opponents stats and will be given and updated by the Adjudicator everyRound
	string OppName = "na";float Opphealth = -1.0;int OppmatchWins = -1;int OpproundWins = -1;int OpplastMoves[3] = { -1,-1,-1 };
	
	//allows Adjudicator class to change Player objects
	friend class Adjudicator;
	
	/*****    General Statistics    *****/
	//records amount of times each move is performed
	int totalMovesPerformed[13]; 
	//holds total rounds won
	int totalRoundsWon = 0;
	//full match wins
	int totalMatchWins = 0;
	//holds damage done throughout whole tournament
	float totalDmgDone = 0.0;
	//holds damage taken throughout whole tournament
	float totalDmgTaken = 0.0;
	//holds moves which hit you throughout whole tournament
	int totalMovesHitBy[13];
	//holds moves youve been hit by this match
	int matchMovesHitBy[13];
	
	/*****    Functions    *****/
	//resets health alivebool and lastMoves array
	void resetRoundStats() {
		health = 1.0f;
		alive = true;
		for (int i = 0;i < 3;i++) {
			lastMoves[i] = 12;
		}
	}

	//updates variables which change after each turn
	void updateTurnStats(int myMove,float dmgDone,float dmgTaken,int oppMove) {
		//lastMoves
		//totalMovesPerformed
		//totalDmgDone
		//totalDmgTaken
		//matchMovesHitBy
		//totalMovesHitBy
	}

	void updateRoundStats() {
		if (alive) {
			roundWins++;
			totalRoundsWon++;
		}
		//roundWins
		//totalRoundsWon
	}

	//this executes your strategy function
	int doStrat(void) {
		return StrategyFunc();
	}
	//resets players round stats
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
	void getOppName(string nOppName) {
		OppName = nOppName;
	}
	void getOpphealth(float nOpphealth) {
		Opphealth = nOpphealth;
	}
	void getOppmatchWins(int nOppmatchWins) {
		OppmatchWins = nOppmatchWins;
	}
	void getOpproundWins(int nOpproundWins) {
		OpproundWins = nOpproundWins;
	}
	void getOpplastMove(int nOpplastMove, int index) {
		OpplastMoves[index] = nOpplastMove;
	}
	//sets p1's opponentvariables to p2's variables
	void getAllOppInfo(Player p1, Player p2) {
		p1.getOpphealth(p2.getHealth());
		p1.getOppName(p2.getName());
		p1.getOppmatchWins(p2.getMatchWins());
		p1.getOpproundWins(p2.getRoundWins());
		for (int i = 0;i < 3;i++) {
			p1.getOpplastMove(p2.getLastMoveI(i), i);
		}
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
	// returns last move at index i (0-2)
	int getLastMoveI(int i) {
		//checks that i is in range
		if (i < 3 && i >= 0) {
			return lastMoves[i];
		}
	}
	//returns current match's amount of rounds won
	int getRoundWins(void) {
		return roundWins;
	}
	//returns match wins
	int getMatchWins(void) {
		return totalMatchWins;
	}

	/*     Opponent Getters     */
	float Opp_getHealth(void) {
		return Opphealth;
	}
	string Opp_getName(void) {
		return OppName;
	}
	// returns last move at index i (0-2)
	int Opp_getLastMoveI(int i) {
		//checks that i is in range
		if (i < 3 && i >= 0) {
			int lastMove = OpplastMoves[i];
			return lastMove;
		}
	}
	int Opp_getRoundWins(void) {
		return OpproundWins;
	}
	int Opp_getMatchWins(void) {
		return OppmatchWins;
	}

};

//This function will be used to implement your strategy and will return the index of the move to be performed on your next turn
int Firstname_LastName_StratName();

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
	return 0;
}