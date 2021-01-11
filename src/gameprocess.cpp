#include "../include/gameprocess.hpp"



GameProcess::GameProcess() {
    // max_waiting_time = 10;

    if (TEST_MODE)
        time_for_move = 16000; //16ms
    else
        time_for_move = 5000000; //5000ms == 5s
}

[[noreturn]] void GameProcess::run() {

    usleep(1000000);
    std::cout << "[G] Game running [[noreturn]]" << std::endl << std::flush;
    while (true) {        
        generateNextMove();
        sendPing();
        waitForSignal();
        validateSignal();     
    }
}


void GameProcess::generateNextMove() {

    if (!failed) {

        //skip generating next move if human failed to satisfy it previously (time obviously hasnt ran out)
        if (!human_incorrect_move) {

            if ( !TEST_MODE ) {
                //if in human mode, wait for a moment
                usleep(1000000);
            }

            current_key = rg.generateMove();

            time_obstacle_spotted = std::chrono::system_clock::now();
            
            message_out.key = current_key;
            message_out.deadline = time_obstacle_spotted;
        
        if (CNSL_LOG)
            std::cout << "[G] Next move: '" << current_key << "' with deadline UNKNOWN" << std::endl << std::flush;


        //still waiting for human movement
        } else {
            // std::cout << "[G] still waiting for correct move of: '" << current_key << "'" << std::endl << std::flush;
        }



    } else {

        
        if (ITERATIONS == 1) {

            std::cout << "[G] Game ended. Saving stats to file." << std::endl << std::flush;

            saveOnExit();
            usleep(100000000);


        } else {    


            ++iteration_id;
            resetGame();


        }
    }
}


void GameProcess::sendPing() {

    if (!failed) {
        shque.push(&message_out);

        if (CNSL_LOG)
            std::cout << "[G] Pinged PROD with new move " << std::endl << std::flush;        
    }
}





void GameProcess::waitForSignal() {


    steeringQueue.pop(&message_in);

    time_move_received = std::chrono::system_clock::now();


    if (CNSL_LOG)
        std::cout << "[G] Got input signal: '" << message_in.key << "'" << std::endl << std::flush;
    
}



void GameProcess::validateSignal() {

    
    std::chrono::microseconds time_elapsed_micro = std::chrono::duration_cast<std::chrono::microseconds>(time_move_received - time_obstacle_spotted);


    //if human is playing, allow for fail moves
    if (!TEST_MODE) {

        if (message_in.key == message_out.key && time_elapsed_micro.count() < time_for_move) {
            //human made a correct move
            time_for_move = time_for_move * scale_factor;
            human_incorrect_move = false;
        
        } else if (message_in.key != message_out.key &&  time_elapsed_micro.count() < time_for_move) {
            //human made a wrong move but theres still time
            human_incorrect_move = true;
        
        }else {
        // } else if (message_in.key != message_out.key && time_elapsed_micro.count() > time_for_move) {
            //human failed to make a correct move in given time
            human_incorrect_move = true;
            failed = true;
        }


    } else {


        if (message_in.key == message_out.key && (first_move || time_elapsed_micro.count() < time_for_move)) {

            if (CNSL_LOG)
                std::cout << "[G] Successful move." << std::endl;

            time_for_move = time_for_move * scale_factor;
        
        } else
            failed = true;

    }

    first_move = false;

}

void GameProcess::saveOnExit() {

    auto time = std::chrono::system_clock::now();
    const char* extension = ".txt";
    const char* target_path = "../single_measurements/";
    std::string file_name{};

    std::time_t time_ctype = std::chrono::system_clock::to_time_t(time);

    file_name.append(target_path);
    file_name.append( std::ctime(&time_ctype) );
    file_name.append(extension);


    for( int length = 0; length < file_name.length(); length++) {

        if (file_name.at(length) == ':' || file_name.at(length) == ' ')
            file_name.at(length) = '-';
    }

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

    if (TEST_MODE)
        time_for_move = 16000;
    else
        time_for_move = 5000000;

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
    // const char* prefix = "iters_ ";
    const char* extension = ".txt";
    std::string file_name{};

    auto time = std::chrono::system_clock::now();
    std::time_t time_ctype = std::chrono::system_clock::to_time_t(time);

    file_name.append(target_folder);
    // file_name.append(prefix);
    file_name.append(TEST_PREFIX);
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