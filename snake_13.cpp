/*
        Bem vindo ao jogo da cobrinha
 - Arquivos necessários para a compilação -


Necessário(Se modif.) |                   Incluído
 -------------------------------------------------
 Mapa                 |                   map1.bmp
 Personagem           | pacman.tga/pacman_left.tga
 Power Up/Sprite      |             MeatSmall4.bmp
 Fonte(Qlqr TTF serve)|               Starjedi.ttf


FLAGS NECESSÁRIAS PARA O COMPILADOR/LINKADOR -> -lallegro -lallegro_image -lallegro_font -lallegro_ttf

*/
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>   //Importação das funções de FONTE do allegro -> usar junto com 'allegro_ttf.h' para carregar fontes ttf(TrueType Font) 
#include <allegro5/allegro_ttf.h>    //Ao importar esses dois arquivos, adicionar as flags -lallegro_font -lallegro_ttf ao linkador
#include <iostream>
#include <stdlib.h>

using namespace std;

char PONTOS;

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

char corpo[26][26] = {0};
int andou[26][26] = {0};
int andouCorpo[26][26] = {0};
int passo = 0;


ALLEGRO_COLOR *cor = NULL;        // Ponteiro de COR para a fonte
ALLEGRO_FONT *font = NULL;        // Ponteiro de FONTE para inicializar o objeto
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa   = NULL;
ALLEGRO_BITMAP *pacman   = NULL;
ALLEGRO_BITMAP *pacman_left = NULL;
ALLEGRO_BITMAP *power_up = NULL;
ALLEGRO_BITMAP *power_up2 = NULL;
int i = 15, j = 12;   //posicao inicial do Pacman na matriz
int q = 20;           //tamanho de cada celula no mapa
int posy = i*q;
int posx = j*q;

int tamanho = 3;
int tamanho_corpo = 1;

bool key[4] = { false, false, false, false };
bool redraw = true;
bool sprite = false;
bool power_up_tamanho = true;
bool sair = false;
bool cima, baixo, esq, dir;

void CLEAR(){                   // FUNÇÃO PARA MANDAR AO TERMINAL (Prompt de comando(Cmd), Windows) O COMANDO DE LIMPAR  --- Equivalente em POWERSHELL OU BASH É O COMANDO ("CLEAR")
    system("CLS");
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

    pacman = al_load_bitmap("VERMELHO.bmp");
    if(!pacman)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    pacman_left = al_load_bitmap("VERDE.bmp");
    if(!pacman_left)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);
    power_up = al_load_bitmap("MeatSmall4.bmp");
    if(!power_up)
    {
        cout << "Falha ao carregar o power_up" << endl;
        al_destroy_display(display);
        return 0;
    }
    power_up2 = al_load_bitmap("MeatSmall3.bmp");
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
    al_init_ttf_addon();    // INICIALIZAR ESTE ADDON -APÓS- INICIALIZAR O font_addon
    font = al_load_ttf_font("Starjedi.ttf", 26, 0);  // al_load_ttf_font(ARQUIVO, tamanho, flags) 
    
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
    srand(time(NULL));                // SEED ALEATÓRIA           
    int HRand = rand() % 22 + 1;     // int Coordenada -> gera um número de 1 a 22 (pois a matriz de Char contém 22x22 espaços vazios)
    while(HRand>=23){               // Contingência para caso fosse sorteado algo além do range | Obsoleto
        HRand = rand() % 22 + 1;    
    }
    cout << HRand << endl;
    int WRand = rand() % 22 + 1;
    while(WRand>=23){
        WRand = rand() % 22 + 1;
    }
    for(int i=1;i<25;i++){                  // Após o número sorteado, o For loop aninhado passará pelas posições da matriz para procurar o valor sorteado
        for(int j=1;j<25;j++){            
            if(i==HRand && j==WRand){       // Quando encontrar as coordenadas do mapa onde o ponto foi sorteado, adicionará ao array de char um valor de 'a' no lugar de '0'
                MAPA[j][i]='a';             // Passo necessário para manter registrado o local da matriz onde a comida da cobra estará.
            }
        }
    }
    //if(tamanho>=4){
    //    for(int i=1;i<25;i++){
    //        for(int j=1;j<25;j++){
    //            if(andou[i][j]>= passo-tamanho){
    //                MAPA[j][i]='b';
    //            }
    //        }
    //    }
    //}



    for(int i = 0;i<25; i++){               // Simples For loop aninhado para printar as matrizes no terminal
        for(int j = 0;j<25; j++){           // Apenas para debug
            cout << MAPA[i][j];
            if(j==24)
                cout << '\n';
        }
    }
    
    cout << MAPA[HRand][WRand] << endl;     // Apenas para debug

    

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
            //CLEAR();
            //for(int i = 0;i<25; i++){
            //    for(int j = 0;j<25; j++){
            //        cout << MAPA[i][j];
            //        if(j==24)
            //            cout << '\n';
            //    }
            //}
            passo++;
            andou[i][j]= passo;
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
        // Condicional que estabelece que, se a colisão com a cobra for detectada(MAPA[i][j] é a posição da cobra), um novo número será sorteado
            if(MAPA[i][j] == 'a'){                     
                MAPA[i][j]='0';                  // Limpa a matriz antes de sortear um número novo
                HRand = rand() % 22 + 1;        
                WRand = rand() % 22 + 1;
                CLEAR();
                for(int i=1;i<25;i++){              // Substitui na matriz o novo valor da comida da cobrinha
                    for(int j=1;j<25;j++){
                        if(i==HRand && j==WRand){
                            MAPA[j][i]='a';
                        }
                    }
                }

            if(tamanho>=4){
        
                cout << "ACERTOU" << endl;
                for(int i=1;i<25;i++){
                    for(int j=1;j<25;j++){
                        if(andou[i][j]>= passo-tamanho){
                            MAPA[i][j]='b';
                        }
                    }
                }
            }
            


                tamanho++;   
            }
            

            else if(MAPA[i][j] == '1'){         // Caso a colisão com alguma borda do mapa for detectada, o jogo é finalizado
                sair=true;
            }


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
            cima = true;
            baixo = false;
            esq = false;
            dir = false;
                break;

            case ALLEGRO_KEY_DOWN:
            cima = false;
            baixo = true;
            esq = false;
            dir = false;
                break;

            case ALLEGRO_KEY_LEFT:
            cima = false;
            baixo = false;
            esq = true;
            dir = false;
                break;

            case ALLEGRO_KEY_RIGHT:
            cima = false;
            baixo = false;
            esq = false;
            dir = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            // case ALLEGRO_KEY_UP:
            //     key[KEY_UP] = false;
            //     break;
            //
            // case ALLEGRO_KEY_DOWN:
            //     key[KEY_DOWN] = false;
            //     break;
            //
            // case ALLEGRO_KEY_LEFT:
            //     key[KEY_LEFT] = false;
            //     break;
            //
            // case ALLEGRO_KEY_RIGHT:
            //     key[KEY_RIGHT] = false;
            //     break;

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
            
        
        // Se 

                al_draw_bitmap(power_up2, HRand*q, WRand*q, 0);





        // A FUNÇÃO AL_DRAW_TEXT SÓ FUNCIONA COM UMA STRING(const *char) ;
        // PORÉM A FUNÇÃO AL_DRAW_TEXTF FUNCIONA COM UMA STRING OU ARRAY DE CHAR FORMATADO IGUAL AO printf() DA LINGUAGEM C
        // FUNÇÂO(ponteiro da Fonte, FUNÇÃO que mapeia um RGB, pos X, pos Y, flags, STRING )
            al_draw_text ( font, al_map_rgb(255,0,0) , 310, 510, 0, "Pontos");            
            al_draw_textf( font, al_map_rgb(255,0,0) , 425, 510, 0, "%d", tamanho-3);

            for(int i=0;i<26;i++){
              for(int j=0;j<26;j++){
                if (andou[i][j]>= passo-tamanho){
                  al_draw_bitmap(pacman_left,j*q,i*q,0);
                }
                if(andouCorpo[i][j]>= passo-tamanho_corpo){

                    al_draw_bitmap(pacman,j*q,i*q,0);
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
