#include "header.hpp"
/*start*/
void    checkWinsLoses(hurdleGame *hurdle, windGame *wind, divingGame *diving){
    if (wind->gpu != "GAME_OVER" && (100 - game.turn < wind->gpu.size() || wind->guarentedWinLose())) wind->gpu = "GAME_OVER";
    if (diving->gpu != "GAME_OVER" && (100 - game.turn < diving->gpu.size() || diving->guarentedWinLose())) diving->gpu = "GAME_OVER";
    if (hurdle->gpu != "GAME_OVER" && (hurdle->stun[game.playerIdx] || hurdle->guarentedWinLose(game))) hurdle->gpu = "GAME_OVER";
}

void    prioritizeGames(hurdleGame *hurdle, windGame *wind, divingGame *diving) {
        vector<pair<double, char>> order;
        float sum = 0;
        if (wind->gpu != "GAME_OVER") {
            double medals = min((double)(game.windMedals[0] * 3 + game.windMedals[1]) / 12.0, 1.0);
            double toend = wind->gpu.size() / 16.0;
            order.push_back({toend * 0.3 + medals * 0.7 , 'E'});
            sum += 1.0 - order.back().first;
        }
        if(hurdle->gpu != "GAME_OVER" && !hurdle->stun[game.playerIdx]) {
            double pos = hurdle->getCurPlace() / 3.0;
            double toend = 1 - hurdle->getMyPos() / 31.0;
            double medals = min((double)(game.hurdleMedals[0] * 3 + game.hurdleMedals[1]) / 12.0, 1.0);
            order.push_back({pos * 0.3 + toend * 0.4 + medals * 0.3, 'H'});
            sum += 1.0 - order.back().first;

        }
        if (diving->gpu != "GAME_OVER") { 
            double pos = diving->getCurPlace() / 3.0;
            double medals = min((double)(game.divingMedals[0] * 3 + game.divingMedals[1]) / 12.0, 1.0);
            order.push_back({pos * 0.4  + medals * 0.6 , 'D'});
            sum += 1.0 - order.back().first;
        }

        sort(order.begin(), order.end());
        // cerr << order.size() << ' ' << sum << ' ' << game.gamesOrder.size() << endl;
        if (sum){
            for(auto &[l, r] : order)
                game.gamesOrder[r] = (1 - l) / sum;
        }
        
        // cerr << "Order\n";
        // for(auto&[l, r] : game.gamesOrder) cerr << l << ' ' << r << endl;

}


int main()
{
    srand(time(0));
    cin >> game.playerIdx; cin.ignore();
    int nb_games;
    cin >> nb_games; cin.ignore();
    // game.generatePermutations();
    // game loop
    while (1) {
        for (int i = 0; i < 3; i++) {
            string scoreInfo;
            getline(cin, scoreInfo);
            if (i == game.playerIdx)
                game.setMedals(scoreInfo);
        }
        for (int i = 0; i < nb_games; i++) {
            setupGame(i);
        }
        MCTS mct;
        game.gamesOrder.clear();
        checkWinsLoses(game.hurdle, game.wind, game.diving);
        prioritizeGames(game.hurdle, game.wind, game.diving);
        int cntOverGames = (game.hurdle->gpu == "GAME_OVER") + (game.wind->gpu == "GAME_OVER") + (game.diving->gpu == "GAME_OVER");
       
        if (cntOverGames == 2) {
            if (game.hurdle->gpu != "GAME_OVER")
                game.hurdle->play();
            else if (game.diving->gpu != "GAME_OVER")
                cout << mp[game.diving->gpu[0]] << endl;
            else game.wind->play();
        }
        else {
            miniGame mg(game.hurdle, game.wind, game.diving);
            if (game.turn == 0)
                    cout << mct.findNextMove(mg, 10000) << endl;
            else cout << mct.findNextMove(mg, 6000) << endl;
        }
        game.turn++;
    }
    return 0;
}
/*end*/