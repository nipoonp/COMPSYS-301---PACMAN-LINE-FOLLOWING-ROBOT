#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

void dfs(int starting_r, int starting_c, int final_r , int final_c);
void recurs(int row, int col, int starting_r, int starting_c, int final_r , int final_c);
void findpath(int row, int col);

int r = 15;
int c = 19;

int path_array[15*19][2] = {0};

int distance_map[15][19] = {0};
int locations_visited[15][19] = {0};

int turns_array[15*19] = {0};
int turnArrayCounter = 0;
int initDirection = 2;

// convert map to matrix
int original_map[15][19] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	                        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	                        {1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
	                        {1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	                        {1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,1},
	                        {1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1},
	                        {1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1},
	                        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
	                        {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1},
	                        {1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
	                        {1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1},
	                        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
	                        {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1},
	                        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};


void dfs(int starting_r, int starting_c, int final_r , int final_c) {
    // instantiate distance map with -1
    int i = 0;
    int j = 0;

    for (i = 0; i < r; i++){
        for (j = 0; j < c; j++){
            locations_visited[i][j] = 1;
        }
    }

    for (i = 0; i < r; i++){
        for (j = 0; j < c; j++){
            distance_map[i][j] = -1;
        }
    }

    // set distance to starting location as 0
    distance_map[starting_r][starting_c] = 0;

    recurs(starting_r, starting_c, starting_r, starting_c, final_r , final_c);
    //left out line 56
    findpath(final_r,final_c);




    int b = distance_map[final_r][final_c];
    int a = 0;
    int direction_array[15*19] = {0};

    int loop_c = 0;
    for(loop_c = 0; loop_c < (15*19); loop_c++){
        direction_array[loop_c] = 4;
    }

    while((a+1) != (b+1)){
    	
    	if (((path_array[a+1][0] - path_array[a][0]) == 1) && ((path_array[a+1][1] - path_array[a][1]) == 0)){
    		direction_array[a] = DOWN;
    	} else if (((path_array[a+1][0] - path_array[a][0]) == 0) && ((path_array[a+1][1] - path_array[a][1]) == 1)){
    		direction_array[a] = RIGHT;
    	} else if (((path_array[a+1][0] - path_array[a][0]) == -1) && ((path_array[a+1][1] - path_array[a][1]) == 0)){
    		direction_array[a] = UP;
    	} else if (((path_array[a+1][0] - path_array[a][0]) == 0) && ((path_array[a+1][1] - path_array[a][1]) == -1)){
    		direction_array[a] = LEFT;
    	}
    	a++;
    }

    a = 0;
    int zeroCounter = 0;
    int xx = 0;
    int yy = 0;
    while((a+1) != (b+1)){

    	xx = path_array[a+1][0];
    	yy = path_array[a+1][1];

    	zeroCounter = 0;
    	if (original_map[xx+1][yy] == 0){
			zeroCounter++;
    	}
    	if (original_map[xx-1][yy] == 0){
			zeroCounter++;
    	} 
    	if (original_map[xx][yy+1] == 0){
			zeroCounter++;
    	} 
    	if (original_map[xx][yy-1] == 0){
			zeroCounter++;
    	}


    	if ((direction_array[a+1] - direction_array[a]) != 0){

    		int turn = direction_array[a+1] - direction_array[a];

    		if (turn == 2 || turn == -1 || turn == -3){
    			turns_array[turnArrayCounter] =  LEFT;
    		} else if (turn == 3 || turn == 1 || turn == -2) {
    			turns_array[turnArrayCounter] =  RIGHT;
    		}

			turnArrayCounter++;
    	}
		if ((zeroCounter > 2) && ((direction_array[a+1] - direction_array[a]) == 0)){
			turns_array[turnArrayCounter] =  UP;
			turnArrayCounter++;
		}

    	a++;
    }



int k = 0;
for (k = 0; k < turnArrayCounter; k++){
	printf("turns_array[%d] = %d\n", k, turns_array[k]);
}

}


void recurs(int row, int col, int starting_r, int starting_c, int final_r , int final_c){
    // append map to show visited
    locations_visited[row][col] = 0;
    // get current distance final_rom start
    int val = distance_map[row][col];
    // check cardinal directions only


    int i = 0;
    int rowN = 0;
    int colN = 0;
    for (i = 1; i < 5; i++){

        switch(i) {
                case 1 :
                    rowN=row+1;
                    colN=col;
                    break;
                case 2 :
                    rowN=row-1;
                    colN=col;
                    break;
                case 3 :
                    rowN=row;
                    colN=col+1;
                    break;
                case 4 :
                    rowN=row;
                    colN=col-1;
                    break;
        }


        // check location valid
        if(rowN < r && colN < c && rowN>=0 && colN>=0) {
            // check to makesure unexplored and not a wall
            if(((distance_map[rowN][colN] == -1) || distance_map[rowN][colN] > val + 1) && (original_map[rowN][colN] == 0)){
                // set distance final_rom start
                distance_map[rowN][colN] = val + 1;
                // if at finish, end
                if ((rowN == final_r) && (colN == final_c)){
                    return;
                }
                // iterate on new location
                recurs(rowN, colN, starting_r, starting_c, final_r , final_c);
            }
        }

    }
}


void findpath(int row, int col) {

    // append location to path map in reverse order
    int pos = distance_map[row][col];
    //printf("%d %d %d\n", row, col, pos);

    path_array[pos][0] = row;
    path_array[pos][1] = col;




    int rowN = 0;
    int colN = 0;
    int run = 1;
    int i = 1;
    // using while loop instead of for so that we only check neccessary
    // neighbors
    while((i < 5) && (run)) {
        switch(i) {
                case 1 :
                    rowN=row+1;
                    colN=col;
                    break;
                case 2 :
                    rowN=row-1;
                    colN=col;
                    break;
                case 3 :
                    rowN=row;
                    colN=col+1;
                    break;

                case 4 :
                    rowN=row;
                    colN=col-1;
                    break;
        }
        // look for lower value neighbor
            if(rowN < r && colN < c && rowN>=0 && colN>=0){
                int check = distance_map[rowN][colN];
                int curr = distance_map[row][col];
                if((check != -1) && (check < curr)) {
                    run = 0;

                    findpath(rowN,colN);
                }
            }

        i = i + 1;
    }
}


int main() {

    dfs(13,17,11,5);

}