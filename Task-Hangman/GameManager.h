#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <queue>
#include <utility>

using namespace std;

class GameManager
{
private:
    const int MAX_TRIES;
    string word;
    vector<string> words; // Use vector for dynamic size
    string player_name;
    vector<char> playerLetters; // Store player's letters
    time_t startTime, endTime;
    int attemptsUsed;

    vector<string> loadWords(); // Load words from file
    int letterFill(char guess, string& guessword);
    void updateHighScores(int current_score);
    void displayStatistics();

public:
    GameManager();
    void getPlayerName();
    void showRules();
    void startGame();
    void showTitle();
    void showHighScores();
};

#endif
