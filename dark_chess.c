#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

enum team {
	red = 1,
	blue
};

struct Chess {
	int level;
	int team;
	bool alive;
	bool flip;
};

typedef struct Chess chess;

struct Direction {
	int x;
	int y;
};

typedef struct Direction direction;

direction which_direction(int input) {
	direction coordinate = { 0, 0 };
	if (input == UP) {
		coordinate.x = -1;
		coordinate.y = 0;
	}
	else if (input == LEFT) {
		coordinate.x = 0;
		coordinate.y = -1;
	}
	else if (input == DOWN) {
		coordinate.x = 1;
		coordinate.y = 0;
	}
	else if (input == RIGHT) {
		coordinate.x = 0;
		coordinate.y = 1;
	}	

	return coordinate;
}

void setTextColor(int color) {
	if (color == red) {
		printf("\033[0;31m");
	}
	else if (color == blue) {
		printf("\033[0;34m");
	}

	return;
}

void resetTextColor() {
	printf("\033[0m");

	return;
}

int main() {
	srand(time(NULL));
	chess board[4][8];
	chess* which_chess = NULL;
	bool used[32] = { false };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			int r = rand() % 32;
			while (used[r]) {
				r = rand() % 32;
			}
			board[i][j].alive = true;
			board[i][j].flip = false;
			board[i][j].team = r % 2 + 1;
			switch (r / 2) {
				case 0: case 1: case 2: case 3: case 4:
					board[i][j].level = 1;
					break;
				case 5: case 6:
					board[i][j].level = 2;
					break;
				case 7: case 8:
					board[i][j].level = 3;
					break;
				case 9: case 10:
					board[i][j].level = 4;
					break;
				case 11: case 12:
					board[i][j].level = 5;
					break;
				case 13: case 14:
					board[i][j].level = 6;
					break;
				case 15:
					board[i][j].level = 7;
					break;
			}
			used[r] = true;
		}
	}

	int position_x, position_y;
	static int red_chess_count = 16, blue_chess_count = 16;
	int player1_team, player2_team;
	static int steps = 0;
	while (1) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				if (!board[i][j].flip) {
					printf("?");
				}
				else if (!board[i][j].alive) {
					printf("-");
				}
				else {
					setTextColor(board[i][j].team);
					printf("%d", board[i][j].level);
					resetTextColor();
				}
				printf(" ");
			}
			printf("\n");
		}

		if (red_chess_count == 0) {
			setTextColor(blue);
			printf("Blue win!\n");
			resetTextColor();
			int quit = 0;
			printf("Enter 1 to quit");
			scanf("%d", &quit);
			if(quit){
				return 0;
			}
		}
		else if (blue_chess_count == 0) {
			setTextColor(red);
			printf("Red win!\n");
			resetTextColor();
			int quit = 0;
			printf("Enter 1 to quit");
			scanf("%d", &quit);
			if(quit){
				return 0;
			}
		}

		if (steps) {
			if (steps % 2 == 0) {
				printf("Player 1's color: ");
				if (player1_team == red) {
					setTextColor(red);
					printf("Red\n");
					resetTextColor();
				}
				else if (player1_team == blue) {
					setTextColor(blue);
					printf("Blue\n");
					resetTextColor();
				}
			}
			else if (steps % 2 == 1) {
				printf("Player 2's color: ");
				if (player2_team == red) {
					setTextColor(red);
					printf("Red\n");
					resetTextColor();
				}
				else if (player2_team == blue) {
					setTextColor(blue);
					printf("Blue\n");
					resetTextColor();
				}
			}
		}

		if (steps % 2 == 0) {
			printf("Player 1's turn: \n");
		}
		else if (steps % 2 == 1) {
			printf("Player 2's turn: \n");
		}

		flip_or_move:
		printf("Enter the chess you want to flip or move (row: 0 ~ 3, column: 0 ~ 7)\n");
		scanf("%d %d", &position_x, &position_y);
		if (((position_x < 0) || (position_x >= 4)) || ((position_y < 0) || (position_y >= 8))) {
			printf("Out of board!\n");
			goto flip_or_move;
		}
		which_chess = &board[position_x][position_y];

		if (!which_chess->flip) {
			which_chess->flip = true;
			if (!steps) {
				player1_team = which_chess->team;
				player2_team = 3 - which_chess->team;
			}
		}
		else {
			if (steps % 2 == 0) { // player 1
				if (!which_chess->alive) {
					printf("This space is empty!\n");
					goto flip_or_move;
				}
				if (which_chess->team != player1_team) {
					printf("You can't move opponent's chess!\n");
					goto flip_or_move;
				}
				int dir_input;
				printf("Enter the direction you want to move: (1: up, 2: left, 3: down, 4: right)");
				scanf("%d", &dir_input);
                if((dir_input < 1) || (dir_input > 4)){
                    printf("Invalid input!\n");
                    goto flip_or_move;
                }
				if (((position_x + which_direction(dir_input).x < 0) || (position_x + which_direction(dir_input).x >= 4)) || (((position_y + which_direction(dir_input).y) < 0) || (position_y + which_direction(dir_input).y >= 8))) {
					printf("Out of board!\n");
					goto flip_or_move;
				}
				if (!board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].flip) {
					// still face down
					printf("You can't move toward this way!\n");
					goto flip_or_move;
				}
				else if (!board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].alive) {
					// move
					chess temp = board[position_x][position_y];
					board[position_x][position_y] = board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y];
					board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y] = temp;
				}
				else if (board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].team == player1_team){
					printf("You can't kill your own chess!\n");
					goto flip_or_move;
				}
				else if (board[position_x][position_y].level < board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].level) {
					if (!((board[position_x][position_y].level == 1) && (board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].level == 7))) {
						printf("You can't kill a higher level chess! (Expect 1 and 7 can kill each other)\n");
						goto flip_or_move;
					}
					else {
						// kill
						board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].alive = false;
						chess temp = board[position_x][position_y];
						board[position_x][position_y] = board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y];
						board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y] = temp;
						if (player2_team == red) {
							red_chess_count--;
						}
						else if (player2_team == blue) {
							blue_chess_count--;
						}
					}
				}
				else {
					// kill
					board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].alive = false;
					chess temp = board[position_x][position_y];
					board[position_x][position_y] = board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y];
					board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y] = temp;
					if (player2_team == red) {
						red_chess_count--;
					}
					else if (player2_team == blue) {
						blue_chess_count--;
					}
				}
				
			}
			else if (steps % 2 == 1) {	//player 2
				if (!which_chess->alive) {
					printf("This space is empty!\n");
					goto flip_or_move;
				}
				if (which_chess->team != player2_team) {
					printf("You can't move opponent's chess!\n");
					goto flip_or_move;
				}
                int dir_input;
				printf("Enter the direction you want to move: (1: up, 2: left, 3: down, 4: right)\n");
				scanf("%d", &dir_input);
                if((dir_input < 1) || (dir_input > 4)){
                    printf("Invalid input!\n");
                    goto flip_or_move;
                }
				if (((position_x + which_direction(dir_input).x < 0) || (position_x + which_direction(dir_input).x >= 4)) || (((position_y + which_direction(dir_input).y) < 0) || (position_y + which_direction(dir_input).y >= 8))) {
					printf("Out of board!\n");
					goto flip_or_move;
				}
				if (!board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].flip) {
					// still face down
					printf("You can't move toward this way!\n");
					goto flip_or_move;
				}
				else if (!board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].alive) {
					// move
					chess temp = board[position_x][position_y];
					board[position_x][position_y] = board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y];
					board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y] = temp;
				}
				else if (board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].team == player2_team) {
					printf("You can't kill your own chess!\n");
					goto flip_or_move;
				}
				else if (board[position_x][position_y].level < board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].level) {
					if (!((board[position_x][position_y].level == 1) && (board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].level == 7))) {
						printf("You can't kill a higher level chess! (Expect 1 and 7 can kill each other)\n");
						goto flip_or_move;
					}
					else {
						// kill
						board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].alive = false;
						chess temp = board[position_x][position_y];
						board[position_x][position_y] = board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y];
						board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y] = temp;
						if (player1_team == red) {
							red_chess_count--;
						}
						else if (player1_team == blue) {
							blue_chess_count--;
						}
					}
				}
				else {
					// kill
					board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y].alive = false;
					chess temp = board[position_x][position_y];
					board[position_x][position_y] = board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y];
					board[position_x + which_direction(dir_input).x][position_y + which_direction(dir_input).y] = temp;
					if (player1_team == red) {
						red_chess_count--;
					}
					else if (player1_team == blue) {
						blue_chess_count--;
					}
				}
			}
		}
		
		steps++;
		system("cls");
	}
}