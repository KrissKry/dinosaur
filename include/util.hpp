
#define WIDTH 1280                                  //expected frame size from webcam
#define HEIGHT 720                                  //needs editing in case of different setup ;-;

#define FRAME_SHM "PAMIEC_KLATEK"                   //shared memory name for frame data
#define FRAME_SHM_CNT 1                             //max number of frames in shared memory

#define FRAME_SIZE WIDTH*HEIGHT*3                   //frame size (char array of cv::Mat.data) in bytes
#define BUFFER_SIZE FRAME_SIZE*FRAME_SHM_CNT        //buffer size in bytes for n frames

#define PROD_SEM "PROD_SEM"                         //semaphore names
#define CONS_SEM "CONS_SEM"
#define GAME_SEM "GAME_SEM"

#define MQ_PRODUCER_GAME "MQ_PRODUCER_GAME"         //message queue name shared between producer and game (game sends expected move)
#define MQ_FILTER_CLIENT "MQ_FILTER_CLIENT"         //message queue name shared between filter and client (filter sends coords)
#define MQ_CLIENT_GAME  "MQ_CLIENT_GAME"            //message queue with keys pressed sent to the game process
//predefiniowane koordynaty
#define X_REG 1000
#define Y_UP  200
#define Y_REG 360
#define Y_DWN 600

