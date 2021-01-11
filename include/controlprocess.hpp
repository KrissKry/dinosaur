#ifndef CONTROLPROCESS_HPP
#define CONTROLPROCESS_HPP

#include <iostream>
#include <thread>
#include <unistd.h>

#include "messagequeue.hpp"
#include "gameprocess.hpp"
#include "util.hpp"
/*
    ControlProcess uses coords sent by filter process in shared memory to steer the dinosaur adequately.
    tutaj koncept ciężki ehh
    Wariant 1:
    Stworzona bedzie jeszcze turbo uproszczona wersja (proof of concept) dinozaura specjalnie pod program GameProcess czy coś
    i on będzie losował ruch (góra dół) który należy wykonać w liniowo zmniejszającym sie czasie.
    
        Producent który produkuje klatki adekwatnie do potrzeb tego procesu automatycznie nie korzystając z obrazu z kamerki 
        (niwelowanie błędu ludzkiego z równania). Miałby określoną szansę wyprodukowania błędnej klatki. W pamięci byłyby 3 klatki
        (ruch w górę, dół oraz idle), z których by wybierał. Przy dostatecznie dużym czasie
        reakcji (co najmniej dwie klatki) mógłby poprawić swój ruch.


    Wariant 2:
        Korzystamy z kamerki, wraz z naszą uproszczoną gierką. Najmniejszy czas ruchu postaci w grze wynosi tyle ile wydajność klatek,
        a dodatkowo pojawia się tu błąd ludzki, niedostateczny czas reakcji oraz błędny odczyt

*/
class ControlProcess {

    private:

        MessageQueue shque = {false, MQ_FILTER_CLIENT};
        MessageQueue game_queue = {true, MQ_CLIENT_GAME};
        // GameProcess game = {12};
        // GameProcess game
        // std::thread game_thread;
        coords_message current_coords;// = new coords_message;
        game_message current_move;
        char pressed_key{};

    public:
        ControlProcess();
        ~ControlProcess() {}

        void readCoords();
        void send_steering_signal();


        [[noreturn]] void run();
};

#endif