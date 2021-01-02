#ifndef CONTROLPROCESS_HPP
#define CONTROLPROCESS_HPP

#include "sharedmemory.hpp"
#include "sharedqueue.hpp"
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
        SharedMemory shmem;
        SharedQueue shque;
        // GameProcess gameProcess;
        

    public:
        ControlProcess();
        ~ControlProcess();

        void readCoords();
        void steer();


        [[noreturn]] void run();
};

#endif