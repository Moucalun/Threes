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
#include <ctype.h>

typedef struct {
	char player[10];
	int score;
} sb;

void init() {
	srand((int)time(NULL));

	al_init();
	al_init_image_addon();
	al_init_acodec_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_acodec_addon();

	al_install_mouse();
	al_install_keyboard();
	al_install_audio();
}
void exitGame() {
	exit(1);
}

ALLEGRO_COLOR color(char* color) {
	if (color == "Black") {
		return al_map_rgb(0, 0, 0);
	}
	else if (color == "Cream White") {
		return al_map_rgb(245, 240, 227);
	}
	else if (color == "Light Blue") {
		return al_map_rgb(64, 191, 193);
	}
	else if (color == "Red") {
		return al_map_rgb(240, 19, 77);
	}
	else if (color == "Pink") {
		return al_map_rgb(210, 80, 114);
	}
	else if (color == "Gray") {
		return al_map_rgb(182, 178, 168);
	}
	else {
		return al_map_rgb(255, 255, 255);
	}
}

int randomizer(int min, int max) {
	int a;
	a = rand() % (max + 1 - min) + min;
	return a;
}

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

int canSum(int a, int b) {
	if (a == -1 || b == -1 || a == 6144 || b == 6144) {
		return 0;
	}
	else if (a == 0 && b == 0) {
		return 2;
	}
	else if ((a == 0 || b == 0)) {
		return 2;
	}
	else if ((a == 1 && b == 2) || (a == 2 && b == 1)) {
		return 1;
	}
	else if ((a != -1 && b != -1) && (a != 1 && a != 2 && a == b)) {
		return 1;
	}
	return 0;
}
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

int randPosLines(int jogo[][7], int line) {
	int cont = 0, aux, selected;
	for (int i = 1; i < 6; ++i) {
		if (jogo[line][i] == 0) {
			cont++;
			aux = i;
		}
	}
	if (cont == 1) {
		return aux;
	}
	else if (cont > 1) {
		do {
			selected = randomizer(1, 5);
		} while (jogo[line][selected] != 0);
		return selected;
	}
	return 0;
}
int randPosColumns(int jogo[][7], int column) {
	int cont = 0, aux, selected;
	for (int i = 1; i < 6; ++i) {
		if (jogo[i][column] == 0) {
			cont++;
			aux = i;
		}
	}
	if (cont == 1) {
		return aux;
	}
	else if (cont > 1) {
		do {
			selected = randomizer(1, 5);
		} while (jogo[selected][column] != 0);
		return selected;
	}
	return 0;
}

int move(int jogo[][7], int a, int b, int* score) {
	int moved = 0, moves = 0, sum, var, tempScore = 0;
	ALLEGRO_SAMPLE* sdX2 = al_load_sample("assets/ogg/soundX2.ogg");

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

	if (moves > 1) {
		tempScore = tempScore * 2;
		al_play_sample(sdX2, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
	*score = *score + tempScore;

	return moved;
}
void pressedUp(int jogo[][7], int* nextValue, int* score, ALLEGRO_SAMPLE* sdMove, ALLEGRO_SAMPLE* sdCantMove) {
	int check = move(jogo, -1, 0, &*score);

	if (check) {
		int i = randPosLines(jogo, 5);
		if (i != 0) {
			jogo[5][i] = *nextValue;
			*nextValue = next(jogo);
			al_play_sample(sdMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		else {
			al_play_sample(sdCantMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}
void pressedDown(int jogo[][7], int* nextValue, int* score, ALLEGRO_SAMPLE* sdMove, ALLEGRO_SAMPLE* sdCantMove) {
	int check = move(jogo, 1, 0, &*score);
	if (check) {
		int i = randPosLines(jogo, 1);
		if (i != 0) {
			jogo[1][i] = *nextValue;
			*nextValue = next(jogo);
			al_play_sample(sdMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		else {
			al_play_sample(sdCantMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}
void pressedLeft(int jogo[][7], int* nextValue, int* score, ALLEGRO_SAMPLE* sdMove, ALLEGRO_SAMPLE* sdCantMove) {
	int check = move(jogo, 0, -1, &*score);
	if (check) {
		int i = randPosColumns(jogo, 5);
		if (i != 0) {
			jogo[i][5] = *nextValue;
			*nextValue = next(jogo);
			al_play_sample(sdMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		else {
			al_play_sample(sdCantMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}
void pressedRight(int jogo[][7], int* nextValue, int* score, ALLEGRO_SAMPLE* sdMove, ALLEGRO_SAMPLE* sdCantMove) {
	int check = move(jogo, 0, 1, &*score);
	if (check) {
		int i = randPosColumns(jogo, 1);
		if (i != 0) {
			jogo[i][1] = *nextValue;
			*nextValue = next(jogo);
			al_play_sample(sdMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		else {
			al_play_sample(sdCantMove, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}

void printm(int jogo[][7], int score, int nextValue) {
	int i, j;
	for (i = 0; i < 7; ++i) {
		for (j = 0; j < 7; ++j) {
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
void allegro_printm(int jogo[][7], int nextValue) {
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 35, 0);
	ALLEGRO_FONT* font2 = al_load_font("assets/ttf/Arcade_Alternate.ttf", 25, 0);
	ALLEGRO_FONT* font3 = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);
	ALLEGRO_FONT* font4 = al_load_font("assets/ttf/Arcade_Alternate.ttf", 14, 0);
	ALLEGRO_BITMAP* one = al_load_bitmap("assets/bmp/one.bmp");
	ALLEGRO_BITMAP* two = al_load_bitmap("assets/bmp/two.bmp");
	ALLEGRO_BITMAP* threes = al_load_bitmap("assets/bmp/threes.bmp");
	ALLEGRO_BITMAP* barrier = al_load_bitmap("assets/bmp/barrier.bmp");
	ALLEGRO_BITMAP* matrix = al_load_bitmap("assets/bmp/matrix.bmp");

	float xI = 37.0;
	float yI = 131.0;
	al_draw_bitmap(matrix, xI, yI, 0);
	for (int i = 1; i < 6; ++i) {
		for (int j = 1; j < 6; ++j) {
			if (jogo[i][j] != 0) {
				if (jogo[i][j] == 1) {
					float x = (xI + (50.0 * j) - 50.0);
					float y = (yI + (50.0 * i) - 50.0);
					al_draw_bitmap(one, x, y, 0);
					al_draw_text(font, color("Cream White"), x + 27, y + 10,
						ALLEGRO_ALIGN_CENTRE, "1");
				}
				else if (jogo[i][j] == 2) {
					float x = (xI + (50.0 * j) - 50.0);
					float y = (yI + (50.0 * i) - 50.0);
					al_draw_bitmap(two, x, y, 0);
					al_draw_text(font, color("Cream White"), x + 27, y + 10,
						ALLEGRO_ALIGN_CENTRE, "2");
				}
				else if (jogo[i][j] == -1) {
					float x = (xI + (50.0 * j) - 50.0);
					float y = (yI + (50.0 * i) - 50.0);
					al_draw_bitmap(barrier, x, y, 0);
				}
				else if (jogo[i][j] != 0) {
					float x = (xI + (50.0 * j) - 50.0);
					float y = (yI + (50.0 * i) - 50.0);
					al_draw_bitmap(threes, x, y, 0);
					if (jogo[i][j] <= 6) {
						al_draw_textf(font, color("Black"), x + 27, y + 10,
							ALLEGRO_ALIGN_CENTRE, "%d", jogo[i][j]);
					}
					else if (jogo[i][j] <= 96) {
						al_draw_textf(font2, color("Black"), x + 27, y + 15,
							ALLEGRO_ALIGN_CENTRE, "%d", jogo[i][j]);
					}
					else if (jogo[i][j] <= 768) {
						al_draw_textf(font3, color("Black"), x + 27, y + 18,
							ALLEGRO_ALIGN_CENTRE, "%d", jogo[i][j]);
					}
					else if (jogo[i][j] == 6144 || jogo[i][j] == 3072) {
						al_draw_textf(font4, color("Red"), x + 27, y + 21,
							ALLEGRO_ALIGN_CENTRE, "%d", jogo[i][j]);
					}
					else {
						al_draw_textf(font4, color("Black"), x + 27, y + 10,
							ALLEGRO_ALIGN_CENTRE, "%d", jogo[i][j]);
					}
				}
			}
		}
	}
	if (nextValue != 0) {
		if (nextValue == 1) {
			al_draw_bitmap(one, 295.0, 10.0, 0);
			al_draw_text(font, color("Cream White"), 295 + 27, 10 + 10,
				ALLEGRO_ALIGN_CENTRE, "1");
		}
		else if (nextValue == 2) {
			al_draw_bitmap(two, 295.0, 10.0, 0);
			al_draw_text(font, color("Cream White"), 295 + 27, 10 + 10,
				ALLEGRO_ALIGN_CENTRE, "2");
		}
		else if (nextValue == 3 || nextValue == 6) {
			al_draw_bitmap(threes, 295.0, 10.0, 0);
			al_draw_textf(font, color("Black"), 295 + 27, 10 + 10,
				ALLEGRO_ALIGN_CENTRE, "%d", nextValue);
		}
		else if (nextValue >= 12 && nextValue <= 96) {
			al_draw_bitmap(threes, 295.0, 10.0, 0);
			al_draw_textf(font2, color("Black"), 295 + 27, 295 + 15,
				ALLEGRO_ALIGN_CENTRE, "%d", nextValue);
		}
		else {
			al_draw_bitmap(threes, 295.0, 10.0, 0);
			al_draw_text(font3, color("Black"), 295 + 27, 295 + 18,
				ALLEGRO_ALIGN_CENTRE, "192");
		}
	}

	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_font(font3);
	al_destroy_font(font4);

	al_destroy_bitmap(one);
	al_destroy_bitmap(two);
	al_destroy_bitmap(threes);
	al_destroy_bitmap(barrier);
	al_destroy_bitmap(matrix);

	al_flip_display();
}
void prints(int score) {
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);

	al_draw_filled_rectangle(84, 53, 273, 74, color("Light Blue"));
	al_draw_textf(font, color("Cream White"), 183.0, 55.0, ALLEGRO_ALIGN_CENTRE,
		"%d", score);
	al_flip_display();
	al_destroy_font(font);
}
void printt(int* gameTime) {
	(*gameTime)++;
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);
	int sec = *gameTime % 60;
	int min = *gameTime / 60;
	int hrs = *gameTime / 3600;

	al_draw_filled_rectangle(84, 79, 273, 101, color("Light Blue"));
	al_draw_textf(font, color("Cream White"), 183.0, 82, ALLEGRO_ALIGN_CENTRE,
		"%02d:%02d:%02d", hrs, min, sec);
	al_flip_display();
	al_destroy_font(font);
}

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
	*nextValue = next(jogo);
}
void nameSelect(char player[11]) {
	int nameSelected = 0, i = 0;
	ALLEGRO_BITMAP* nameScreen = al_load_bitmap("assets/bmp/nameScreen.bmp");
	ALLEGRO_EVENT_QUEUE* nameQueue = al_create_event_queue();
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 35, 0);
	ALLEGRO_SAMPLE* sdChar = al_load_sample("assets/ogg/soundChar.ogg");
	ALLEGRO_SAMPLE* sdBackspace = al_load_sample("assets/ogg/soundBackspace.ogg");
	ALLEGRO_SAMPLE* sdError = al_load_sample("assets/ogg/soundError.ogg");
	ALLEGRO_SAMPLE* sdOk = al_load_sample("assets/ogg/soundNewGame.ogg");

	al_register_event_source(nameQueue, al_get_keyboard_event_source());
	al_register_event_source(nameQueue, al_get_display_event_source(al_get_current_display()));

	al_draw_bitmap(nameScreen, 0, 0, 0);
	al_flip_display();

	while (!nameSelected) {
		ALLEGRO_EVENT nameEvent;
		al_wait_for_event(nameQueue, &nameEvent);

		if (nameEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exitGame();
		}

		if (nameEvent.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (nameEvent.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				if (i != 0) {
					nameSelected = 1;
					al_play_sample(sdOk, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					al_play_sample(sdError, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			case ALLEGRO_KEY_BACKSPACE:
				if (i != 0) {
					al_play_sample(sdBackspace, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					player[i - 1] = '\0';
					i--;
				}
				else {
					al_play_sample(sdError, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			default:
				if (i < 10) {
					if ((nameEvent.keyboard.unichar >= 65 &&
						nameEvent.keyboard.unichar <= 90) ||
						(nameEvent.keyboard.unichar >= 97 &&
						nameEvent.keyboard.unichar <= 122)) {
						al_play_sample(sdChar, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						player[i] = nameEvent.keyboard.unichar;
						player[i + 1] = '\0';
						i++;
					}
					else {
						al_play_sample(sdError, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				else {
					al_play_sample(sdError, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			}

			al_draw_filled_rectangle(60, 155, 278, 251, color("Red"));
			al_draw_textf(font, color("Cream White"), 169.0, 203.0, ALLEGRO_ALIGN_CENTRE, "%s", player);
			al_flip_display();
		}
	}

	al_destroy_bitmap(nameScreen);
	al_destroy_event_queue(nameQueue);
	al_destroy_font(font);
	al_rest(1.0);
}

void createFile() {
	FILE* fileHighscores = fopen("Pontuacoes.txt", "w");
	int i;
	sb scoreboard[10];

	for (i = 0; i < 10; ++i) {
		scoreboard[i].player[0] = '!';
		scoreboard[i].player[1] = '\0';
		scoreboard[i].score = 0;
		fprintf(fileHighscores, "%s %d\n", scoreboard[i].player,
			scoreboard[i].score);
	}
	fclose(fileHighscores);
}
int fileCheck() {
	FILE* fileHighscores = fopen("Pontuacoes.txt", "r");
	if (fileHighscores == NULL) {
		createFile();
	}

	if (fileHighscores != NULL) {
		fclose(fileHighscores);
		return 1;
	}
	else {
		return 0;
	}
}

void gameoverScreen(int placed, sb scoreboard[10]) {
	ALLEGRO_BITMAP* highscoresScreen = al_load_bitmap("assets/bmp/highscoresMenu.bmp");
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);
	ALLEGRO_FONT* font2 = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);
	ALLEGRO_EVENT_QUEUE* gameoverQueue = al_create_event_queue();
	float x = 70;
	float y = 110;
	int quit = 0;

	al_draw_bitmap(highscoresScreen, 0, 0, 0);
	for (int i = 0; i < 10; ++i, y = y + 30) {
		if (i == placed) {
			al_draw_textf(font, color("Red"), x, y, 0, "%02d - %s : %d", i + 1, scoreboard[i].player, scoreboard[i].score);
		}
		else {
			al_draw_textf(font, color("Black"), x, y, 0, "%02d - %s : %d", i + 1, scoreboard[i].player, scoreboard[i].score);
		}
	}

	al_flip_display();

	al_register_event_source(gameoverQueue, al_get_mouse_event_source());
	al_register_event_source(gameoverQueue, al_get_display_event_source(al_get_current_display()));
	
	while (!quit) {
		ALLEGRO_EVENT gameoverEvent;
		al_wait_for_event(gameoverQueue, &gameoverEvent);

		if (gameoverEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (gameoverEvent.mouse.x >= 258.0 && gameoverEvent.mouse.x <= 303.0) {
				if (gameoverEvent.mouse.y >= 51.0 && gameoverEvent.mouse.y <= 93.0) {
					quit = 1;
				}
			}
		}
		if (gameoverEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exitGame();
		}
	}
}
void scoreboardSorter(char player[11], int score) {
	int i, j, quit = 0, placed = 11;
	FILE* fileHighscores = fopen("Pontuacoes.txt", "r+");
	sb scoreboard[10];
	sb sbAux, sbAuxInit;

	for (i = 0; i < 10; ++i) {
		fscanf(fileHighscores, "%s %d\n", scoreboard[i].player,
			&(scoreboard[i].score));
	}

	i = 0;
	while (!quit) {
		if (i > 9) {
			quit = 1;
		}
		else if ((score >= scoreboard[i].score) && (quit == 0)) {
			placed = i;
			if (score > scoreboard[i].score) {
				sbAuxInit = scoreboard[i + 1];
				scoreboard[i + 1] = scoreboard[i];

				for (j = i + 2; j < 10; j++) {
					sbAux = scoreboard[j];
					scoreboard[j] = sbAuxInit;
					sbAuxInit = sbAux;
				}
				scoreboard[i].score = score;
				strcpy(scoreboard[i].player, player);
				quit = 1;
			}
			else if (score == scoreboard[i].score) {
				sbAuxInit = scoreboard[i + 2];
				scoreboard[i + 2] = scoreboard[i];

				for (j = i + 3; j < 10; j++) {
					sbAux = scoreboard[j];
					scoreboard[j] = sbAuxInit;
					sbAuxInit = sbAux;
				}
				scoreboard[i].score = score;
				strcpy(scoreboard[i + 1].player, player);
				quit = 1;
			}
		}
		i++;
	}

	fclose(fileHighscores);
	fileHighscores = fopen("Pontuacoes.txt", "w");

	for (i = 0; i < 10; ++i) {
		fprintf(fileHighscores, "%s %d\n", scoreboard[i].player, scoreboard[i].score);
	}

	fclose(fileHighscores);

	gameoverScreen(placed, scoreboard);
}
void gameOver(char player[11], int score) {
	fileCheck();
	scoreboardSorter(player, score);
}

void pauseGame(int jogo[][7], int nextValue, ALLEGRO_FONT* nameFont, int* gameTime, int score, char player[11], ALLEGRO_BITMAP* inGame, int* gameOption) {
	int resume = 0;
	ALLEGRO_BITMAP* pauseScreen = al_load_bitmap("assets/bmp/pauseScreen.bmp");
	al_convert_mask_to_alpha(pauseScreen, color("Gray"));
	ALLEGRO_EVENT_QUEUE* pauseQueue = al_create_event_queue();
	al_register_event_source(pauseQueue, al_get_keyboard_event_source());
	al_register_event_source(pauseQueue, al_get_mouse_event_source());
	al_register_event_source(pauseQueue, al_get_display_event_source(al_get_current_display()));

	al_draw_bitmap(pauseScreen, 0, 0, 0);
	al_flip_display();
	while (!resume) {
		ALLEGRO_EVENT pauseEvent;
		al_wait_for_event(pauseQueue, &pauseEvent);
		if (pauseEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {

			if (pauseEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				exitGame();
			}

			if (pauseEvent.mouse.x >= 129 && pauseEvent.mouse.x <= 198) {
				if (pauseEvent.mouse.y >= 204 && pauseEvent.mouse.y <= 273) {
					resume = 1;
					al_draw_bitmap(inGame, 0, 0, 0);
					al_draw_textf(nameFont, color("Pink"), 103, 428, 0, "%s", player);
					printt(gameTime);
					prints(score);
					allegro_printm(jogo, nextValue);
					al_flip_display();
				}
			}
			else if (pauseEvent.mouse.x >= 42 && pauseEvent.mouse.x <= 111) {
				if (pauseEvent.mouse.y >= 204 && pauseEvent.mouse.y <= 273) {
					resume = 1;
					*gameOption = 1;
				}
			}
			else if (pauseEvent.mouse.x >= 215 && pauseEvent.mouse.x <= 284) {
				if (pauseEvent.mouse.y >= 204 && pauseEvent.mouse.y <= 273) {
					resume = 1;
					*gameOption = 2;
				}
			}
		}
	}
	al_destroy_bitmap(pauseScreen);
	al_destroy_event_queue(pauseQueue);
}
void newGame(ALLEGRO_DISPLAY* gameWindow, int* isRestarting) {
	int jogo[7][7], score = 0, nextValue, exitGame = 0, gameTime = 0, timeStoppedAt, gameOption = 0, outOfMoves = 0;
	char player[11];
	ALLEGRO_BITMAP* inGame = al_load_bitmap("assets/bmp/inGame.bmp");
	ALLEGRO_TIMER* gameTimer = al_create_timer(1.0);
	ALLEGRO_FONT* nameFont = al_load_font("assets/ttf/Arcade_Classic.ttf", 18, 0);
	ALLEGRO_FONT* outOfMovesFont = al_load_font("assets/ttf/Arcade_Classic.ttf", 20, 0);
	ALLEGRO_SAMPLE* sdHovering = al_load_sample("assets/ogg/soundHovering.ogg");
	ALLEGRO_SAMPLE* sdPauseIn = al_load_sample("assets/ogg/soundPauseIn.ogg");
	ALLEGRO_SAMPLE* sdPauseOut = al_load_sample("assets/ogg/soundPauseOut.ogg");
	ALLEGRO_SAMPLE* sdGameOver = al_load_sample("assets/ogg/soundGameOver.ogg");
	ALLEGRO_SAMPLE* sdMove = al_load_sample("assets/ogg/soundMove.ogg");
	ALLEGRO_SAMPLE* sdCantMove = al_load_sample("assets/ogg/soundCantMove.ogg");

	nameSelect(player);

	newBoard(jogo, &score, &nextValue);

	al_draw_bitmap(inGame, 0, 0, 0);
	al_draw_textf(nameFont, color("Pink"), 103, 428, 0, "%s", player);

	printt(&gameTime);
	prints(score);

	allegro_printm(jogo, nextValue);

	ALLEGRO_EVENT_QUEUE* gameQueue = al_create_event_queue();
	al_register_event_source(gameQueue, al_get_display_event_source(gameWindow));
	al_register_event_source(gameQueue, al_get_keyboard_event_source());
	al_register_event_source(gameQueue, al_get_mouse_event_source());
	al_register_event_source(gameQueue, al_get_timer_event_source(gameTimer));
	al_start_timer(gameTimer);
	int x = 0, y = 0;
	while (!exitGame) {
		ALLEGRO_EVENT gameEvent;
		al_wait_for_event(gameQueue, &gameEvent);

		if (gameEvent.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (gameEvent.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				pressedUp(jogo, &nextValue, &score, sdMove, sdCantMove);
				allegro_printm(jogo, nextValue);
				prints(score);
				if (!canContinue(jogo)) {
					exitGame = 1;
					timeStoppedAt = gameTime;
					outOfMoves = 1;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				pressedDown(jogo, &nextValue, &score, sdMove, sdCantMove);
				allegro_printm(jogo, nextValue);
				prints(score);
				if (!canContinue(jogo)) {
					exitGame = 1;
					timeStoppedAt = gameTime;
					outOfMoves = 1;
				}
				break;
			case ALLEGRO_KEY_LEFT:
				pressedLeft(jogo, &nextValue, &score, sdMove, sdCantMove);
				allegro_printm(jogo, nextValue);
				prints(score);
				if (!canContinue(jogo)) {
					exitGame = 1;
					timeStoppedAt = gameTime;
					outOfMoves = 1;
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				pressedRight(jogo, &nextValue, &score, sdMove, sdCantMove);
				allegro_printm(jogo, nextValue);
				prints(score);
				if (!canContinue(jogo)) {
					exitGame = 1;
					timeStoppedAt = gameTime;
					outOfMoves = 1;
				}
				break;
			}
		}

		if (gameEvent.type == ALLEGRO_EVENT_MOUSE_AXES) {
			x = gameEvent.mouse.x;
			y = gameEvent.mouse.y;
		}

		if (gameEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			int deltax = gameEvent.mouse.x - x;
			int deltay = gameEvent.mouse.y - y;
			if (deltax >= deltay){
				if (gameEvent.mouse.x >= x) {
					pressedRight(jogo, &nextValue, &score, sdMove, sdCantMove);
					allegro_printm(jogo, nextValue);
					prints(score);
					if (!canContinue(jogo)) {
						exitGame = 1;
						timeStoppedAt = gameTime;
						outOfMoves = 1;
					}
				}
				if (gameEvent.mouse.x < x) {
					pressedLeft(jogo, &nextValue, &score, sdMove, sdCantMove);
					allegro_printm(jogo, nextValue);
					prints(score);
					if (!canContinue(jogo)) {
						exitGame = 1;
						timeStoppedAt = gameTime;
						outOfMoves = 1;
					}
				}
			}
			if(deltay > deltax) {
				if (gameEvent.mouse.y >= y) {
					pressedDown(jogo, &nextValue, &score, sdMove, sdCantMove);
					allegro_printm(jogo, nextValue);
					prints(score);
					if (!canContinue(jogo)) {
						exitGame = 1;
						timeStoppedAt = gameTime;
						outOfMoves = 1;
					}
				}
				if (gameEvent.mouse.y < y) {
					pressedUp(jogo, &nextValue, &score, sdMove, sdCantMove);
					allegro_printm(jogo, nextValue);
					prints(score);
					if (!canContinue(jogo)) {
						exitGame = 1;
						timeStoppedAt = gameTime;
						outOfMoves = 1;
					}
				}
			}
		}

		if (gameEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exit(1);
		}

		if (gameEvent.type == ALLEGRO_EVENT_TIMER) {
			printt(&gameTime);
		}

	}

	if (exitGame && outOfMoves) {
		al_rest(1.0);
		al_play_sample(sdGameOver, 3, 0, 0.5, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_draw_text(outOfMovesFont, color("Red"), 162, 390, ALLEGRO_ALIGN_CENTRE, "Acabaram os movimentos!");
		al_flip_display();
		al_rest(5.0);
		gameOver(player, score);
	}

	al_destroy_bitmap(inGame);
	al_destroy_timer(gameTimer);
	al_destroy_font(nameFont);
	al_destroy_font(outOfMovesFont);
	al_destroy_event_queue(gameQueue);
}

void help(ALLEGRO_DISPLAY* gameWindow) {
	int quit = 0;
	ALLEGRO_BITMAP* helpScreen = al_load_bitmap("assets/bmp/helpScreen.bmp");
	ALLEGRO_EVENT_QUEUE* helpQueue = al_create_event_queue();
	al_draw_bitmap(helpScreen, 0, 0, 0);
	al_flip_display();

	al_register_event_source(helpQueue, al_get_mouse_event_source());
	al_register_event_source(helpQueue, al_get_display_event_source(al_get_current_display()));
	
	while (!quit) {
		ALLEGRO_EVENT helpEvent;
		al_wait_for_event(helpQueue, &helpEvent);

		if (helpEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (helpEvent.mouse.x >= 258.0 && helpEvent.mouse.x <= 303.0) {
				if (helpEvent.mouse.y >= 51.0 && helpEvent.mouse.y <= 93.0) {
					quit = 1;
				}
			}
		}
		if (helpEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exitGame();
		}
	}
}
void hsMenu(ALLEGRO_DISPLAY* gameWindow) {
	int quit = 0;
	float x = 70, y = 110;
	fileCheck();
	FILE* fileHighscores = fopen("Pontuacoes.txt", "r+");
	ALLEGRO_FONT* font = al_load_font("assets/ttf/Arcade_Alternate.ttf", 18, 0);
	ALLEGRO_EVENT_QUEUE* hsEventQueue = al_create_event_queue();
	ALLEGRO_BITMAP* hsScreen = al_load_bitmap("assets/bmp/highscoresMenu.bmp");
	sb scoreboard[10];

	al_draw_bitmap(hsScreen, 0, 0, 0);
	for (int i = 0; i < 10; ++i, y = y + 30) {
		fscanf(fileHighscores, "%s %d\n", scoreboard[i].player, &(scoreboard[i].score));
		al_draw_textf(font, color("Black"), x, y, 0, "%02d - %s : %d", i + 1, scoreboard[i].player, scoreboard[i].score);
	}

	al_flip_display();
	fclose(fileHighscores);

	ALLEGRO_EVENT_QUEUE* hsQueue = al_create_event_queue();

	al_register_event_source(hsEventQueue, al_get_mouse_event_source());
	al_register_event_source(hsEventQueue, al_get_display_event_source(al_get_current_display()));
	
	while (!quit) {
		ALLEGRO_EVENT hsEvent;
		al_wait_for_event(hsEventQueue, &hsEvent);

		if (hsEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (hsEvent.mouse.x >= 258.0 && hsEvent.mouse.x <= 303.0) {
				if (hsEvent.mouse.y >= 51.0 && hsEvent.mouse.y <= 93.0) {
					quit = 1;
				}

			}
		}
		if (hsEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exitGame();
		}
	}
}

void allegro_main() {
	int quit = 0, isRestarting = 0;
	ALLEGRO_DISPLAY* gameWindow = al_create_display(350, 450);
	ALLEGRO_BITMAP* gameIcon = al_load_bitmap("assets/bmp/gameIcon.bmp");
	ALLEGRO_BITMAP* mainMenu = al_load_bitmap("assets/bmp/mainMenu.bmp");
	ALLEGRO_AUDIO_STREAM* bgMusic = al_load_audio_stream("assets/ogg/bgMusic.ogg", 4, 1024);
	ALLEGRO_SAMPLE* sdHovering = al_load_sample("assets/ogg/soundHovering.ogg");
	ALLEGRO_SAMPLE* sdNewGame = al_load_sample("assets/ogg/soundNewGame.ogg");
	ALLEGRO_SAMPLE* sdSelect = al_load_sample("assets/ogg/soundSelect.ogg");
	ALLEGRO_EVENT_QUEUE* mainQueue = al_create_event_queue();

	al_reserve_samples(30);
	al_attach_audio_stream_to_mixer(bgMusic, al_get_default_mixer());
	al_set_audio_stream_playmode(bgMusic, ALLEGRO_PLAYMODE_LOOP);

	al_set_window_title(gameWindow, "Threes");
	al_set_display_icon(gameWindow, gameIcon);

	al_draw_bitmap(mainMenu, 0, 0, 0);
	al_flip_display();

	al_register_event_source(mainQueue, al_get_display_event_source(gameWindow));
	al_register_event_source(mainQueue, al_get_keyboard_event_source());
	al_register_event_source(mainQueue, al_get_mouse_event_source());

	while (!quit) {
		ALLEGRO_EVENT mainEvent;
		al_wait_for_event(mainQueue, &mainEvent);

		if (isRestarting) {
			al_pause_event_queue(mainQueue, 1);
			newGame(gameWindow, &isRestarting);
			if (isRestarting == 0) {
				al_draw_bitmap(mainMenu, 0, 0, 0);
				al_flip_display();
			}
			al_pause_event_queue(mainQueue, 0);
		}

		if (mainEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exitGame();
			quit = 1;
		}

		if (mainEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if ((mainEvent.mouse.x >= 44.0 && mainEvent.mouse.x <= 282) &&
				(mainEvent.mouse.y >= 118 && mainEvent.mouse.y <= 231)) {
				al_play_sample(sdNewGame, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_pause_event_queue(mainQueue, 1);
				isRestarting = 0;
				newGame(gameWindow, &isRestarting);
				if (isRestarting == 0) {
					al_draw_bitmap(mainMenu, 0, 0, 0);
					al_flip_display();
				}
				al_pause_event_queue(mainQueue, 0);
			}
			else if ((mainEvent.mouse.x >= 169.0 && mainEvent.mouse.x <= 281.0) &&
				(mainEvent.mouse.y >= 254.0 && mainEvent.mouse.y <= 366.0)) {
				al_play_sample(sdHovering, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_pause_event_queue(mainQueue, 1);
				help(gameWindow);
				al_play_sample(sdHovering, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_bitmap(mainMenu, 0, 0, 0);
				al_flip_display();
				al_pause_event_queue(mainQueue, 0);
			}
			else if ((mainEvent.mouse.x >= 44.0 && mainEvent.mouse.x <= 157.0) &&
				(mainEvent.mouse.y >= 254.0 && mainEvent.mouse.y <= 366.0)) {
				al_play_sample(sdHovering, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_pause_event_queue(mainQueue, 1);
				hsMenu(gameWindow);
				al_play_sample(sdHovering, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_bitmap(mainMenu, 0, 0, 0);
				al_flip_display();
				al_pause_event_queue(mainQueue, 0);
			}
		}
	}
}
void main() {
	init();
	allegro_main();
}
