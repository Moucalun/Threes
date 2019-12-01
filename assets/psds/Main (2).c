#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Inicializa as funções do Allegro
void init() {
	srand((int)time(NULL));

	al_init();
	al_init_image_addon();
	al_init_acodec_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	al_install_mouse();
	al_install_keyboard();
	al_install_audio();
}
// Retorna um valor aleatório de min até max
int randomizer(int min, int max) {
	int a;
	a = rand() % (max + 1 - min) + min;
	return a;
}
// Retorna o valor da maior peça do jogo
int higher(int jogo[][7]) {
	int higher = 0;
	for (int i = 1; i < 6; ++i) {
		for (int j = 1; j < 6; j++) {
			if (jogo[i][j] > higher) {
				higher = jogo[i][j];
			}
		}
	}
	return higher;
}
// Retorna um valor maior que 6 para next()
int rnext(int h) {
	int mat[7][10] = { {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
					  {6, 6, 6, 6, 6, 6, 6, 6, 12, 12},
					  {6, 6, 6, 6, 6, 12, 12, 12, 12, 12},
					  {6, 6, 6, 6, 12, 12, 12, 12, 24, 24},
					  {6, 6, 6, 12, 12, 12, 12, 24, 24, 48},
					  {6, 6, 6, 12, 12, 12, 24, 48, 48, 96},
					  {6, 6, 12, 12, 12, 24, 48, 48, 96, 192} };
	switch (h) {
	case 48:
		return 6;
		break;
	case 96:
		return mat[1][randomizer(0, 9)];
		break;
	case 192:
		return mat[2][randomizer(0, 9)];
		break;
	case 768:
		return mat[3][randomizer(0, 9)];
		break;
	case 1536:
		return mat[4][randomizer(0, 9)];
		break;
	case 3072:
		return mat[5][randomizer(0, 9)];
		break;
	case 6144:
		return mat[6][randomizer(0, 9)];
		break;
	default:
		return 0;
		break;
	}
}
// Retorna um valor aleatório para ser a próxima peça
int next(int jogo[][7]) {
	int h = higher(jogo);
	if (h < 48) {
		return randomizer(1, 3);
	}
	else {
		int r = randomizer(0, 3);
		if (r == 0) {
			int vet[3];
			vet[0] = randomizer(1, 3);
			vet[1] = vet[0];
			vet[2] = rnext(h);
			return vet[randomizer(0, 2)];
		}
		else {
			return r;
		}
	}
}
// Retorna 1 ou 2 caso a soma de A e B possa ser feita
int canSum(int a, int b) {
	// Qualquer um for -1
	if (a == -1 || b == -1 || a == 6144 || b == 6144) {
		return 0;
	}
	// Zero com 0
	else if (a == 0 && b == 0) {
		return 1;
	}
	// Qualquer um com 0
	else if ((a == 0 || b == 0)) {
		return 2;
	}
	// 1 com 2 ou 2 com 1
	else if ((a == 1 && b == 2) || (a == 2 && b == 1)) {
		return 1;
	}
	// Igual com Igual
	else if ((a != -1 && b != -1) && (a != 1 && a != 2 && a == b)) {
		return 1;
	}
	// sla?
	return 0;
}
// Retorna 1 se o jogo ainda pode continuar
int canContinue(int jogo[][7]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (jogo[i][j] == 0) {
				return 1;
			}
		}
	}
	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 6; x++) {
			if (canSum(jogo[x][y], jogo[x - 1][y])) {
				return 1;
			}
		}
	}
	for (int y = 5; y > 0; y--) {
		for (int x = 5; x > 0; x--) {
			if (canSum(jogo[x][y], jogo[x + 1][y])) {
				return 1;
			}
		}
	}
	for (int x = 0; x < 6; x++) {
		for (int y = 5; y > 0; y--) {
			if (canSum(jogo[x][y], jogo[x][y + 1])) {
				return 1;
			}
		}
	}
	for (int x = 6; x > 0; x--) {
		for (int y = 1; y < 6; y++) {
			if (canSum(jogo[x][y], jogo[x][y - 1])) {
				return 1;
			}
		}
	}
	return 0;
}
// Retorna o índice da linha em que o próximo valor aparecerá
int randPosLines(int jogo[][7], int line) {
	int cont = 0, aux, selected;
	for (int i = 1; i < 6; ++i) {
		if (jogo[line][i] == 0) {
			cont++;
			aux = i;
		}
	}
	if (cont == 1) {
		// retorna a única posição que possui 0
		return aux;
	}
	else if (cont > 1) {
		// retorna a posição que possui 0 que foi escolhida aleatóriamente
		do {
			selected = randomizer(1, 5);
		} while (jogo[line][selected] != 0);
		return selected;
	}
	return 0;
}
// Retorna o índice da coluna em que o próximo valor aparecerá
int randPosColumns(int jogo[][7], int column) {
	int cont = 0, aux, selected;
	for (int i = 1; i < 6; ++i) {
		if (jogo[i][column] == 0) {
			cont++;
			aux = i;
		}
	}
	if (cont == 1) {
		// retorna a única posição que possui 0
		return aux;
	}
	else if (cont > 1) {
		// retorna a posição que possui 0 que foi escolhida aleatóriamente
		do {
			selected = randomizer(1, 5);
		} while (jogo[selected][column] != 0);
		return selected;
	}
	return 0;
}
// Faz o movimento das peças, retorna 1 caso houver movimento
int move(int jogo[][7], int a, int b, int* score) {
	int moved = 0, moves = 0, sum, var, tempScore = 0;

	if (a == -1) {
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 6; x++) {
				var = canSum(jogo[x][y], jogo[x - 1][y]);
				if (var == 1 || var == 2) {
					moved = 1;
					sum = jogo[x][y] + jogo[x - 1][y];
					if (var == 1) {
						moves++;
						tempScore = tempScore + sum;
					}
					jogo[x - 1][y] = sum;
					jogo[x][y] = 0;
				}
			}
		}
	}
	else if (a == 1) {
		for (int y = 5; y > 0; y--) {
			for (int x = 5; x > 0; x--) {
				var = canSum(jogo[x][y], jogo[x + 1][y]);
				if (var == 1 || var == 2) {
					moved = 1;
					sum = jogo[x][y] + jogo[x + 1][y];
					if (var == 1) {
						moves++;
						tempScore = tempScore + sum;
					}
					jogo[x + 1][y] = sum;
					jogo[x][y] = 0;
				}
			}
		}
	}
	else if (b == 1) {
		for (int x = 0; x < 6; x++) {
			for (int y = 5; y > 0; y--) {
				var = canSum(jogo[x][y], jogo[x][y + 1]);
				if (var == 1 || var == 2) {
					moved = 1;
					sum = jogo[x][y] + jogo[x][y + 1];
					if (var == 1) {
						moves++;
						tempScore = tempScore + sum;
					}
					jogo[x][y + 1] = sum;
					jogo[x][y] = 0;
				}
			}
		}
	}
	else if (b == -1) {
		for (int x = 6; x > 0; x--) {
			for (int y = 1; y < 6; y++) {
				var = canSum(jogo[x][y], jogo[x][y - 1]);
				if (var == 1 || var == 2) {
					moved = 1;
					sum = jogo[x][y] + jogo[x][y - 1];
					if (var == 1) {
						moves++;
						tempScore = tempScore + sum;
					}
					jogo[x][y - 1] = sum;
					jogo[x][y] = 0;
				}
			}
		}
	}

	// Updeiteia o score
	if (moves > 1) {
		tempScore = tempScore * 2;
	}
	*score = *score + tempScore;

	return moved;
}
// Comandos ao mover a matriz para cima
void pressedUp(int jogo[][7], int* nextValue, int* score) {
	printf("\nUp\n");
	int check = move(jogo, -1, 0, &*score);

	if (check) {
		int i = randPosLines(jogo, 5);
		if (i != 0) {
			jogo[5][i] = *nextValue;
			*nextValue = next(jogo);
		}
	}
}
// Comandos ao mover a matriz para baixo
void pressedDown(int jogo[][7], int* nextValue, int* score) {
	printf("\nDown\n");
	int check = move(jogo, 1, 0, &*score);
	if (check) {
		int i = randPosLines(jogo, 1);
		if (i != 0) {
			jogo[1][i] = *nextValue;
			*nextValue = next(jogo);
		}
	}
}
// Comandos ao mover a matriz para esquerda
void pressedLeft(int jogo[][7], int* nextValue, int* score) {
	printf("\nLeft\n");
	int check = move(jogo, 0, -1, &*score);
	if (check) {
		int i = randPosColumns(jogo, 5);
		if (i != 0) {
			jogo[i][5] = *nextValue;
			*nextValue = next(jogo);
		}
	}
}
// Comandos ao mover a matriz para direita
void pressedRight(int jogo[][7], int* nextValue, int* score) {
	printf("\nRight\n");
	int check = move(jogo, 0, 1, &*score);
	if (check) {
		int i = randPosColumns(jogo, 1);
		if (i != 0) {
			jogo[i][1] = *nextValue;
			*nextValue = next(jogo);
		}
	}
}
// Função que imprime no console a matriz, o score, e o próximo valor a ser
// inserido
void printm(int jogo[][7], int score, int nextValue) {
	int i, j;
	for (i = 0; i < 7; ++i) {
		for (j = 0; j < 7; j++) {
			if (jogo[i][j] == -1) {
				printf(" X ");
			}
			else {
				printf(" %d ", jogo[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("Score: %d\n\n", score);
	printf("Next:  %d\n", nextValue);
}
// Função que cria uma nova matriz
void newBoard(int jogo[][7], int* score, int* nextValue) {
	int i, j, x, y;
	for (i = 0; i < 6; ++i) {
		for (j = 0; j < 6; j++) {
			jogo[i][j] = 0;
		}
	}
	for (i = 0; i < 7; ++i) {
		jogo[0][i] = -1;
		jogo[i][0] = -1;
		jogo[6][i] = -1;
		jogo[i][6] = -1;
	}
	jogo[randomizer(1, 5)][randomizer(1, 5)] = -1;
	i = 1;
	do {
		x = randomizer(1, 5);
		y = randomizer(1, 5);
		if (jogo[x][y] == 0) {
			jogo[x][y] = randomizer(1, 3);
			i++;
		}
	} while (i <= 12);
	printf("\nMatriz Inicial: \n");
	*nextValue = next(jogo);
}
// Função que recebe o jogador
void getPlayer(char* p_player) {
	printf("Insira seu nome: \n");
	fgets(p_player, 11, stdin);
	printf("Bora pro jogo, %s", p_player);
}
// Função que retorna uma allegro color
ALLEGRO_COLOR
color(char* color) {
	if (color == "Black") {
		return al_map_rgb(0, 0, 0);
	}
	else if (color == "Cream White") {
		return al_map_rgb(245, 240, 227);
	}
	else if (color == "Light Blue") {
		return al_map_rgb(64, 191, 193);
	}
	else {
		return al_map_rgb(255, 255, 255);
	}
}
// Função que imprime no jogo o score
void prints(int score) {
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);

	al_draw_filled_rectangle(84, 53, 288, 74, color("Light Blue"));
	al_draw_textf(font, color("Cream White"), 183.0, 55.0, ALLEGRO_ALIGN_CENTRE,
		"%d", score);
	al_flip_display();
	al_destroy_font(font);
}
// Função que imprime no jogo o tempo
void printt(int* gameTime) {
	(*gameTime)++;
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);
	int sec = *gameTime % 60;
	int min = *gameTime / 60;
	int hrs = *gameTime / 3600;

	printf("Tempo de jogo:\n%02d:%02d:%02d\n", hrs, min, sec);

	al_draw_filled_rectangle(84, 79, 288, 101, color("Light Blue"));
	al_draw_textf(font, color("Cream White"), 183.0, 82, ALLEGRO_ALIGN_CENTRE,
		"%02d:%02d:%02d", hrs, min, sec);
	al_flip_display();
	al_destroy_font(font);
}
// Função que sei lá
void gameover() { printf("\ncabou\n"); }
// Função que começa um novo jogo
void newGame(ALLEGRO_DISPLAY* gameWindow, ALLEGRO_BITMAP* mainMenu) {
	int jogo[7][7], score = 0, nextValue, exitGame = 0, gameTime = 0;
	// char player[11];
	ALLEGRO_BITMAP* inGame = al_load_bitmap("assets/bmp/inGame.bmp");
	ALLEGRO_TIMER* gameTimer = al_create_timer(1.0);

	// getPlayer(player);
	newBoard(jogo, &score, &nextValue);

	al_draw_bitmap(inGame, 0, 0, 0);
	prints(score);

	printm(jogo, score, nextValue);

	ALLEGRO_EVENT_QUEUE* gameQueue = al_create_event_queue();
	al_register_event_source(gameQueue, al_get_display_event_source(gameWindow));
	al_register_event_source(gameQueue, al_get_keyboard_event_source());
	al_register_event_source(gameQueue, al_get_mouse_event_source());
	al_register_event_source(gameQueue, al_get_timer_event_source(gameTimer));
	al_start_timer(gameTimer);

	while (!exitGame) {
		ALLEGRO_EVENT gameEvent;
		al_wait_for_event(gameQueue, &gameEvent);

		if (gameEvent.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (gameEvent.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				pressedUp(jogo, &nextValue, &score);
				printm(jogo, score, nextValue);
				prints(score);
				if (!canContinue(jogo)) gameover();
				break;
			case ALLEGRO_KEY_DOWN:
				pressedDown(jogo, &nextValue, &score);
				printm(jogo, score, nextValue);
				prints(score);
				if (!canContinue(jogo)) gameover();
				break;
			case ALLEGRO_KEY_LEFT:
				pressedLeft(jogo, &nextValue, &score);
				printm(jogo, score, nextValue);
				prints(score);
				if (!canContinue(jogo)) gameover();
				break;
			case ALLEGRO_KEY_RIGHT:
				pressedRight(jogo, &nextValue, &score);
				printm(jogo, score, nextValue);
				prints(score);
				if (!canContinue(jogo)) gameover();
				break;
			}
		}

		if (gameEvent.type == ALLEGRO_EVENT_TIMER) {
			printt(&gameTime);
		}
	}
}
// Função principal do jogo
void allegro_main() {
	int exit = 0;
	ALLEGRO_DISPLAY* gameWindow = al_create_display(350, 450);
	ALLEGRO_BITMAP* gameIcon = al_load_bitmap("assets/bmp/gameIcon.bmp");
	ALLEGRO_BITMAP* mainMenu = al_load_bitmap("assets/bmp/mainMenu.bmp");
	ALLEGRO_EVENT_QUEUE* mainQueue = al_create_event_queue();

	al_set_window_title(gameWindow, "Threes");
	al_set_display_icon(gameWindow, gameIcon);

	al_draw_bitmap(mainMenu, 0, 0, 0);
	al_flip_display();

	al_register_event_source(mainQueue, al_get_display_event_source(gameWindow));
	al_register_event_source(mainQueue, al_get_keyboard_event_source());
	al_register_event_source(mainQueue, al_get_mouse_event_source());

	while (!exit) {
		ALLEGRO_EVENT mainEvent;
		al_wait_for_event(mainQueue, &mainEvent);

		if (mainEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (mainEvent.mouse.x >= 44.0 && mainEvent.mouse.x <= 282) {
				if (mainEvent.mouse.y >= 118 && mainEvent.mouse.y <= 231) {
					newGame(gameWindow, mainMenu);
				}
			}
		}
	}
}
// Função principal
void main() {
	init();
	//eae
	allegro_main();
}
