
#define WIDTH 1280
#define HEIGHT 720
#define QUEUE_NAME "KOLEJKA"
#define FRAME_Q_NAME "KOLEJKA_KLATEK" //informacja o dostepnej klatce Producent->Filtr
#define COORDS_Q_NAME "KOLEJKA_XY"  //informacja o dostepnych koordynatach Filtr->Klient
// #define GAME_Q_NAME "KOLEJKA_RUCHOW" //informacja o następnym ruchu wymaganym przez grę

// WSPÓŁDZIELONA PAMIĘĆ DLA PRODUCENT->FILTR
// WSPÓŁDZIELONA KOLEJKA DLA FILTR->KLIENT

//predefiniowane koordynaty
#define X_REG 1000
#define Y_UP  200
#define Y_REG 360
#define Y_DWN 600