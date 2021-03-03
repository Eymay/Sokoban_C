#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define MAP_HEIGHT 6
#define MAP_WIDTH 7 //with \n character 6 + 1

//void save_game(int level, FILE ** load_ptr, char * map);
int saved_map_builder(FILE * load_ptr, char * map);
void map_builder(int b, char * map);
void destination_builder(int b, int * dest_ptr, char * map);
void unicode_printer(char * map);
int arrow_key(void);
void movement(int * dest_ptr, int * exit_ptr, int key, char * map);
int dest_finished(int level, char * map);

int main(){
    system("chcp 65001");//for unicode printing

    int key = 0;//var for input key

    int on_a_destination = 0;//var is 1 if the player is on a destination
    int *dest_ptr;
    dest_ptr = &on_a_destination;
    int on_exit = 0;//var is 1 if the player is on an exit
    int *exit_ptr;
    exit_ptr = &on_exit;

    printf("          Main Menu\n1.New Game (type and enter 1) \n2.Resume Game (type and enter 2)\n3.Exit (type and enter 3)\n");
    char input;
    input = getchar();

    FILE *load_ptr;//file pointer
    int level = 1;
    

    if(input == '1'){
        do {char map[]= {//default string of map. Built upon for final map
            "######\n"
            "#    #\n"
            "#    #\n"
            "#    #\n"
            "#    #\n"
            "######\n"
            };
            printf("Level %d\n", level);
            map_builder(level, map);//pulls string from file, extracts it, parses it as int array, then builds the raw non-unicode map
            destination_builder(level,dest_ptr, map);//adds the destination crosses on top of map
            while(1){//single level entrance
            key = arrow_key();//reads user input continously, 1,2,3,4 are for up, down, right and left. Returns 5 for reset, 6 for save, 7 for menu requests.
            if(key == 6){//if s is pressed to save
                load_ptr = fopen("savedgame.txt", "w");//the file where the game map is copied
                fwrite(map, 1, sizeof(map), load_ptr);
                fclose(load_ptr);
                load_ptr = fopen("savedlevel.txt", "w");//the file where the level is copied
                fprintf(load_ptr, "%d", level);
                fclose(load_ptr);
                
               
               printf("Game Saved\n");
            }if(key == 7){//if m is pressed for menu
                printf("Move to Menu?(type and enter ok or no)\n");
                char confirm_menu[2];
                char ok[] = "ok";
                scanf("%s", &confirm_menu);

                if(strcmp(confirm_menu, ok) == 0){//if user types ok
                    load_ptr = fopen("savedgame.txt", "w");//Saves automatically
                    fwrite(map, 1, sizeof(map), load_ptr);
                    fclose(load_ptr);
                    load_ptr = fopen("savedlevel.txt", "w");//saves level
                    fprintf(load_ptr, "%d", level);
                    fclose(load_ptr);
                    printf("Game Saved\n");

                    printf("          Main Menu\n1.New Game (type and enter 1) \n2.Resume Game (type and enter 2)\n3.Exit (type and enter 3)\n");
                    
                    int input_menu = 0;
                    scanf("%d", &input_menu);
                    if(input_menu == 1){//New game, makes the level 1 and breaks the inner loop so that it starts from beginning
                        level = 1;
                        break;
                    }else if(input_menu == 2){//Resume game, copies the map saved to savedgame file and returns the level information from savedlevel.txt
                        level = saved_map_builder(load_ptr, map);
                    }else if(input_menu == 3) return 0;//exit

                }}if(key == 5){//Reset, makes level 1 and builds the map as default
                printf("You will lose all the progress unless saved. Reset Game?(type and enter ok or no)\n");
                char confirm_reset[2];
                char ok[] = "ok";
                scanf("%s", &confirm_reset);
                if(strcmp(confirm_reset, ok) == 0){//confirms resetting
                    printf("resetting\n");
                    level = 1;
                    char temp[]= {
                    "######\n"
                    "#    #\n"
                    "#    #\n"
                    "#    #\n"
                    "#    #\n"
                    "######\n"
                    };
                    for(int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++){//copies everything inside temp to map string
                        map[i] = temp[i];
                    }
                    on_a_destination = 0;//resets on a destination variable for newer games
                    printf("Level %d", level);
                    map_builder(level, map);
                    printf("%s", map);
                    destination_builder(level,dest_ptr, map);
                    printf("%s", map);
                }
            }//after conditionals the game is played here
            movement(dest_ptr, exit_ptr,key, map);//prints the player to desired location and applies the rules
            if(on_exit == 1 && dest_finished(level, map) == 1){//checks if level is passed
              level++; 
              on_exit = 0; 
              break;//increments level, resets on exit information and breaks from game loop
            } 
            printf("Level %d\n", level);
            unicode_printer(map);//where the raw map is printed with unicode at every iteration

        }
        }while(level<11);
              
        printf("You Win");

    }else if(input == '2'){//Resume Game, very similar to new game option
            char map[MAP_WIDTH*MAP_HEIGHT];//defines arbitrary map sized string
            
            int level = saved_map_builder(load_ptr, map);//returns saved level and builds saved map
            printf("Level %d", level);
            while(1){
            key = arrow_key();
            if(key == 6){
                load_ptr = fopen("savedgame.txt", "w");
                fwrite(map, 1, sizeof(map), load_ptr);
                fclose(load_ptr);
                load_ptr = fopen("savedlevel.txt", "w");
                fprintf(load_ptr, "%d", level);
                fclose(load_ptr);
            }
            if(key == 7){
                printf("Move to Menu?\n(type and enter ok or no)\n");
                char confirm_menu[2];
                char ok[] = "ok";
                scanf("%s", &confirm_menu);
                if(strcmp(confirm_menu, ok) == 0){
                    load_ptr = fopen("savedgame.txt", "w");
                    fwrite(map, 1, sizeof(map), load_ptr);
                    fclose(load_ptr);
                    load_ptr = fopen("savedlevel.txt", "w");
                    fprintf(load_ptr, "%d", level);
                    fclose(load_ptr);
                    printf("          Main Menu\n1.New Game (type and enter 1) \n2.Resume Game (type and enter 2)\n3.Exit (type and enter 3)\n");
                    int input_menu = 0;
                    scanf("%d", &input_menu);
                    if(input_menu == 1){
                        level = 1;
                        break;
                    }else if(input_menu == 2){
                        level = saved_map_builder(load_ptr, map);
                    }else if(input_menu == 3) return 0;
                }}if(key == '5'){
                printf("You will lose all the progress unless saved. Reset Game?\n(type and enter ok or no)\n");
                char confirm_reset[2];
                char ok[] = "ok";
                scanf("%s", &confirm_reset);
                if(strcmp(confirm_reset, ok) == 0){
                    level = 1;
                    char map[]= {
                    "######\n"
                    "#    #\n"
                    "#    #\n"
                    "#    #\n"
                    "#    #\n"
                    "######\n"
                    };
                    printf("Level %d", level);
                    map_builder(level, map);
                    destination_builder(level,dest_ptr, map);
                }
                }
            movement(dest_ptr, exit_ptr,key, map);
            if(on_exit == 1 && dest_finished(level, map) == 1){
              level++; 
              on_exit = 0; 
              break;
            } 
            printf("\nLevel %d\n", level);
            unicode_printer(map);

            }//after the saved game level begins the game loop from next level
        do {char map[]= {
            "######\n"
            "#    #\n"
            "#    #\n"
            "#    #\n"
            "#    #\n"
            "######\n"
            };
            
            map_builder(level, map);
            destination_builder(level,dest_ptr, map);
            
            while(1){
            key = arrow_key();
            if(key == 6 ){
                load_ptr = fopen("savedgame.txt", "w");
                fwrite(map, 1, sizeof(map), load_ptr);
                fclose(load_ptr);
                load_ptr = fopen("savedlevel.txt", "w");
                fprintf(load_ptr, "%d", level);
                fclose(load_ptr);

            }if(key == 7){
                printf("Move to Menu?(type and enter ok or no)");
                char confirm_menu[2];
                char ok[] = "ok";
                scanf("%s", &confirm_menu);
                if(strcmp(confirm_menu, ok) == 0){
                    load_ptr = fopen("savedgame.txt", "w");
                    fwrite(map, 1, sizeof(map), load_ptr);
                    fclose(load_ptr);
                    load_ptr = fopen("savedlevel.txt", "w");
                    fprintf(load_ptr, "%d", level);
                    fclose(load_ptr);
                    printf("          Main Menu\n1.New Game (type and enter 1) \n2.Resume Game (type and enter 2)\n3.Exit (type and enter 3)\n");
                    int input_menu = 0;
                    scanf("%d", &input_menu);
                    if(input_menu == 1){
                        level = 1;
                        break;
                    }else if(input_menu == 2){
                        level = saved_map_builder(load_ptr, map);
                    }else if(input_menu == 3) return 0;
                }
                
            }
            movement(dest_ptr, exit_ptr,key, map);
            if(on_exit == 1 && dest_finished(level, map) == 1){
              level++; 
              on_exit = 0; 
              break;
            } 
            printf("Level %d\n", level);
            unicode_printer(map);

        }
        }while(level<11);//when level 10 is passed breaks and You Win messages pops
              
        printf("You Win");
    
    }if(input == '3'){//exit
        printf("closing...");
        return 0;
    }
           
   
    return 0;
}


int saved_map_builder(FILE * load_ptr, char * map){
            load_ptr = fopen("savedgame.txt", "r");
            fread(map, MAP_HEIGHT * MAP_WIDTH, 1, load_ptr);//copies map from the txt file
            fclose(load_ptr);

            load_ptr = fopen("savedlevel.txt", "r");
            char lvl;
            lvl = fgetc(load_ptr);
            fclose(load_ptr);
            int level = lvl - '0';
            return level;
}

void map_builder(int b, char * map){
    int level_lengths[10] = {21, 21, 21, 21, 21, 21, 25, 21, 25, 38};//string lengths are defined for easy implementation despite reducing reproducibility
   FILE *fptr;
    int level = b;
    char str[level_lengths[level - 1]]; 
    fptr = fopen("levels.txt", "r");//file where all map information is in
    if(fptr == NULL){//if txt file cannot be opened creates the file
        printf("No saved default levels file found\n");
        printf("Loading file for further games\n");
        char games[] = "1;3,5;3,3,3,4;4,4;2,1\n2;3,5;3,3,3,4;4,4;2,1\n3;2,3;3,3,3,4;4,4;2,1\n4;3,5;3,3,3,4;4,4;2,1\n5;2,3;2,4,4,3;4,4;2,1\n6;2,3;4,3,3,3;4,4;2,1\n7;2,3;3,3,3,4;4,4,5,2;2,1\n8;5,4;4,3,4,4;3,2;2,1\n9;5,2;3,3,3,4,4,2;4,4;1,3\n10;4,3;3,3,3,4;5,2,5,4,5,5,4,4,4,5;2,1";
        fptr = fopen("levels.txt", "w+");
        fwrite(games, 1, sizeof(games), fptr);
        
        
    }
    rewind(fptr);
    for(int i = 0; i < level - 1; i++){//finds the beginning of string
        fseek(fptr, level_lengths[i] + 2, SEEK_CUR);
    }
    fread(str, level_lengths[level-1], 1, fptr);
    fclose(fptr);
    
    printf("%s\n", str);//a workaround for an unsolved problem we faced
    str[level_lengths[level - 1]] = '\0';
    str[level_lengths[level - 1] + 1] = '\0';
    printf("%s\n", str);

    int length = level_lengths[level - 1];//for loop count
    int semicolon = 0;
    int comma = 0;
    int element_count = 0;//to find two digit and single digit numbers we use length and comma and semicolon numbers

    for(int i = 0; i < length; i++){//counts comma and semicolon, indeed semicolon is actually a fixed number
        if(str[i] == 44) comma++;//ascii values of comma and semicolon
        if(str[i] == 59) semicolon++;
    }

    element_count = semicolon + comma + 1;
    int units = element_count + semicolon; //uses semicolon for boundaries of information
    
    int info[units];

    int k = 0;//adds iself with i at the end of loop
    int i = 0;//like a pointer jumps two or three char depending on the condition, on the string
    int j = 0;//int array index we create 
    while(i < length){
        if(str[i+1] == '\0') {//last character, at the top to prevent issues onward
            info[j] = str[i] - '0';
            
            break;
        }
        if(str[i + 1] == 44){//if next char is comma
            info[j] = str[i] - '0';
            k = 2;//jump twice
        }
        if(str[i + 1] != 44 && str[i+1] != 59){//if next char is not comma and semicolon, two digit number
            info[j] = 10*(str[i] - '0') + (str[i+1] - '0');//two char become one number
            k = 3;//jump three times
        }
        if(str[i + 1] == 59){//if next char is semicolon
            info[j] = str[i] - '0';
            info[j + 1] = 0;//separator of information
            k = 2;//jump twice
            j++;//array jumps twice
        }
        i = i + k;
        j++;
    }

 
    /*for(int i = 0; i < units; i++){//for debugging and optionally to see the int output
        printf("%d ", info[i]);
    }
    printf("\n");*/
   

    //very similar approach as in the last loop
    //zeros are made from semicolons
    k = 2;
    j = 1;
    int index = 0;
    int zero_count = 1;
    while( k < units){
        if(info[k] != 0 && info[k+1] != 0){//searches for x y coordinates and converts it to one dimensional number
            index = (info[k] - 1) * MAP_WIDTH + info[k+1] - 1;
            j = 2;
        } 
        if(info[k] == 0){//if separator is seen 
            zero_count++;
            j = 1;
            k +=j;
            continue;
        }
        k +=j;
        switch (zero_count){//places units according to how many zeros are counted
        case 1:
            map[index] = '@'; //player
            
            continue;
        case 2:
            map[index] = 'B'; //boxes
            
            continue;
        case 3:
            map[index] = '#';//wall
            
            continue;
        case 4:
            map[index] = 'E';//Exit
            
            continue;
        
        }//no destination because it's not in the file, separate function builds it
      
    }
}

void destination_builder(int level, int * dest_ptr, char * map){
    int dest1[] = {4,2,5,3,5};//size of array(how many times the loop will go on) at the beginning and the destination positions
    int dest2[] = {4,2,5,5,2};
    int dest3[] = {4,2,5,4,5};
    int dest4[] = {4,2,5,5,3};
    int dest5[] = {4,2,5,4,5};
    int dest6[] = {4,2,5,5,5};
    int dest7[] = {4,2,5,5,3};
    int dest8[] = {4,3,5,5,3};
    int dest9[] = {6,2,2,2,5,5,5};
    int dest10[] = {4,2,5,5,3};
    int *dest_all[] = {dest1, dest2, dest3, dest4, dest5, dest6, dest7,dest8, dest9, dest10};
    
    int i = 1;
    int index = 0;
    while(i < *dest_all[level - 1]){//very similar to map builder but this does with pointer instead
        index = (*(dest_all[level-1] + i) - 1) * MAP_WIDTH + *(dest_all[level-1] + i + 1) - 1;

        if(map[index] == '@'){//if the destination is under the player start position 
            *dest_ptr = 1;//on_a_destination becomes 1
            i +=2;
            continue;//doesn't print x
        }
        map[index] = 'x';
        i +=2;
    }
    
}

void unicode_printer(char * map){
       for(int i = 0; i < 6*7; i++){
       switch (map[i]){
       case '#':
           printf("\u25A0");//\u2610
           printf("%c", ' ');
           continue;
       
       case ' ':
            printf("%c", ' ');
            printf("%c", ' ');
            continue;
        case '\n':
            printf("%c", '\n');
            
            continue;
        case '@':
            printf("\u25B6");//\u25B6
            printf("%c", ' ');
            continue;
        case 'B'://wall olacakti !!!!!!
            printf("\u2610");
            printf("%c", ' ');
            continue;
        case 'E':
            printf("\u2190");
            printf("%c", ' ');
            continue;
        case 'x':
            printf("\uFF58");
            //printf("%c", ' ');
            continue;
        case 'O':
            printf("\u2612");
            printf("%c", ' ');
       }           
   }
}

int arrow_key(void){//returns integer according to key pressed
    int ch = getch();//couldn't find standard alternative for arrow keys
    switch(ch){
        case 119://w for up, if arrow keys don't work these may work on a different pc 
            return 1;
        case 120://x for down
            return 2;
        case 100://d for right
            return 3;
        case 97://a for left
            return 4;
        case 114:
            return 5; //r for reset
        case 115:
            return 6; //s for save
        case 109:
            return 7; //m for menu
    }
    if (ch == 224 || ch == 0) { //with single arrow key, getch is run twice and gets two values
    switch(getch()) { // the real value
        case 72:
            return 1;// code for arrow up
            
        case 80:
            return 2;// code for arrow down
            
        case 77:
            return 3;// code for arrow right
            
        case 75:
            return 4;// code for arrow left
            
    }
}
};

void movement(int * dest_ptr, int * exit_ptr, int key, char * map){
    int player_position = 0;

    for(int i = 0; 0 <MAP_HEIGHT * MAP_WIDTH; i++){//finds the player on the map
        if(map[i] == '@'){
            player_position = i;
            break;
        } 
    }

    int index = 0;//where the player intends to go
    int further_index = 0;//the further of where the player wants to go
    switch (key)
    {
    case 1://up
        index = player_position - MAP_WIDTH;
        further_index = player_position - 2*MAP_WIDTH;
        break;
    case 2://down
        index = player_position + MAP_WIDTH;
        further_index = player_position + 2*MAP_WIDTH;
        break;
    case 3://right
        index = player_position + 1;
        further_index = player_position + 2;
        break;
    case 4://left
        index = player_position - 1;
        further_index = player_position - 2;
        break;   
    }
    if(map[index] == 'B'){//if there's a box in the direction
            if(map[further_index]  == ' '){//and the box has space behind
                map[further_index] = 'B';
                map[index] = '@';

                if(*dest_ptr == 1){//checks if player is on a destination or exit
                map[player_position] = 'x';
                *dest_ptr = 0;
                }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }
                *dest_ptr = 0;//definitely not on a dest or exit anymore
                *exit_ptr = 0;
            } 
            if(map[further_index] == 'x'){//and the box has a destination behind it
                map[further_index] = 'O';//becomes a special char to differentiate
                map[index] = '@';

                if(*dest_ptr == 1){
                map[player_position] = 'x';
                *dest_ptr = 0;
            }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }
                *dest_ptr = 0;
                *exit_ptr = 0;
            }
            
            
        }
        if(map[index] == 'O'){//similar to box situation but *dest_ptr will change here
            if(map[further_index]  == ' '){
                map[further_index] = 'B';
                map[index] = '@';
                if(*dest_ptr == 1){
                map[player_position] = 'x';
                *dest_ptr = 0;
            }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }
                *dest_ptr = 1;
                *exit_ptr = 0;
            } 
            if(map[further_index] == 'x'){
                map[further_index] = 'O';
                map[index] = '@';
                if(*dest_ptr == 1){
                map[player_position] = 'x';
                *dest_ptr = 0;
            }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }
                *dest_ptr = 1;
                *exit_ptr = 0;
            }
            
        }
        if(map[index] == ' '){//if the player wants to go to an empty space
            map[index] = '@';
            if(*dest_ptr == 1){
                map[player_position] = 'x';
                *dest_ptr = 0;
            }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }  
        }
        if(map[index] == 'x'){//if the player wants to go to a destination
            map[index] = '@';

            if(*dest_ptr == 1){
                map[player_position] = 'x';
                *dest_ptr = 0;
            }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }
            *dest_ptr = 1;
            *exit_ptr = 0;
        }
        if(map[index] == 'E'){//if the player wants to go to exit
            map[index] = '@';
            if(*dest_ptr == 1){
                map[player_position] = 'x';
                *dest_ptr = 0;
            }else{
                if(*exit_ptr == 1){
                    map[player_position] = 'E';
                    *exit_ptr = 0;
                }else{
                    map[player_position] = ' ';
                }
                
            }
            *dest_ptr = 0;
            *exit_ptr = 1;
        }
        //printf("%s", map);//if unicode not functions well can be commented out for raw version
    }
    
    
    


int dest_finished(int level, char * map){//counts correctly placed boxes and returns one if all finished
    int dest_finish_count = 0;
    int level_done = 0;
    for(int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++){
        if(map[i] == 'O') dest_finish_count++;
        }
        if(level == 9){//level 9 has 3 boxes
           if(dest_finish_count == 3) level_done = 1; 
        }else{//all other levels have 2 boxes
           if(dest_finish_count == 2) level_done = 1; 
        }
    return level_done;       
}