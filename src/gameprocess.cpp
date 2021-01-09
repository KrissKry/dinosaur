#include "../include/gameprocess.hpp"



GameProcess::GameProcess() {
    // max_waiting_time = 10;
}

GameProcess::~GameProcess() {
    // delete shque;s
}

void GameProcess::generateNextMove() {

    if (!failed) {
        // std::cout << "[G] Generating next move." << std::endl << std::flush;

        current_key = rg.generateMove();

        time_obstacle_spotted = std::chrono::system_clock::now();
        
        message_out.key = current_key;
        message_out.deadline = time_obstacle_spotted;
        if (CNSL_LOG)
            std::cout << "[G] Next move: '" << current_key << "' with deadline UNKNOWN" << std::endl << std::flush;
        

    } else {

        
        // sem_wait(game_action);
        if (ITERATIONS == 1) {
            std::cout << "[G] Game ended. Saving stats to file." << std::endl << std::flush;
            saveOnExit();
            usleep(100000000);
        } else {    
            // std::cout << "[G] Game ended. Reverting state." << std::endl << std::flush;
            ++iteration_id;
            resetGame();
        }
        // usleep(100000000);
    }
}


void GameProcess::sendPing() {

    if (!failed) {
        shque.push(&message_out);
        if (CNSL_LOG)
            std::cout << "[G] Pinged PROD with new move " << std::endl << std::flush;        
    }
}



[[noreturn]] void GameProcess::run() {

    usleep(500000);
    std::cout << "[G] Game running [[noreturn]]" << std::endl << std::flush;
    while (true) {        
        generateNextMove();
        sendPing();
        waitForSignal();
        validateSignal();     
    }
}


void GameProcess::waitForSignal() {

    // std::cout << "[G] Waiting for input signal..." << std::endl << std::flush;
    steeringQueue.pop(&message_in);
    time_move_received = std::chrono::system_clock::now();
    if (CNSL_LOG)
        std::cout << "[G] Got input signal: '" << message_in.key << "'" << std::endl << std::flush;
    
}



void GameProcess::validateSignal() {

    
    std::chrono::microseconds time_elapsed_micro = std::chrono::duration_cast<std::chrono::microseconds>(time_move_received - time_obstacle_spotted);
    // std::cout << "us elapsed: " << time_elapsed_micro.count() << std::endl << std::flush;
    
    if (message_in.key == message_out.key && (first_move || time_elapsed_micro.count() < time_for_move)) {

        if (CNSL_LOG)
            std::cout << "[G] Successful move." << std::endl;
        time_for_move = time_for_move * scale_factor;
    
    } else {
        std::cout << "[G] Game is fked now." << std::endl;
        failed = true;
    }

    if ( !first_move )
        moves_time_micro.push_back( time_elapsed_micro.count() );

    first_move = false;

    // std::cout << "[G] New max time: " << time_for_move << std::endl << std::flush;
}

void GameProcess::saveOnExit() {

    auto time = std::chrono::system_clock::now();
    const char* extension = ".txt";

    std::time_t time_ctype = std::chrono::system_clock::to_time_t(time);
    std::string file_name = std::ctime(&time_ctype);

    for( int length = 0; length < file_name.length(); length++) {

        if (file_name.at(length) == ':' || file_name.at(length) == ' ')
            file_name.at(length) = '-';
    }

    file_name.append(extension);
    std::cout << "[G] saving to: " << file_name << std::endl << std::flush;

    std::ofstream outFile(file_name);
    outFile << "Moves accepted: " << moves_time_micro.size() << "\n";
    outFile << "Failed when exceeded time:" << time_for_move << "us\n";

    for (const auto &e : moves_time_micro)
        outFile << e << "\n";

    // ++iteration_id;
}

void GameProcess::resetGame() {

    int moves = moves_time_micro.size();
    moves_per_iteration.push_back( moves );

    std::pair<int, int> movecnt_failtime = std::make_pair (moves, time_for_move);

    moves_failtime_array.push_back( movecnt_failtime );

    moves_time_micro.clear();
    failed = false;
    first_move = true;
    time_for_move = 16000;


    if (iteration_id  >= ITERATIONS) {
        
        
        std::cout << "[G] Ended " << ITERATIONS << " iterations." << std::endl << std::flush;

        std::string file_name = prepareOutputFile();

        saveRunsToFile(file_name);

        usleep(900000000);



    } else {
        std::cout << "[G] ---" << iteration_id << "--- ended in " << moves << " moves." << std::endl << std::flush;
        std::cout << "[G] Game has been reset. Continuing execution..." << std::endl << std::flush;
    }



}



std::string GameProcess::prepareOutputFile() {

    const char* target_folder = "../measurements/";
    const char* prefix = "iters-";
    const char* extension = ".txt";
    std::string file_name{};

    auto time = std::chrono::system_clock::now();
    std::time_t time_ctype = std::chrono::system_clock::to_time_t(time);

    file_name.append(target_folder);
    file_name.append(prefix);
    file_name.append( std::ctime(&time_ctype) );
    file_name.append(extension);


    for( int length = 0; length < file_name.length(); length++)
        if (file_name.at(length) == ':' || file_name.at(length) == ' ')
            file_name.at(length) = '_';

    return file_name;
}


void GameProcess::saveRunsToFile(std::string file_name) {

    std::ofstream outFile(file_name);
    outFile << "Moves | Time:\n";

    for (const auto &e : moves_failtime_array)
            outFile << e.first << " " << e.second << "\n";

    outFile.close();


    std::cout << "[G] Saved data to file: " << file_name << std::endl << std::flush; 

}