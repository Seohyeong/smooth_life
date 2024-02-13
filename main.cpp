#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h> 
#include <math.h>
#include <assert.h>
#include <raylib.h>


#define BOARD_SIZE 500
#define CELL_SIZE 1

#define ROW BOARD_SIZE / CELL_SIZE
#define COL BOARD_SIZE / CELL_SIZE

// #define ROW 50
// #define COL 100

// smoothlife parameters
int r_a = 6;
int r_i = 2;
float b_1 = 0.278;
float b_2 = 0.365;
float d_1 = 0.267;
float d_2 = 0.445;
float alpha_n = 0.028;
float alpha_m = 0.147;
float d_t = 0.1f;


// less dense to more dense
char level[] = " .-=coa@#";
#define LEVEL_COUNT (sizeof(level)/sizeof(level[0])-1)


// NOTE: 0.0 is double (8bytes, double the float) and 0.9f is float (4bytes)
struct State{
	float current_cells[ROW][COL] = {};
	float next_cells[ROW][COL] = {};
};


void init_cells(State& state){
	for(int i=0; i<ROW/2; i++){
		for(int j=0; j<COL/2; j++){
			state.current_cells[i][j] = (float)rand()/(float)RAND_MAX;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// get integral
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


int mod(int x, int y){
	// returns x % y
	int output = x % y;
	if(x<0){
		return output + y;
	} else {
		return output;
	}
}


// ~O(n^3)
void integral_of(State& state, int i, int j, float array[2]){
	int M = 0;
	int N = 0;
	float m = 0;
	float n = 0;

	for(int ii=i-r_a; ii<=i+r_a; ii++){
		for(int jj=j-r_a; jj<=j+r_a; jj++){
			int d_i_pow = (i-ii)*(i-ii);
			int d_j_pow = (j-jj)*(j-jj);
			if(d_i_pow + d_j_pow <= r_i*r_i){
				m += state.current_cells[mod(ii, ROW)][mod(jj, COL)];
				M += 1;
			} else if(d_i_pow + d_j_pow <= r_a*r_a){
				n += state.current_cells[mod(ii, ROW)][mod(jj, COL)];
				N += 1;
			}
		}
	}
	array[0] = m/M;
	array[1] = n/N;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// get sigma functions
float sigma_1(float x, float a, float alpha){
	return 1.0f / (1.0f + expf(-(x-a)*4/alpha));
}


float sigma_2(float x, float a, float b, float alpha){
	return sigma_1(x, a, alpha)*(1 - sigma_1(x, b, alpha));
}


float sigma_m(float x, float y, float m, float alpha){
	float sigma_1_o = sigma_1(m, 0.5f, alpha);
	return x*(1 - sigma_1_o) + y*sigma_1_o;
}


float s(float n, float m){
	// σ2(n, σm(b1, d1, m), σm(b2, d2, m))
	float sigma_m_o1 = sigma_m(b_1, d_1, m, alpha_m);
	float sigma_m_o2 = sigma_m(b_2, d_2, m, alpha_m);
	return sigma_2(n, sigma_m_o1, sigma_m_o2, alpha_n);
}


void get_next(State& state){
	for(int i=0; i<ROW; i++){
		for(int j=0; j<COL; j++){
			// calculate m and n
			float m_and_n[2];
			integral_of(state, i, j, m_and_n);
			float m = m_and_n[0];
			float n = m_and_n[1];

			// calculate S(n, m)
			// float cell_diff = 2*s(n, m) - 1;

			// TODO: this has to be inbetween 0 and 1
			// state.next_cells[i][j] = state.current_cells[i][j] + d_t * cell_diff;
			state.next_cells[i][j] = s(n, m);
			// if(state.next_cells[i][j] < 0){
			// 	state.next_cells[i][j] = 0.0f;
			// } else if(state.next_cells[i][j] > 1){
			// 	state.next_cells[i][j] = 1.0f;
			// }

			// // discrete time stepping
			// state.next_cells[i][j] = s(n, m);
		}
	}

	// reset the current state
	for(int i=0; i<ROW; i++){
		for(int j=0; j<COL; j++){
			state.current_cells[i][j] = state.next_cells[i][j];
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// render states
void render_state(const State& state){
	int i, j;
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
				Color color_palate;
				uint8_t hue = (uint8_t)(state.current_cells[i][j]*255.0);
				color_palate = {hue, hue, hue, 255};
				DrawRectangle(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE, color_palate);
		}
	}
}


void print_state(float array[ROW][COL]){
	for(int i=0; i<ROW; i++){
		for(int j=0; j<COL; j++){
			printf("%f ", array[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}


void print_state_w_char(float array[ROW][COL]){
	for(int i=0; i<ROW; i++){
		for(int j=0; j<COL; j++){
			char c = level[(int)(array[i][j] * (LEVEL_COUNT-1))];
			printf("%c", c);
		}
		printf("\n");
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// main function
int main(){
	srand(time(0));

	State state{};

	// raylib loop
	InitWindow(BOARD_SIZE, BOARD_SIZE, "SmoothLife");
	SetTargetFPS(60);

	init_cells(state);

	while (!WindowShouldClose()) {	

		BeginDrawing();
		{
			ClearBackground(BLACK);
			render_state(state);
		}
		EndDrawing();

		get_next(state);
	}
	CloseWindow();


	// // just print
	// init_cells(state);
	// print_state_w_char(state.current_cells);

	// int count = 0;
	// while(count < 1000){
	// 	get_next(state);
	// 	print_state_w_char(state.current_cells);
	// 	count += 1;
	// }

	return 0;
}
