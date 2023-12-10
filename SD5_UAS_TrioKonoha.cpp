#include <iostream>
#include <conio.h>
#include <time.h>
#include <set>
#include <windows.h>

using namespace std;

//define all text color
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

//function to set cursor position
void gotoxy(int x, int y)
{
    HANDLE hConsoleOutput;
    COORD dwCursorPosition;
    cout.flush();
    dwCursorPosition.X = x;
    dwCursorPosition.Y = y;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}

int BOARD_SIZE;
set<int> drawnNumbers; //contains the numbers that have been generated before

/* struct BingoCard{
    int numbers[10][10];
}; */

struct BingoInfo{
    int playerCard[5][5];
};

//Ask User to input board size
bool initializeBoardSize(){
    cout << "Masukkan ukuran dari papan bingo : ";
    cin >> BOARD_SIZE;
    if(BOARD_SIZE < 3 || BOARD_SIZE > 9){
        cout << RED << "Papan Bingo harus berukuran diantara 3x3 ~ 9x9" << endl << RESET;
        return 0;
    }
    return 1;
}

//Ask user to input the number of player
int setNumberOfPlayers(){
    int numberOfPlayers;
    cout << "Masukkan jumlah pemain           : ";
    cin >> numberOfPlayers;
    if(numberOfPlayers < 2 || numberOfPlayers > 4) 
        cout << RED << "Jumlah pemain harus diantara 2 ~ 4" << endl << RESET;
    return numberOfPlayers;
}

//Initialize all players' card with random number
void initializeGame(int numOfPlayers, BingoInfo players[]){
    for(int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++){
        set<int> generatedNumbers; //Declare a Set of numbers that have been generated previously
        int totalBoardNumbers = BOARD_SIZE * BOARD_SIZE; //set the maximum number in the board

        for(int row = 0; row < BOARD_SIZE; row++){
            for(int column = 0; column < BOARD_SIZE; column++){
                //prevent the number that have been generated previously from being inserted into the playerCard
                int randomNumber;
                do{
                    randomNumber = (rand() % totalBoardNumbers) + 1; //make a random number between 1 and totalBoardNumbers
                } while(generatedNumbers.count(randomNumber) > 0); //check whether the randomNumber is present in the generatedNumbers
                players[playerIndex].playerCard[row][column] = randomNumber;
                generatedNumbers.insert(randomNumber);
            }
        }
    }
}

//display all of the players' card and show the number that has been drawn in the card
void displayPlayersCard(int numOfPlayers, BingoInfo players[]){
    int x = 0, y = 0;
    for(int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++){
        gotoxy(x, y);
        cout << CYAN << "Pemain " << playerIndex + 1 << RESET;
        y++;

        for(int row = 0; row < BOARD_SIZE; row++){
            for(int column = 0; column < BOARD_SIZE; column++){
                //if the number has been drawn, then print the number with RED color
                if(drawnNumbers.find(players[playerIndex].playerCard[row][column]) != drawnNumbers.end()){
                    gotoxy(x, y);
                    cout << RED << players[playerIndex].playerCard[row][column] << RESET;
                    x += 4;
                } else{
                    gotoxy(x, y);
                    cout << players[playerIndex].playerCard[row][column];
                    x += 4;
                }
            }
            x = 0;
            y++;
        }
        y++;
    }
    cout<<endl;
}

//draw a random number that has never been generated before in the board
int drawNumber(){
    int totalBoardNumbers = BOARD_SIZE * BOARD_SIZE; //set the maximum number in the board
    int randomNumber;
    do{
        randomNumber = (rand() % totalBoardNumbers) + 1; //make a random number between 1 and totalBoardNumbers
    } while(drawnNumbers.count(randomNumber) > 0); //check whether the randomNumber is present in the generatedNumbers
    //insert the randomNumber to drawnNumbers so that it cannot be drawn ever again
    drawnNumbers.insert(randomNumber);
    return randomNumber;
}

//Check if the player has reach the pattern
bool checkWinner(BingoInfo player){
    int drawnNumberCount; //save the count of drawn number in a row or in a column
    //check if all numbers in a row have been drawn
    for(int row = 0; row < BOARD_SIZE; row++){
        drawnNumberCount = 0;
        for(int column = 0; column < BOARD_SIZE; column++){
            // drawnNumbers.find(players[playerIndex].playerCard[row][column]) != drawnNumbers.end()
            if(drawnNumbers.find(player.playerCard[row][column]) != drawnNumbers.end()){
                drawnNumberCount++;
            }
        }
        //if all numbers in a row have been drawn, then the player wins
        if(drawnNumberCount == BOARD_SIZE){
            return 1;
        }
    }

    //check if all numbers in a column have been drawn
    for(int column = 0; column < BOARD_SIZE; column++){
        drawnNumberCount = 0;
        for(int row = 0; row < BOARD_SIZE; row++){
            if(drawnNumbers.find(player.playerCard[row][column]) != drawnNumbers.end()){
                drawnNumberCount++;
            }
        }
        //if all numbers in a column have been drawn, then the player wins
        if(drawnNumberCount == BOARD_SIZE){
            return 1;
        }
    }

    //if the conditions above not fulfilled, then the player hasn't win
    return 0;
}

//display the winner's card
void displayWinner(BingoInfo winner){
    int x = 0, y = 0, drawnNumberCount = 0;
    for(int row = 0; row < BOARD_SIZE; row++){
        for(int column = 0; column < BOARD_SIZE; column++){
            //if the number has been drawn, then print the number with RED color
            if(drawnNumbers.find(winner.playerCard[row][column]) != drawnNumbers.end()){
                gotoxy(x, y);
                cout << RED << winner.playerCard[row][column] << RESET;
                x+=4;
            } else{
                gotoxy(x, y);
                cout << winner.playerCard[row][column];
                x+=4;
            }
        }
        x = 0;
        y++;
    }

    //check if all numbers in a row have been drawn
    x = 0, y = 0;
    for(int row = 0; row < BOARD_SIZE; row++){
        drawnNumberCount = 0;
        for(int column = 0; column < BOARD_SIZE; column++){
            if(drawnNumbers.find(winner.playerCard[row][column]) != drawnNumbers.end()){
                drawnNumberCount++;
            }
        }
        //if all numbers in a row have been drawn, then make all the output of the numbers in the row to be GREEN text
        if(drawnNumberCount == BOARD_SIZE){
            for(int col = 0; col < BOARD_SIZE; col++){
                gotoxy(x, y);
                cout << GREEN << winner.playerCard[row][col] << RESET;
                x+=4;
            }
        }
        y++;
    }

    //check if all numbers in a column have been drawn
    x = 0, y = 0;
    for(int column = 0; column < BOARD_SIZE; column++){
        drawnNumberCount = 0;
        for(int row = 0; row < BOARD_SIZE; row++){
            if(drawnNumbers.find(winner.playerCard[row][column]) != drawnNumbers.end()){
                drawnNumberCount++;
            }
        }
        //if all numbers in a column have been drawn, then make all the output of the numbers in the column to be GREEN text
        if(drawnNumberCount == BOARD_SIZE){
            for(int r = 0; r < BOARD_SIZE; r++){
                gotoxy(x, y);
                cout << GREEN << winner.playerCard[r][column] << RESET;
                y+=1;
            }
        }
        x+=4;
    }

    //set the cursor position into after the winner's card display
    x = 0;
    y = BOARD_SIZE;
    gotoxy(x,y);
}

//Main function to start and play the game
void playBingo(){
    int isSizeValid; //check whether board size fits the criteria
    do{
        isSizeValid = initializeBoardSize();
    } while(!isSizeValid);

    //check whether the number of player fits the criteria
    int playerNumbers; //number of players playing the game
    cout << endl;
    do{
        playerNumbers = setNumberOfPlayers();
    } while(playerNumbers < 2 || playerNumbers > 4);

    BingoInfo playersData[playerNumbers]; //Players data declaration
    initializeGame(playerNumbers, playersData);  //Initialize all players' board numbers    
    cout << YELLOW << "\nTekan sembarang tombol untuk memulai permainan!!!\n" << RESET;
    getch();

    //Start the game and check the winner
    int winner = -1;
    do{
        system("cls");

        //display all of the players' bingo card (rombak supaya bisa dibuat 2 buah playersData yg isinya sama) 
        displayPlayersCard(playerNumbers, playersData);

        //draw the number
        cout << YELLOW << "\nTekan sembarang tombol untuk mengundi angka!!!\n" << RESET;
        getch();
        int number = drawNumber(); 
        cout <<"Angka yang didapat adalah "<< RED << number << RESET << endl;
        cout << YELLOW << "\nTekan sembarang tombol untuk melanjutkan permainan ...\n" << RESET;
        getch();

        for(int playerIndex = 0; playerIndex < playerNumbers; playerIndex++){
            bool isWin = checkWinner(playersData[playerIndex]); //check if the player wins
            //if player win, then exit the loop
            if(isWin){
                winner = playerIndex;
            }
        }
    } while(winner == -1);

    //show the winner's card and congratulation text
    system("cls");
    displayWinner(playersData[winner]);
    cout << "\nPemenangnya adalah " << CYAN << "Pemain " << winner + 1 << endl << RESET;
    cout << YELLOW << "Selamat, Andalah pemenang dari game Bingo ini!!!\n" << RESET;
}

//Main Program
int main(){
    system("cls");
    //set seed for rand() function
    srand(time(0)); 
    //start the Bingo Game
    playBingo();
    return 0;
}