#include <stdlib.h>
#include <time.h> 

#include <raylib.h>

#define CELL_SIZE 1
#define ROW 800 // TODO: can this be inside State struct?
#define COL 800


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
			state.current_cells[i][j] = rand() % 2;
		}
	}
}


void get_next(State& state){
	int i, j;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			// loops for counting alive neighboring cells
			int cnt_alive=0;
			int ii, jj;
			for(ii=max(i-1, 0); ii<=min(i+1, ROW); ii++){
				for(jj=max(j-1, 0); jj<=min(j+1, COL); jj++){
					cnt_alive += state.current_cells[ii][jj];
				}
			}
			cnt_alive -= state.current_cells[i][j];

			// applying rules
			// Any live cell with fewer than two live neighbors dies as if caused by underpopulation.
			// Any live cell with two or three live neighbors lives on to the next generation.
			// Any live cell with more than three live neighbors dies, as if by overpopulation.
			// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
			if(cnt_alive<2){
				state.next_cells[i][j] = 0;
			} else if(cnt_alive == 2 || cnt_alive == 3){
				state.next_cells[i][j] = 1;
			} else {
				state.next_cells[i][j] = 0;
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
				DrawRectangle(i, j, CELL_SIZE, CELL_SIZE, RAYWHITE);
			}
		}
	}
}


int main(){
	srand(time(NULL));

	State state{};

	InitWindow(800, 800, "Game of Life");
	SetTargetFPS(60);

	init_cells(state);

	while (!WindowShouldClose()) {		
		// draw current state
		BeginDrawing();

		{
			ClearBackground(BLACK);
			render_state(state);
		}

		EndDrawing();

		// update state
		get_next(state);
	}
	CloseWindow();
	return 0;
}