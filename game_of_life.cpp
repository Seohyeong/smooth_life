#include <stdlib.h>
#include <time.h> 
#include <math.h>

#include <raylib.h>

#define BOARD_SIZE 1000
#define CELL_SIZE 1

#define KERNEL_SIZE 1

#define ROW BOARD_SIZE / CELL_SIZE
#define COL BOARD_SIZE / CELL_SIZE

#define SHBLACK {31,31,31,255}


struct State{
	int current_cells[ROW][COL];
	int next_cells[ROW][COL];
};


int min(int a, int b){
	if(a<=b){
		return a;
	} else {
		return b;
	}
}

int max(int a, int b){
	if(a>=b){
		return a;
	} else {
		return b;
	}
}


void init_cells(State& state){
	int i, j;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			// 1. random init
			state.current_cells[i][j] = rand() % 2;

			// // 2. X diagoal init
			// if(i==j){state.current_cells[i][j] = 1;}
			// if(i+j==ROW-1){state.current_cells[i][j] = 1;}
		}
	}

	// // 3. sine 
	// int i;
	// for(i=0; i<ROW; i++){
	// 	int y = (int)(sin(i/100.0) * (((double)ROW) / 2.0));
	// 	state.current_cells[i][y+ROW/2] = 1;
	// }

}


void get_next(State& state){
	int i, j;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			// loops for counting alive neighboring cells
			int cnt_alive=0;
			int ii, jj;
			for(ii=max(i-KERNEL_SIZE, 0); ii<=min(i+KERNEL_SIZE, ROW-1); ii++){
				for(jj=max(j-KERNEL_SIZE, 0); jj<=min(j+KERNEL_SIZE, COL-1); jj++){
					cnt_alive += state.current_cells[ii][jj];
				}
			}
			cnt_alive -= state.current_cells[i][j];

			// applying rules
			// Any live cell with fewer than two live neighbors // dies as if caused by underpopulation.
			// Any live cell with two or three live neighbors // lives on to the next generation.
			// Any live cell with more than three live neighbors // dies, as if by overpopulation.
			// Any dead cell with exactly three live neighbors // becomes a live cell, as if by reproduction.
			if(cnt_alive<2 && state.current_cells[i][j]==1){
				state.next_cells[i][j] = 0;
			}
			if((cnt_alive==2 && state.current_cells[i][j]==1) || (cnt_alive==3 && state.current_cells[i][j]==1)){
				state.next_cells[i][j] = 1;
			}
			if (cnt_alive>3 && state.current_cells[i][j]==1){
				state.next_cells[i][j] = 0;
			}
			if (cnt_alive==3 && state.current_cells[i][j]==0){
				state.next_cells[i][j] = 1;
			}
		}
	}

	// reset the current state
	// TODO: array is a pointer. Does this actually copy the value?
	// state.current_cells = state.next_cells;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			state.current_cells[i][j] = state.next_cells[i][j];
		}
	}
}


// if using state.current_cells as the argument, what is the type of it?
void render_state(const State& state){
	int i, j;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			if(state.current_cells[i][j]==1){
				DrawRectangle(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE, RAYWHITE);
			}
		}
	}
}


int main(){
	srand(time(0));

	State state{};

	InitWindow(BOARD_SIZE, BOARD_SIZE, "Game of Life");
	SetTargetFPS(60);

	init_cells(state);

	while (!WindowShouldClose()) {		
		// draw current state
		BeginDrawing();

		{
			ClearBackground(SHBLACK);
			render_state(state);
		}

		EndDrawing();

		// update state
		get_next(state);
	}
	CloseWindow();
	return 0;
}