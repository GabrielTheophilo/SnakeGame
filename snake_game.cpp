/*
        Bem vindo ao jogo da cobrinha
 - Arquivos necessários para a compilação -
Necessário(Se modif.) |                        Incluído
 ------------------------------------------------------
 Mapa                 |                        mapa.bmp
 Personagem           |            cabeca.bmp/corpo.bmp
 Power Up/Sprite      |power_up/power_up1/power_up2.bmp
 Fonte(Qlqr TTF serve)|                    Starjedi.ttf
FLAGS NECESSÁRIAS PARA O COMPILADOR/LINKADOR -> -lallegro -lallegro_image -lallegro_font -lallegro_ttf
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>   //Importação das funções de FONTE do allegro -> usar junto com 'allegro_ttf.h' para carregar fontes ttf(TrueType Font) 
#include <allegro5/allegro_ttf.h>    //Ao importar esses dois arquivos, adicionar as flags -lallegro_font -lallegro_ttf ao linkador
#include <iostream>
#include <stdlib.h>

using namespace std;

const float FPS = 10;
const int SCREEN_W = 500;
const int SCREEN_H = 550;
const int QUAD_SIZE = 20;

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

//matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor
char MAPA[26][26] =
{
    "1111111111111111111111111",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1000000000000000000000001",
    "1111111111111111111111111",
};

int andou[26][26] = {0};
int andouCorpo[26][26] = {0};
int passo = 0;

ALLEGRO_COLOR *cor = NULL;        // Ponteiro de COR para a fonte
ALLEGRO_FONT *font = NULL;        // Ponteiro de FONTE para inicializar o objeto
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_BITMAP *pacman = NULL;
ALLEGRO_BITMAP *pacman_left = NULL;
ALLEGRO_BITMAP *power_up = NULL;
ALLEGRO_BITMAP *power_up1 = NULL;
ALLEGRO_BITMAP *power_up2 = NULL;

int i = 15, j = 12;   //posicao inicial do Pacman na matriz
int q = 20;           //tamanho de cada celula no mapa
int posy = i*q;
int posx = j*q;
int tamanho = 2;
int tamanho_corpo = 1;
int contador = 0;     //contador de tempo
int HRand, WRand;     
int HRand1, WRand1;   
int HRand2, WRand2;   
int *timer1;

bool key[4] = { false, false, false, false };
bool redraw = true;
bool sprite = false;
bool power_up_tamanho = true;
bool sair = false;
bool cima, baixo, esq, dir;

void CLEAR(){                   
    system("CLS");		//limpar tela
}

int inicializa() {
    
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    mapa = al_load_bitmap("map2.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    pacman = al_load_bitmap("VERDE.bmp");
    if(!pacman)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    pacman_left = al_load_bitmap("VERMELHO.bmp");
    if(!pacman_left)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);
    power_up = al_load_bitmap("MeatSmall3.bmp");
    if(!power_up)
    {
        cout << "Falha ao carregar o power_up" << endl;
        al_destroy_display(display);
        return 0;
    }

    power_up1 = al_load_bitmap("MeatSmall.bmp");
    if(!power_up1)
    {
        cout << "Falha ao carregar o power_up" << endl;
        al_destroy_display(display);
        return 0;
    }
	power_up2 = al_load_bitmap("power_up2.bmp");
    if(!power_up2)
    {
        cout << "Falha ao carregar o power_up" << endl;
        al_destroy_display(display);
        return 0;
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }
    al_init_font_addon();    // INICIALIZAR AS FUNÇÕES DE FONTE 
    al_init_ttf_addon();     // INICIALIZAR ESTE ADDON -APÓS- INICIALIZAR O font_addon
    font = al_load_ttf_font("Starjedi.ttf", 26, 0);  // carrega arquivo ttf para formato da fonte
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int main(int argc, char **argv)
{
    srand(time(NULL));                		// SEED ALEATÓRIA           
    HRand = rand() % 22 + 1;     		// int Coordenada -> gera um número de 1 a 22 (pois a matriz de Char contém 22x22 espaços vazios)
    WRand = rand() % 22 + 1;
	
//	if (contador >= 20 && contador%20 <= 10){
	HRand1 = rand() % 22 + 1;
	WRand1 = rand() % 22 + 1;
	if(HRand1 == HRand){                    //Contenção para não gerar no mesmo lugar o "Power_up" e o "Power_up1"
	    HRand1 = rand() % 22 + 1;
	}
	if(WRand1 == WRand){                    //Contenção para não gerar no mesmo lugar o "Power_up" e o "Power_up1"
	    WRand1 = rand() % 22 + 1;       
	}
//	}
	
//	if (contador >= 35 && contador%35 <= 10){
	HRand2 = rand() % 22 + 1;
	WRand2 = rand() % 22 + 1;
	if(HRand2 == HRand || HRand2 == HRand1){//Contenção para não gerar no mesmo lugar os "Power_up","Power_up1" e "Power_up2"
	    HRand1 = rand() % 22 + 1;       
	}
	if(WRand2 == WRand || WRand2 == WRand1){//Contenção para não gerar no mesmo lugar os "Power_up","Power_up1" e "Power_up2"
		WRand1 = rand() % 22 + 1;       
	}
//	}
	
    for(int i=1;i<25;i++){                  //  procura o valor sorteado
        for(int j=1;j<25;j++){            
            if(i==HRand && j==WRand){       // se achar as coordenardas sorteadas
                MAPA[j][i]='a';             // grava 'a' na posicao, que receberá a "comida"
            }
        }
    }
	
	for(int i=1;i<25;i++){                  //  procura o valor sorteado
        for(int j=1;j<25;j++){            
            if(i==HRand1 && j==WRand1 && MAPA[i][j] != 'a' && MAPA[i][j] != 'c'){     // se achar as coordenardas sorteadas
                MAPA[j][i]='b';             // grava 'b' na posicao, que receberá a "comida especial 1"
            }
        }
    }
	
	for(int i=1;i<25;i++){                  //  procura o valor sorteado
        for(int j=1;j<25;j++){            
            if(i==HRand2 && j==WRand2 && MAPA[i][j] != 'a' && MAPA[i][j] != 'b'){     // se achar as coordenardas sorteadas
                MAPA[j][i]='c';             // grava 'a' na posicao, que receberá a "comida especial 2"
            }
        }
    }
	
	
   
	if(!inicializa()) return -1;

    cima = false;
    baixo = false;
    esq = false;
    dir = false;

    while(!sair)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            passo++;
			contador++;
			if (contador%100 == 0){
			cout << timer;
			}
            //testar colisao cobra - cobra
			if(!cima && !baixo && !esq && !dir)		//controle para nao finalizar jogo na primeira iteracao
				cima = false;
			else if(andou[i][j] < passo-tamanho+tamanho_corpo)
			{
				andou[i][j] = passo;
			}
			else
				sair = true;	//se a posicao da cabeca coincidir com alguma posicao do corpo o jogo acaba
            andouCorpo[i][j] = passo - 1;
            
            if(cima && MAPA[i][j] != '1')
            {
                i--;
                posy = i*q;

            }
            if(baixo && MAPA[i][j] != '1')
            {
                i++;
                posy = i*q;

            }
            if(esq && MAPA[i][j] != '1')
            {
                j--;
                posx = j*q;

            }
            if(dir && MAPA[i][j] != '1')
            {
                j++;
                posx = j*q;

            }
		
        //sortear novas coordenardas para 'a' (comida padrão)
            if(MAPA[i][j] == 'a') 	//localiza o 'a' na matriz
			{                     
                MAPA[i][j]='0';                  // Limpa a matriz antes de sortear um número novo
                HRand = rand() % 22 + 1;        
                WRand = rand() % 22 + 1;
                CLEAR();
				if(andou[i][j]>= passo-tamanho_corpo){
				    HRand = rand() % 22 + 1;        
                    WRand = rand() % 22 + 1;
                    CLEAR();	
				}
                for(int i=1;i<25;i++)
				{
                    for(int j=1;j<25;j++)
					{
                        if(i==HRand && j==WRand)
						{
                            MAPA[j][i]='a';		// Substitui na matriz para a nova posicao da comida
                        }
                    }
                }
				tamanho++;   
			}
        //sortear novas coordenardas para 'b' (comida especial 1)			
			if(MAPA[i][j] == 'b' && contador >= 200 && contador%200 <= 100) 	//localiza o 'a' na matriz
			{ 
                MAPA[i][j]='0';                  // Limpa a matriz antes de sortear um número novo
                HRand1 = rand() % 22 + 1;        
                WRand1 = rand() % 22 + 1;
                CLEAR();
				if(andou[i][j]>= passo-tamanho_corpo || WRand1 == WRand || HRand1 == HRand){
				    HRand1 = rand() % 22 + 1;        
                    WRand1 = rand() % 22 + 1;
                    CLEAR();	
				}
                for(int i=1;i<25;i++)
				{
                    for(int j=1;j<25;j++)
					{
                        if(i==HRand1 && j==WRand1)
						{
                            MAPA[j][i]='b';		// Substitui na matriz para a nova posicao da comida
                        }
                    }
                }
				tamanho+=2;   
			}
        //sortear novas coordenardas para 'c' (comida especial 2)				
			if(MAPA[i][j] == 'c'&& contador >= 350 && contador%350 <= 100) 	//localiza o 'c' na matriz
			{                     
                MAPA[i][j]='0';                  // Limpa a matriz antes de sortear um número novo
                HRand2 = rand() % 22 + 1;        
                WRand2 = rand() % 22 + 1;
                CLEAR();
				if(andou[i][j]>= passo-tamanho_corpo || WRand2 == WRand || HRand2 == HRand|| WRand2 == WRand1 || HRand2 == HRand1){
				    HRand2 = rand() % 22 + 1;        
                    WRand2 = rand() % 22 + 1;
                    CLEAR();	
				}
                for(int i=1;i<25;i++)
				{
                    for(int j=1;j<25;j++)
					{
                        if(i==HRand2 && j==WRand2)
						{
                            MAPA[j][i]='c';		// Substitui na matriz para a nova posicao da comida
                        }
                    }
                }
				tamanho+=3;   
			}
			
 
            else if(MAPA[i][j] == '1')        // Caso a colisão com alguma borda (parede) do mapa for detectada, o jogo é finalizado
                sair=true;

            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
				case ALLEGRO_KEY_UP:
				if(!baixo)					//bloqueia sentido para baixo se estiver indo para cima
				{	cima = true;
					baixo = false;
					esq = false;
					dir = false;                
				}
				break;
				case ALLEGRO_KEY_DOWN:
				if (!cima)					//bloqueia sentido para cima se estiver indo para baixo
				{
					 cima = false;
					 baixo = true;
					 esq = false;
					 dir = false;
				}
                break;

				case ALLEGRO_KEY_LEFT:
				if (!dir)					//bloqueia sentido para direita se estiver indo para esquerda
				{
					 cima = false;
					 baixo = false;
					 esq = true;
					 dir = false;
				}
                break;
				case ALLEGRO_KEY_RIGHT:
				if (!esq)					//bloqueia sentido para esquerda se estiver indo para direita
				{
					 cima = false;
					 baixo = false;
					 esq = false;
					 dir = true;
				}
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
				case ALLEGRO_KEY_ESCAPE:
                sair = true;
                break;
            }
        }


        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(mapa,0,0,0);
			al_draw_bitmap(power_up, HRand*q, WRand*q, 0);		    //redesenha comida nas coordenardas aleatorias
			if (contador >= 200 && contador%200 <= 100){            //contenção para aparecer a cada 20 segundos e durar 10
			al_draw_bitmap(power_up1, HRand1*q, WRand1*q, 0);		//redesenha comida nas coordenardas aleatorias
			}
			if (contador >= 350 && contador%350 <= 100){            //contenção para aparecer a cada 70 segundos e durar 15
			al_draw_bitmap(power_up2,HRand2*q,WRand2*q, 0);		//redesenha comida nas coordenardas aleatorias
			}
			
        // A FUNÇÃO AL_DRAW_TEXT SÓ FUNCIONA COM UMA STRING(const *char) ;
        // PORÉM A FUNÇÃO AL_DRAW_TEXTF FUNCIONA COM UMA STRING OU ARRAY DE CHAR FORMATADO IGUAL AO printf() DA LINGUAGEM C
        // FUNÇÂO(ponteiro da Fonte, FUNÇÃO que mapeia um RGB, pos X, pos Y, flags, STRING )
		    al_draw_text ( font, al_map_rgb(255,0,0) , 60, 500, 0, "Tempo  "); 
		    al_draw_textf( font, al_map_rgb(255,0,0) , 170, 500, 0, "%d", (contador/10)); 
            al_draw_text ( font, al_map_rgb(255,0,0) , 310, 500, 0, "Pontos  ");            
            al_draw_textf( font, al_map_rgb(255,0,0) , 430, 500, 0, "%d", tamanho-2);	//escreve a pontuacao na tela
		
            for(int i=0;i<26;i++)
			{
				for(int j=0;j<26;j++)
				{
					if (andou[i][j]>= passo-tamanho)
					{
						al_draw_bitmap(pacman_left,j*q,i*q,0);		//atualiza e redesenha a cabeca
					}
					if(andouCorpo[i][j]>= passo-tamanho_corpo)
					{
						al_draw_bitmap(pacman,j*q,i*q,0);			//atualiza e redesenha o corpo
					} 
				}
			}
			al_flip_display();
		}
	}

    al_destroy_bitmap(mapa);
    al_destroy_bitmap(pacman);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
