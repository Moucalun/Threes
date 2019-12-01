#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//Funções de inicialização-----------------------------------
void preInit() {
	srand((int)time(NULL));
	al_init();
	al_init_image_addon();
	al_init_acodec_addon();
	
	al_install_mouse();
	al_install_keyboard();
	al_install_audio();
}
//-----------------------------------------------------------

//DECLARAÇÕES GLOBAIS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ALLEGRO_DISPLAY* gameWindow;
ALLEGRO_BITMAP* gameIcon;
ALLEGRO_BITMAP* background;
ALLEGRO_BITMAP* titleScreen;
ALLEGRO_BITMAP* mainMenu;
ALLEGRO_BITMAP* newGame;
ALLEGRO_BITMAP* highscores;
ALLEGRO_BITMAP* help;
ALLEGRO_BITMAP* newGameSelected;
ALLEGRO_BITMAP* highscoresSelected;
ALLEGRO_BITMAP* helpSelected;
ALLEGRO_AUDIO_STREAM* bgMusic;
ALLEGRO_EVENT_QUEUE* queue_titleScreen;
ALLEGRO_EVENT_QUEUE* queue_mainMenu;
FILE* a;
int quit = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Retorna uma ALLEGRO_COLOR da cor recebida como parâmetro
ALLEGRO_COLOR color(char* color) {
	if (color == "Alpha")
		return al_map_rgb(252, 220, 68);
	if (color == "Red")
		return al_map_rgb(240, 19, 77);
	if (color == "Blue")
		return al_map_rgb(64, 191, 193);
	if (color == "Yellow")
		return al_map_rgb(255, 220, 52);
	if (color == "White")
		return al_map_rgb(245, 240, 227);
	if (color == "Alpha_2")
		return al_map_rgb(252, 221, 68);

	return al_map_rgb(255, 255, 255);
}
void init() {
	//---------------------- TITLE SCREEN -----------------------------------------------------------------
	background = al_load_bitmap("assets/bmp/background.bmp");
	titleScreen = al_load_bitmap("assets/bmp/titleScreen.bmp");
	bgMusic = al_load_audio_stream("assets/ogg/bgMusic.ogg", 4, 1024);
	//-------------------------audio-----------------------------------------------------------------------
	al_reserve_samples(1);
	al_convert_mask_to_alpha(titleScreen, color("Alpha"));
	al_attach_audio_stream_to_mixer(bgMusic, al_get_default_mixer());
	al_set_audio_stream_playmode(bgMusic, ALLEGRO_PLAYMODE_LOOP);
	//-----------------------------------------------------------------------------------------------------

	//----------------------- MAIN MENU -------------------------------------------------------------------
	mainMenu = al_load_bitmap("assets/bmp/mainMenu.bmp");
	newGame = al_load_bitmap("assets/bmp/newGame.bmp");
	highscores = al_load_bitmap("assets/bmp/highscores.bmp");
	help = al_load_bitmap("assets/bmp/help.bmp");
	//-----------------------------------------------------------------------------------------------------
}

//FUNÇÕES-------------------------------------------------------------------

//Cria um diálogo nativo do sistema com o texto recebido como parâmetro
void displayError(char* text) {
	al_show_native_message_box(NULL, "Erro", "Ocorreu o seguinte erro:", text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

//Retorna um valor aleatório de min até max
int randomizer(int min, int max) {
	int a;
	a = rand() % (max + 1 - min) + min;
	return a;
}
//Retorna o valor da maior peça do jogo
int higher(int jogo[][7]) {
	int higher = 0;
	for (int i = 1; i < 6; i++) {
		for (int j = 1; j < 6; j++) {
			if (jogo[i][j] > higher) {
				higher = jogo[i][j];
			}
		}
	}
	return higher;
}
//Retorna um valor maior que 6 para next()
int rnext(int h) {
	int mat[7][10] = {
		{ 6, 6, 6,  6,  6,  6,  6,  6,  6,  6   },
		{ 6, 6, 6,  6,  6,  6,  6,  6,  12, 12  },
		{ 6, 6, 6,  6,  6,  12, 12, 12, 12, 12  },
		{ 6, 6, 6,  6,  12, 12, 12, 12, 24, 24  },
		{ 6, 6, 6,  12, 12, 12, 12, 24, 24, 48  },
		{ 6, 6, 6,  12, 12, 12, 24, 48, 48, 96  },
		{ 6, 6, 12, 12, 12, 24, 48, 48, 96, 192 }
	};
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
//Retorna um valor aleatório para ser a próxima peça
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
//Retorna 1 ou 2 caso a soma de A e B possa ser feita
int canSum(int a, int b) {
	//Qualquer um for -1
	if (a == -1 || b == -1) {
		return 0;
	}
	//Zero com 0
	else if (a == 0 && b == 0) {
		return 0;
	}
	//Qualquer um com 0
	else if ((a == 0 && b != 0) || (a != 0 && b == 0)) {
		return 2;
	}
	//1 com 2 ou 2 com 1
	else if ((a == 1 && b == 2) || (a == 2 && b == 1)) {
		return 1;
	}
	//Igual com Igual
	else if ((a != -1 && b != -1) && (a != 1 && a != 2 && a == b)) {
		return 1;
	}
	//sla?
	return 0;
}
//Retorna 1 se o jogo ainda pode continuar
int canContinue(int jogo[][7]) {
	int a, b;

	for (int y = 1; y < 6; y++) {
		for (int x = 1; x < 6; x++) {
			a = -1; b = 0;
			if (canSum(jogo[x][y], jogo[x + a][y + b])) {
				return 1;
			}
			a = 0; b = -1;
			if (canSum(jogo[x][y], jogo[x + a][y + b])) {
				return 1;
			}
		}
	}
	for (int y = 5; y > 0; y--) {
		for (int x = 5; x > 0; x--){
			a = 1; b = 0;
			if (canSum(jogo[x][y], jogo[x + a][y + b])) {
				return 1;
			}

			a = 0; b = 1;
			if (canSum(jogo[x][y], jogo[x + a][y + b])) {
				return 1;
			}
		}	
	}
	return 0;
}
//Retorna o índice da linha em que o próximo valor aparecerá
int randPosLines(int jogo[][7], int line) {
	int cont = 0, aux, selected;
	for (int i = 1; i < 6; i++) {
		if (jogo[line][i] == 0) {
			cont++;
			aux = i;
		}
	}
	if (cont == 1) {
		//retorna a única posição que possui 0
		return aux;
	}
	else if (cont > 1) {
		//retorna a posição que possui 0 que foi escolhida aleatóriamente
		do {
			selected = randomizer(1, 5);
		} while (jogo[line][selected] != 0);
		return selected;
	}
	return 0;
}
//Retorna o índice da coluna em que o próximo valor aparecerá
int randPosColumns(int jogo[][7], int column) {
	int cont = 0, aux, selected;
	for (int i = 1; i < 6; i++) {
		if (jogo[i][column] == 0) {
			cont++;
			aux = i;
		}
	}
	if (cont == 1) {
		//retorna a única posição que possui 0
		return aux;
	}
	else if (cont > 1) {
		//retorna a posição que possui 0 que foi escolhida aleatóriamente
		do {
			selected = randomizer(1, 5);
		} while (jogo[column][selected] != 0);
		return selected;
	}
	return 0;
}
//Faz o movimento das peças, retorna 1 caso houver movimento
int move(int jogo[][7], int a, int b, int* score) {
	int moved = 0, moves = 0, sum, var, tempScore = 0;

	if (a == -1 || b == -1) {
		for (int y = 1; y < 6; y++) {
			for (int x = 1; x < 6; x++) {
				var = canSum(jogo[x][y], jogo[x + a][y + b]);
				if (var == 1 || var == 2) {
					moved = 1;
					sum = jogo[x][y] + jogo[x + a][y + b];
					if (var == 1) {
						moves++;
						tempScore = tempScore + sum;
					}
					jogo[x + a][y + b] = sum;
					jogo[x][y] = 0;
				}
			}
		}
	}
	else if (a == 1 || b == 1) {
		for (int y = 5; y > 0; y--) {
			for (int x = 5; x > 0; x--) {
				var = canSum(jogo[x][y], jogo[x + a][y + b]);
				if (var == 1 || var == 2) {
					moved = 1;
					sum = jogo[x][y] + jogo[x + a][y + b];
					if (var == 1) {
						moves++;
						tempScore = tempScore + sum;
					}
					jogo[x + a][y + b] = sum;
					jogo[x][y] = 0;
				}
			}
		}
	}

	//Updeiteia o score
	if (moves > 1) {
		tempScore = tempScore * 2;
	}
	*score = *score + tempScore;
	
	return moved;
}
//Comandos ao mover a matriz para cima
void pressedUp(int jogo[][7], int* nextValue, int* score) {
	int check = move(jogo, -1, 0, &*score);

	if (check) {
		int i = randPosLines(jogo, 5);
		jogo[5][i] = *nextValue;
		*nextValue = next(jogo);
	}
}
//Comandos ao mover a matriz para baixo
void pressedDown(int jogo[][7], int* nextValue, int* score) {
	int check = move(jogo, 1, 0, &*score);
	if (check) {
		int i = randPosLines(jogo, 1);
		jogo[1][i] = *nextValue;
		*nextValue = next(jogo);
	}
}
//Comandos ao mover a matriz para esquerda
void pressedLeft(int jogo[][7], int* nextValue, int* score) {
	int check = move(jogo, 0, -1, &*score);
	if (check) {
		int i = randPosColumns(jogo, 5);
		jogo[i][5] = *nextValue;
		*nextValue = next(jogo);
	}
}
//Comandos ao mover a matriz para direita
void pressedRight(int jogo[][7], int* nextValue, int* score) {
	int check = move(jogo, 0, 1, &*score);
	if (check) {
		int i = randPosColumns(jogo, 1);
		jogo[i][1] = *nextValue;
		*nextValue = next(jogo);
	}

}
//Função que imprime no console a matriz, o score, e o próximo valor a ser inserido
void printm(int jogo[][7], int score, int nextValue) {
	int i, j;
	for (i = 0; i < 7; i++) {
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
//Função que cria uma nova matriz
void newBoard(int jogo[][7], int *score, int *nextValue) {
	int i, j, x, y;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 6; j++) {
			jogo[i][j] = 0;
		}
	}
	for (i = 0; i < 7; i++) {
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
	printf("Matriz Final: \n");
	*nextValue = next(jogo);
}
//Função que começa um novo jogo
void nameSelectScreen() {
	int jogo[7][7], score = 0, nextValue;
	newBoard(jogo, &score, &nextValue);
	printm(jogo, score, nextValue);
}
void playScreen() {
	int jogo[7][7];
	int score = 0;
	int nextValue;
	newBoard(jogo, &score, &nextValue);
	printm(jogo, score, nextValue);
}
//Função dos highscores
void highscoresScreen() {

}
//Função da help
void helpScreen() {

}
//Função que cria a janela do jogo
void allegro_createGameWindow() {
	gameWindow = al_create_display(350, 450);
	gameIcon = al_load_bitmap("assets/bmp/gameIcon.bmp");
	al_set_display_icon(gameWindow, gameIcon);
	al_set_window_title(gameWindow, "Threes");
}
//Função que verifica se o cursor está em cima de newGame
int hovering_newGame(ALLEGRO_EVENT event) {
	if ((event.mouse.x >= 44 && event.mouse.x <= 274) && (event.mouse.y >= 115 && event.mouse.y <= 234)) {
		return 1;
	}
	return 0;
}
//Função que verifica se o cursor está em cima de highscores
int hovering_highscores(ALLEGRO_EVENT event) {
	if ((event.mouse.x >= 44 && event.mouse.x <= 159) && (event.mouse.y >= 254 && event.mouse.y <= 369)) {
		return 1;
	}
	return 0;
}
//Função que verifica se o cursor está em cima de help
int hovering_help(ALLEGRO_EVENT event) {
	if ((event.mouse.x >= 169 && event.mouse.x <= 284) && (event.mouse.y >= 254 && event.mouse.y <= 369)) {
		return 1;
	}
	return 0;
}
//Função do menu principal
void allegro_mainMenu() {
	al_draw_bitmap(mainMenu, 0, 0, 0);
	al_flip_display();

	queue_mainMenu = al_create_event_queue();
	al_register_event_source(queue_mainMenu, al_get_mouse_event_source());
	al_register_event_source(queue_mainMenu, al_get_keyboard_event_source());
	al_register_event_source(queue_mainMenu, al_get_display_event_source(gameWindow));

	while (quit == 0) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue_mainMenu, &event);

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (hovering_newGame) {
				nameSelectScreen();
			}
			if (hovering_highscores) {
				highscoresScreen();
			}
			if (hovering_help) {
				helpScreen();
			}
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			quit = -1;
			printf("Fechou!");
			al_destroy_event_queue(queue_mainMenu);
		}
	}
}
//Função que cria a titleScreen do Allegro
void allegro_titleScreen() {
	al_draw_bitmap(background, 0, 0, 0);
	al_draw_bitmap(titleScreen, 0, 0, 0);
	al_flip_display();

	queue_titleScreen = al_create_event_queue();
	al_register_event_source(queue_titleScreen, al_get_mouse_event_source());
	al_register_event_source(queue_titleScreen, al_get_keyboard_event_source());
	al_register_event_source(queue_titleScreen, al_get_display_event_source(gameWindow));

	//Loop Inicial
	while (quit == 0) {
		al_flip_display();
		ALLEGRO_EVENT event;
		al_wait_for_event(queue_titleScreen, &event);

		//Clique em qualquer lugar para começar
		if ((event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) ||
			(event.type == ALLEGRO_EVENT_KEY_DOWN) && (event.keyboard.keycode == ALLEGRO_KEY_ENTER)) {
			al_destroy_bitmap(titleScreen);
			al_destroy_event_queue(queue_titleScreen);
			allegro_mainMenu();
			printf("Voltou!");
		}


		//Caso apertar no botão de sair
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			quit = -1;
			printf("Fechou!");
			al_destroy_event_queue(queue_titleScreen);
		}
	}
}
//Função para iniciar tudo
void start() {
	preInit();
	init();

	allegro_createGameWindow();
	allegro_titleScreen();
}
//Função principal
void main() {
	start();
}