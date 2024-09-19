#include "GameManager.h"

GameManager::GameManager() : MAX_TRIES(5), attemptsUsed(0)
{
    words = loadWords();
    player_name = "default";

    // Ensure the high-score file exists or create it
    ifstream scoreFile("data/scores.txt");
    if (!scoreFile)
    {
        if (system("mkdir data") != 0)
        {
            cerr << "Error creating directory.\n";
            exit(1);
        }
        ofstream fout("data/scores.txt");
        fout.close();
    }
    scoreFile.close();

    startTime = time(nullptr);
}

vector<string> GameManager::loadWords()
{
    vector<string> words;
    ifstream fin("encrypted_word_list.txt");
    if (!fin)
    {
        cerr << "Error opening encrypted word list file.\n";
        exit(1);
    }

    string word;
    while (getline(fin, word))
    {
        words.push_back(word);
    }
    fin.close();
    return words;
}

void GameManager::showTitle()
{
    cout << "\t *    *     **     **    *  *****   **   **     **     **    *" << endl;
    cout << "\t *    *    *  *    * *   * *        * * * *    *  *    * *   *" << endl;
    cout << "\t ******   ******   *  *  * *  ***** *  *  *   ******   *  *  *" << endl;
    cout << "\t *    *  *      *  *   * * *      * *     *  *      *  *   * *" << endl;
    cout << "\t *    * *        * *    **  ******  *     * *        * *    **" << endl;
}

void GameManager::showRules()
{
    cout << endl;
    cout << "\t\t\t HOW TO PLAY" << endl;
    cout << "\t\t\t-------------" << endl;
    cout << "Welcome to hangman." << endl;
    cout << "You have to guess a country Name." << endl;
    cout << "Each letter is represented by a star." << endl;
    cout << "You have to type only one letter in one try." << endl;
    cout << "You have " << MAX_TRIES << " tries to try and guess the word." << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Press any key to continue." << endl;
    cin.ignore();
    cin.get();
}

void GameManager::startGame()
{
    int num_of_wrong_guesses = 0, cur_score = 100;  // Initialize the current score to 100.
    // Choose and copy a word from array of words randomly
    srand(static_cast<unsigned>(time(nullptr)));
    int n = rand() % words.size();
    word = words[n];

    string unknown(word.length(), '*');
    playerLetters.clear();
    startTime = time(nullptr);

    // Loop until the guesses are used up
    system("cls");
    while (num_of_wrong_guesses < MAX_TRIES)
    {
        cout << endl;
        cout << unknown << endl;
        cout << "Guess a letter: ";
        char letter;
        cin >> letter;
        playerLetters.push_back(letter);
        cout << endl;

        // Fill secret word with letter if the guess is correct,
        // otherwise increment the number of wrong guesses.
        if (letterFill(letter, unknown) == 0)
        {
            cout << "Whoops! That letter isn't in there!" << endl;
            num_of_wrong_guesses++;
            cur_score -= 10;    // Deduct 10 points for a wrong guess
        }
        else
        {
            cout << "You found a letter! Isn't that exciting!" << endl;
        }

        // Tell user how many guesses are left
        cout << "You have " << MAX_TRIES - num_of_wrong_guesses << " guesses left." << endl;

        // Check if user guessed the word
        if (word == unknown)
        {
            cout << word << endl;
            cout << "Woohoo! You got it!" << endl;
            break;
        }
    }

    endTime = time(nullptr);
    if (num_of_wrong_guesses == MAX_TRIES)
    {
        cout << "\nSorry, you lose...you've been hanged." << endl;
        cout << "The word was : " << word << endl;
        cur_score -= 20;    // Deduct 20 more points if the player loses
    }
    if (cur_score < 0)
        cur_score = 0;

    cout << "You scored: " << cur_score << endl;
    cout << endl;
    updateHighScores(cur_score);    // Update the scores in the file
    displayStatistics();            // Display game statistics
}

int GameManager::letterFill(char guess, string& guessword)
{
    int i;
    int matches = 0;
    for (i = 0; i < word.length(); i++)
    {
        // Did we already match this letter in a previous guess?
        if (guess == guessword[i])
            return 0;
        // Is the guess in the secret word?
        if (guess == word[i])
        {
            guessword[i] = guess;
            matches++;
        }
    }
    return matches;
}

void GameManager::getPlayerName()
{
    cout << "Please enter your name: ";
    cin >> player_name;
}

void GameManager::updateHighScores(int current_score)
{
    ifstream fin("data/scores.txt");

    priority_queue<pair<int, string>> pq;
    string name;
    int score;
    while (fin >> name >> score)
    {
        pq.push({ score, name });
    }
    pq.push({ current_score, player_name });
    fin.close();

    ofstream fout("data/scores.txt");
    for (int i = 0; !pq.empty() && i < 5; i++)
    {
        fout << pq.top().second << " " << pq.top().first << endl;
        pq.pop();
    }
    fout.close();
}

void GameManager::showHighScores()
{
    ifstream fin("data/scores.txt");
    system("cls");
    cout << "\tTOP SCORERS" << endl;

    string n;
    int score;
    int i = 1;
    while (fin >> n >> score)
    {
        cout << i << ". " << n << "   " << score << endl;
        i++;
    }
    if (i == 1)
    {
        cout << "No entries yet." << endl;
        cout << "Be the first one to get your name listed." << endl;
        cout << "Hurry up!" << endl;
    }

    cout << "---------------------------------------------" << endl;
    cout << endl;
    cout << "Press any key to continue." << endl;
    cin.ignore();
    cin.get();
}

void GameManager::displayStatistics()
{
    double timeSpent = difftime(endTime, startTime);

    cout << "\nGame Statistics:\n";
    cout << "Time spent: " << timeSpent << " seconds\n";
    cout << "Number of attempts: " << MAX_TRIES - attemptsUsed << endl;
    cout << "Word: " << word << endl;
    cout << "Player's letters: ";
    for (char letter : playerLetters) {
        cout << letter << ' ';
    }
    cout << '\n';
}
