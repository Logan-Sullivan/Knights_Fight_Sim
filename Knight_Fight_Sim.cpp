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
	string Name = "";
	//Health of player only modifiable by adjudication
	float health = 1.0;
	//full match wins only modifiable by adjudication
	int matchWins = 0;
	//rounds won in current round resets after match
	int roundWins = 0;
	//hold moves performed in the previous 3 turns with lastMoves[0] being the most recent move this is updated by adjudication
	int lastMoves[3] = { 0,0,0 };
	//This function will be used to implement your strategy and will return the index of the move to be performed on your next turn
	int chooseNextMove() {
		return -1;
	}
public:
	//returns Player's current health
	float getHealth() {
		return health;
	}
	//damages your player only to be used by adjudication
	void takedamage(float dmg) {
		health -= dmg;
		return;
	}
};

int main(int argc, char** argv) {
	cout << "Hello World!" << endl;
}