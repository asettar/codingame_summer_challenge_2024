#include "header.hpp"
/*start*/
gamesInfo game;

void    playGame(int gameNumber) {
    cin >> game.gpu; cin.ignore();
    
    if (gameNumber == 0) {
        game.hurdle = new hurdleGame(game);
        game.hurdle->play(game);
    }
    else if (gameNumber == 1) {
        game.wind = new windGame(game);
        game.wind->play(game.movesCnt);
    }
    else if (gameNumber == 2) {
        int num;
        for(int i = 0; i < 7; i++) cin >> num;
    }
    else {
        game.diving = new divingGame(game);
        game.diving->play(game);
    }
}

int main()
{
    cin >> game.player_idx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();

    // game loop
    while (1) {
        game.movesCnt.clear();
        for (int i = 0; i < 3; i++) {
            string score_info;
            getline(cin, score_info);
        }
        for (int i = 0; i < nb_games; i++) {
            playGame(i);
        }   
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

/*end*/