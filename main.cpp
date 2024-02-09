#include <stdlib.h>
#include <stdint.h>
#include <time.h> 
#include <math.h>

#include <raylib.h>


#define BOARD_SIZE 1000
#define CELL_SIZE 1

#define ROW BOARD_SIZE / CELL_SIZE
#define COL BOARD_SIZE / CELL_SIZE

#define SHBLACK {31,31,31,255}

// smoothlife parameters
int r_a = 21;
int r_i = r_a / 3;
float b_1 = 0.278;
float b_2 = 0.365;
float d_1 = 0.267;
float d_2 = 0.445;
float alpha_n = 0.028;
float alpha_m = 0.147;


struct State{
	float current_cells[ROW][COL];
	float next_cells[ROW][COL];
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
			state.current_cells[i][j] = (float)rand() / (float)RAND_MAX;
		}
	}
}


float integral_of(State& state, int i, int j, int radius){
	int sum = 0;
	int ii, jj;
	for(ii=max(i-radius, 0); ii<=min(i+radius, ROW-1); ii++){
		for(jj=max(j-radius, 0); jj<=min(j+radius, COL-1); jj++){
			if((i-ii)^2+(j-jj)^2 <= r_i^2){
				sum += state.current_cells[i][j];
			}
		}
	}
	return sum;
}


float sigma_1(float x, float a, float alpha){
	return 1.0/(1.0+(float)exp(-(x-a)*4/alpha));
}


float sigma_2(float x, float a, float b, float alpha){
	return sigma_1(x, a, alpha)*(1.0-sigma_1(x, b, alpha));
}


float sigma_m(float x, float y, float m, float alpha){
	float sigma_1_o;
	sigma_1_o = sigma_1(m, 0.5, alpha);
	return x*(1.0-sigma_1_o)+y*sigma_1_o;
}


float s(float n, float m, float b1, float b2, float d1, float d2, float alpha){
	// σ2(n, σm(b1, d1, m), σm(b2, d2, m))
	float sigma_m_o_1, sigma_m_o_2;
	sigma_m_o_1 = sigma_2(b1, d1, m, alpha);
	sigma_m_o_2 = sigma_2(b2, d2, m, alpha);
	return sigma_2(n, sigma_m_o_1, sigma_m_o_2, alpha);
}


void get_next(State& state){
	int i, j;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			// calculate m and n
			float m, n;
			m = integral_of(state, i, j, r_i);
			n = integral_of(state, i, j, r_a) - m;

			// calculate S(n, m)
			// TODO: what is alpha? we have alpha_n and alpha_m
			state.next_cells[i][j] = s(n, m, b_1, b_2, d_1, d_2, alpha_m);
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
				Color color_palate;
				// state.current_cells[i][j]
				if(state.current_cells[i][j]==1.0){
					color_palate = {255, 255, 255, 255};
				} else if(state.current_cells[i][j]==0.0){
					color_palate = {0, 0, 1, 255};
				} else {
					uint8_t hue = (uint8_t)255 - (uint8_t)(state.current_cells[i][j]*255.0);
					color_palate = {hue, hue, hue, 255};
				}
				DrawRectangle(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE, color_palate);
		}
	}
}


int main(){
	srand(time(0));

	State state{};

	InitWindow(BOARD_SIZE, BOARD_SIZE, "Game of Life");
	SetTargetFPS(10);

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