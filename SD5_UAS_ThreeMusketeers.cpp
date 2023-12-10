#include <iostream>
#include <conio.h>
#include <time.h>
#include <set>

using namespace std;

int BOARD_SIZE;
set<int> drawnNumbers; //contains the numbers that have been generated before

/* struct BingoCard{
    int numbers[10][10];
}; */

struct BingoInfo{
    int playerCard[5][5];
    // int drawnCount;
};

//Ask User to input board size
bool initializeBoardSize(){
    cout << "Masukkan ukuran dari papan bingo : ";
    cin >> BOARD_SIZE;
    if(BOARD_SIZE < 3 || BOARD_SIZE > 5){
        cout << "Papan Bingo harus berukuran diantara 3x3 ~ 5x5" << endl;
        return 0;
    }
    return 1;
}

//Ask user to input the number of player
int setNumberOfPlayers(){
    int numberOfPlayers;
    cout << "Masukkan jumlah pemain : ";
    cin >> numberOfPlayers;
    if(numberOfPlayers < 2 || numberOfPlayers > 4) 
        cout << "Jumlah pemain harus diantara 2 ~ 4" << endl;
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
    for(int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++){
        cout<<"Pemain "<<playerIndex+1<<endl;
        for(int row = 0; row < BOARD_SIZE; row++){
            for(int column = 0; column < BOARD_SIZE; column++){
                cout<<players[playerIndex].playerCard[row][column] << " ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
}

//draw a random number that has never been generated before in the board
int drawNumber(){
    int totalBoardNumbers = BOARD_SIZE * BOARD_SIZE; //set the maximum number in the board
    int randomNumber;
    do{
        randomNumber = (rand() % totalBoardNumbers) + 1; //make a random number between 1 and totalBoardNumbers
    } while(drawnNumbers.count(randomNumber) > 0); //check whether the randomNumber is present in the generatedNumbers
    return randomNumber;
}

//update every player card after the number has been drawn
void updateCard(int numOfPlayers, BingoInfo players[], int drawnNumber){
    for(int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++){
        //insert the drawnNumber to drawnNumbers so that it cannot be drawn ever again
        drawnNumbers.insert(drawnNumber);
        for(int row = 0; row < BOARD_SIZE; row++){
            for(int column = 0; column < BOARD_SIZE; column++){
                if(players[playerIndex].playerCard[row][column] == drawnNumber){
                    //Mark the number which is equal to drawnNumber in Arruy as 0  
                    players[playerIndex].playerCard[row][column] = 0; 
                    break;
                }
            }
        }
    }
}

//Check if the player has reach the pattern
bool checkWinner(BingoInfo player){
    int zeroCount; //save the count of zeros in a row or in a column
    //check if all numbers in a row is 0
    for(int row = 0; row < BOARD_SIZE; row++){
        zeroCount = 0;
        for(int column = 0; column < BOARD_SIZE; column++){
            if(player.playerCard[row][column] == 0){
                zeroCount++;
            }
        }
        //if all numbers in a row is zero, then the player wins
        if(zeroCount == BOARD_SIZE){
            return 1;
        }
    }

    //check if all numbers in a column is 0
    for(int column = 0; column < BOARD_SIZE; column++){
        zeroCount = 0;
        for(int row = 0; row < BOARD_SIZE; row++){
            if(player.playerCard[column][row] == 0){
                zeroCount++;
            }
        }
        //if all numbers in a column is zero, then the player wins
        if(zeroCount == BOARD_SIZE){
            return 1;
        }
    }

    //if the conditions above not fulfilled, then the player hasn't win
    return 0;
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
    cout << "\nTekan sembarang tombol untuk memulai permainan!!!\n";
    getch();

    //Start the game and check the winner
    int winner = -1;
    do{
        system("cls");

        //display all of the players' bingo card (rombak supaya bisa dibuat 2 buah playersData yg isinya sama) 
        displayPlayersCard(playerNumbers, playersData);

        //draw the number
        cout << "Tekan sembarang tombol untuk mengundi angka!!!\n";
        getch();
        int number = drawNumber(); 
        cout<<"Angka yang didapat adalah "<< number << endl;
        cout << "Tekan sembarang tombol untuk melanjutkan permainan ...\n";
        getch();

        for(int playerIndex = 0; playerIndex < playerNumbers; playerIndex++){
            updateCard(playerNumbers, playersData, number); //update player card data
            bool isWin = checkWinner(playersData[playerIndex]); //check if the player wins
            //if player win, then exit the loop
            if(isWin){
                winner = playerIndex;
            }
        }
    } while(winner == -1);

    //show the winner
    cout<<"Pemenangnya adalah Pemain "<< winner+1 <<endl;
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