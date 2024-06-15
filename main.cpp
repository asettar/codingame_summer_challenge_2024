#include "header.hpp"
/*start*/
gamesInfo game;

void    checkcheckHurdleDiving() {
    // firstTime
    if (!game.diveCnt && !game.windCnt && !game.diving->player_idx) {
        // start with the one that take minimum moves possibles
        int windMoves = game.wind->gpu.size();

    }
}

void    setupGame(int gameNumber) {
    cin >> game.gpu; cin.ignore();
    
    if (gameNumber == 0)
        game.hurdle = new hurdleGame(game);
    else if (gameNumber == 1)
        game.wind = new windGame(game);
    else if (gameNumber == 2) {
        int num;
        for(int i = 0; i < 7; i++) cin >> num;
    }
    else
        game.diving = new divingGame(game);
}

void    checkBestMoves() {
    // if (!game.hurdleCnt && !game.diveCnt && !game.hurdle->player_pos) {
    //     // first turn start either hurdle or diving base on minimum moves
    //     int divingMoves = game.diving->gpu.size();
    //     int hurdleMoves = game.hurdle->getMinMoves(0); 

    //     if (divingMoves > hurdleMoves) {
    //         game.playingHurdle = 1;
    //         game.hurdle->play(game);
    //     }
    //     else game.diving->play(game);
    // }
    if (game.hurdle->gpu != "GAME_OVER" && game.hurdleMedals[0] == 0 && !game.hurdle->guarentedWin(game))
        game.hurdle->play(game);
    else if (game.diving->gpu != "GAME_OVER" && game.divingMedals[0] == 0 && !game.diving->guarentedWin())
        game.diving->play(game);
    else if (game.hurdleMedals[0] == 1 && (game.divingMedals[0] == 1 || (game.divingMedals[0] == 0 && game.diving->guarentedWin())))
        game.hurdle->play(game);
        // add 1 medal to hurdle 
    else if ((game.hurdleMedals[0] == 2 || (game.hurdle->guarentedWin(game) && game.hurdleMedals[0] == 1)) 
        && game.divingMedals[0] == 1 && !game.diving->guarentedWin()) {
        // add 1 medal to diiving
        game.diving->play(game);
    }
    else { 
        // now you guarented 2 gold in hurdle and 2 gold in diving 
        // strategie for know if windgpu is less than 10 play for wind
        // else play either for the hurdle or diving

        // for now play just diving (for debugging first 2 medals)
        // if (game.wind->gpu.size() >= 10) {
        //     game.diving->play(game);
        //     game.
        // }  
        if (game.wind->gpu != "GAME_OVER" && game.wind->gpu.size() <= 9)
            game.wind->play(game);
        game.hurdle->score(game);
        game.diving->play(game);
    }
}

int main()
{
    cin >> game.player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();
    game.generatePermutations();
    // game loop
    while (1) {
        game.movesCnt.clear();
        for (int i = 0; i < 3; i++) {
            string scoreInfo;
            getline(cin, scoreInfo);
            if (i == game.player_idx)
                game.setMedals(scoreInfo);
        }
        for (int i = 0; i < nb_games; i++) {
            setupGame(i);
        }
        checkBestMoves();
        cerr << "\n---------------\n";
        double maxMoves = -2e9;
        string ans;
        for(string s : {"RIGHT", "UP", "DOWN", "LEFT"}) {
            if (game.movesCnt[s] > maxMoves) maxMoves = game.movesCnt[s], ans = s;
            cerr << s << ' ' << game.movesCnt[s] << endl;
        }
        cout << ans << endl;
    }
}
