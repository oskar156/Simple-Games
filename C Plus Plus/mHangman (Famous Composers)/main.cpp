//Hangman (Famous Composers)

#include <iostream>
#include <string>
#include <vector>

bool again = true;
char guess;
int x, y, choice, lives, lost = 0, won = 0;

//There are 2 identical vectors: (1) 1st vector is for choosing the word and checking if the word is correct. 
//(2) The 2nd vector is for keeping track of the user's progress
std::vector<std::string> words = {
    
    "albinoni", 
    "bach", "bartok", "beethoven", "berlioz", "bernstein", "bizet", "boccherini", "borodin", "brahms", "britten", "bruckner", "buxtehude", 
    "chopin", "clementi", "couperin", "czerny", 
    "debussy", "dvorak", 
    "elgar", 
    "faure", 
    "gershwin", "granados", "grieg", 
    "handel", "haydn", 
    "khachaturian",
    "liszt", 
    "mahler", "mendelssohn", "messiaen", "monteverdi", "mozart", "mussorgsky", 
    "offenbach", 
    "pachelbel", "paganini", "palestrina", "prokofiev", "puccini", "purcell",
    "rachmaninov", "rameau", "ravel", "rimsky-korsakov", "rossini", "saint-saens", "salieri", 
    "scarlatti", "schoenberg", "schubert", "schumann", "shostakovich", "sibelius", "strauss", "stravinsky",
    "tchaikovsky",  "telemann", 
    "verdi", "vivaldi", 
    "wagner", "williams", 
    "villa-lobos", 
};

std::vector<std::string> newword = words;

int main(){
    
    std::cout << "Hangman (Famous Composers)" << std::endl;

    while(again==true) {
        
        std::cout << "A word has been chosen: " << std::endl << std::endl; 
    
        //A word is randomly chosen from the 2nd vector and turned into a series of dashes----
        srand(time(NULL));
        choice = rand() % words.size();
        
        for(x = 0; x < words[choice].size(); x++)
            newword[choice][x] = '-';

        std::cout << newword[choice] << std::endl;

        //As the user guesses a correct letter, the appropriate dash(es) turn back into the original letter(s).   
        for(lives = 5; lives > 0;) {
        
                std::cout << "Make a guess..." << std::endl;
                std::cin >> guess;
                
            y = 0;
            for(x = 0; x < words[choice].size(); x++) {
                
                if(guess == words[choice][x] || tolower(guess) == words[choice][x]) {
                    
                    newword[choice][x] = tolower(guess);
                    y = 1;
                }
            }
        
            if(y == 0)
                lives--;
                
            std::cout << newword[choice] << std::endl;
            std::cout <<"Lives Left: " << lives << std::endl;       
        
            //The game ends when the 2nd vector word matches the 1st vector word again, or if the user runs out of lives          
            if(newword[choice] == words[choice] && lives > 0) {
                
                std::cout<<"\n"
                <<"\n   -----------"
                <<"\n   |         |"
                <<"\n   |         |"
                <<"\n             |"
                <<"\n /_O_/ ~     |"
                <<"\n  _|_ ~      |"
                <<"\n_/___/_________"
                <<"\nYou Win!\n\n";
                won++;
                lives = 0;
                
            }
            else if(lives == 0) {
                
                std::cout<<"\n"
                <<"\n   -----------"
                <<"\n   |         |"
                <<"\n   0         |"
                <<"\n  /|/        |"
                <<"\n   |         |"
                <<"\n  / /        |"
                <<"\n             |"
                <<"\n_______________"
                <<"\nYou've been hung!"
                <<"\nThe correct word was: " << words[choice] << "\n\n";
                lost++;
            }
            
            if((newword[choice] == words[choice] && lives > 0) || lives == 0) {
                
                std::cout<<"Games won: "<<won<<"\tGames lost: " << lost << std::endl << std::endl;            
                std::cout<<"Press 1 to play again..." << std::endl;
                std::cout<<"Press 0 to end program..." << std::endl;
                std::cin>>again;
            }
            
            std::cout << std::endl;
        }
    }
    
    std::cout<<"Thanks for playing!";
}


