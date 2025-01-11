#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

struct bowler{
    string name;
    int wickets;
    int runs;
    int wides;
    int nb;
    int maidens;
    double overs;

    bowler(string nom){
        name = nom;
        wickets = 0;
        runs = 0;
        maidens = 0;
        overs = 0.0;
        wides = 0;
        nb = 0;
    }
};

struct batsman{
    string name;
    int runs;
    int ballsFaced;
    bool out;
    bool onStrike;
    bool currentlyBatting;
    int fours;
    int sixes;
    int ducks;
    bool noh;

    batsman(string nom){
        name = nom;
        runs = 0;
        ballsFaced = 0;
        out = false;
        onStrike = false;
        currentlyBatting = false;
        fours = 0;
        sixes = 0;
        ducks = 0;
        noh = false;
    }
};
vector <bowler> prevBowlers;
vector <batsman> batsmen;
vector <vector<string>> spreadsheet;
int wickets = 0;
int noh = 0;

//Get bowler's name to check if they previously bowled, if so return the index in the prevBowlers vector
int prevBowled(string name){
    int prevBowled = -1;
    for (int i = 0; i < prevBowlers.size(); i++){
        if (prevBowlers[i].name.compare(name) == 0){
            prevBowled = i;
            break;
        }
    }
    return prevBowled;
}

//Returns index of batsman on strike
int getBatsmanOnStrike(){
    int index = -1;
    for (int i = 0; i < batsmen.size(); i++){
        if (batsmen[i].onStrike == true){
            index = i;
            break;
        }
    }
    return index;
}

//Returns index of batsman off strike
int getBatsmanOffStrike(){
    int index = -1;
    for (int i = 0; i < batsmen.size(); i++){
        if (batsmen[i].currentlyBatting == true && batsmen[i].onStrike == false){
            index = i;
        }
    }
    return index;
}

//Updates the batsman on strike to the other batsman currently playing and who previously was not on strike
//Index is index of batsman who was previously on strike and is now off strike
void updateStrike(int index){
    for (int i = 0; i < batsmen.size(); i++){
        if(batsmen[i].currentlyBatting == true && batsmen[i].onStrike == false){
            batsmen[i].onStrike = true;
        }
    }
    batsmen[index].onStrike = false;
}

//Updates stats when a wicket falls so the new batsman is in the game
//bool checks to see if there was a runout at the non-striker's end or non striker retired out
void nextBatsman(bool nsEnd){
    if (nsEnd == false){
        int index = getBatsmanOnStrike();
        batsmen[index].out = true;
        batsmen[index].currentlyBatting = false;
        batsmen[index].onStrike = false;
        if (batsmen[index].runs == 0){
            batsmen[index].ducks++;
        }
        //New batsman
        batsmen[wickets + noh + 1].currentlyBatting = true;
        batsmen[wickets + noh + 1].onStrike = true;
    }
    else{
        int index = getBatsmanOffStrike();
        batsmen[index].out = true;
        batsmen[index].currentlyBatting = false;
        if (batsmen[index].runs == 0){
            batsmen[index].ducks++;
        }
        //New batsman
        batsmen[wickets + noh + 1].currentlyBatting = true;
        batsmen[wickets + noh + 1].onStrike = false;
    }
}

//Gets next batsman if the current batsman is not out hurt
void nextBatsmanNOH(int strike){
    if (strike == 1){
        int index = getBatsmanOnStrike();
        batsmen[index].currentlyBatting = false;
        batsmen[index].onStrike = false;
        batsmen[index].noh = true;
        //New batsman
        batsmen[wickets + noh + 1].currentlyBatting = true;
        batsmen[wickets + noh + 1].onStrike = true;
    }
    else if (strike == 2){
        int index = getBatsmanOffStrike();
        batsmen[index].currentlyBatting = false;
        batsmen[index].noh = true;
        //New batsman
        batsmen[wickets + noh + 1].currentlyBatting = true;
        batsmen[wickets + noh + 1].onStrike = false;
    }
}

//Example string: "Arya: 6 WD 7NB W 0 2 1 3"
void parseInput(string input){
    int colon = input.find(":");
    string name = input.substr(0, colon);

    int index = prevBowled(name);
    //cout << index << endl;
    if (index == -1){
        bowler newBowl = bowler(name);
        prevBowlers.push_back(newBowl);
        index = prevBowlers.size() - 1;
    }

    string figures = input.substr(colon + 2, input.length() - colon);
    //cout << figures << endl;
    
    //checks to see if a bowler was subbed in for another bowler
    bool sub = false;
    if (figures.find("-s") != string::npos){
        sub = true;
        figures = figures.substr(0, figures.find("-s") - 1);
    }
    
    //Push all balls in the over to the stats vector
    vector <string> stats;
    while(figures.find(" ") != string::npos){
        int space = figures.find(" ");
        string fig = figures.substr(0, space);
        if (fig.compare("NB") == 0 || fig.compare("nb") == 0 || fig.compare("Nb") == 0){
            fig = "1" + fig;
        }
        stats.push_back(fig);
        figures = figures.substr(space + 1, figures.length() - space - 1);
        //cout << figures << endl;
    }
    if (figures.compare("NB") == 0 || figures.compare("nb") == 0 || figures.compare("Nb") == 0){
            figures = "1" + figures;
        }
    stats.push_back(figures);

    bool maiden = true;
    for (int i = 0; i < stats.size(); i++){
        if ((stats[i].compare("W") == 0) || stats[i].compare("w") == 0){
            prevBowlers[index].overs += 0.1;
            prevBowlers[index].wickets++;
            wickets++;
            int in = getBatsmanOnStrike();
            batsmen[in].ballsFaced++;
            nextBatsman(false);
        }

        //Not out hurt
        //Format: who's out not hurt, NOH
        //ex: 1NOH means batsman on strike is not out hurt, 2NOH means batsman off strike is not out hurt
        else if (stats[i].find("NOH") != string::npos || stats[i].find("noh") != string::npos || stats[i].find("Noh") != string::npos){
            char strike = stats[i].at(0);
            int s = strike - '0';
            noh++;
            nextBatsmanNOH(s);
        }

        //Retired out
        //Format: who's retired out, RO
        //ex: 1RO means batsman on strike retired out, 2RO means batsman off strike retired out
        else if (stats[i].find("RO") != string::npos || stats[i].find("ro") != string::npos || stats[i].find("Ro") != string::npos){
            char strike = stats[i].at(0);
            int s = strike - '0';
            wickets++;
            if (s == 1){
                nextBatsman(false);
            }
            else if (s == 2){
                nextBatsman(true);
            }
        }

        else if ((stats[i].compare("WD") == 0) || stats[i].compare("wd") == 0 || stats[i].compare("Wd") == 0){
            prevBowlers[index].runs++;
            prevBowlers[index].wides++;
            maiden = false;
        }

        //No Ball Run Out
        //Format: runs, where run out occurred, NBW
        else if ((stats[i].find("NBW") != string::npos || stats[i].find("nbw") != string::npos || stats[i].find("NBw") != string::npos || stats[i].find("nbW") != string::npos || stats[i].find("Nbw") != string::npos) && stats[i].length() > 2){
            prevBowlers[index].nb++;
            char runs = stats[i].at(0);
            int r = runs - '0';
            prevBowlers[index].runs += r;
            maiden = false;

            int in = getBatsmanOnStrike();
            batsmen[in].runs += (r - 1);
            batsmen[in].ballsFaced++;
            if (r %2 == 1){
                updateStrike(in);
            }
            wickets++;
            char e = stats[i].at(1);
            int end = e - '0';
            //Run out occurred at the striker's end
            if (end == 1){
                nextBatsman(false);
            }
            //Run out occurred at the non striker's end
            else if (end == 2){
                nextBatsman(true);
            }
        }

        //Run out
        //Format: runs, where run out occurred, W
        //Ex: 11W means 1 run and run out occurred at the striker's end
        //02W means no runs and run out occurred at the non striker's end
        else if ((stats[i].find("W") != string::npos || stats[i].find("w") != string::npos) && stats[i].length() > 1){
            prevBowlers[index].overs += 0.1;
            char runs = stats[i].at(0);
            int r = runs - '0';
            prevBowlers[index].runs += r;
            if (r != 0){
                maiden = false;
            }

            int in = getBatsmanOnStrike();
            batsmen[in].runs += r;
            batsmen[in].ballsFaced++;
            if (r %2 == 0){
                updateStrike(in);
            }
            wickets++;
            char e = stats[i].at(1);
            int end = e - '0';
            //Run out occurred at the striker's end
            if (end == 1){
                nextBatsman(false);
            }
            //Run out occurred at the non striker's end
            else if (end == 2){
                nextBatsman(true);
            }
        }
        
        else if (stats[i].find("NB") != string::npos || stats[i].find("nb") != string::npos || stats[i].find("Nb") != string::npos){
            prevBowlers[index].nb++;
            char runs = stats[i].at(0);
            int r = runs - '0';
            prevBowlers[index].runs += r;
            maiden = false;

            int in = getBatsmanOnStrike();
            batsmen[in].runs += (r - 1);
            batsmen[in].ballsFaced++;
            if ((r - 1) == 4){
                batsmen[in].fours++;
            }
            if ((r - 1) == 6){
                batsmen[in].sixes++;
            }
            if ((r - 1) %2 == 1){
                updateStrike(in);
            }
        }

        else{
            prevBowlers[index].overs += 0.1;
            int runs = stoi(stats[i]);
            prevBowlers[index].runs += runs;
            if (runs != 0){
                maiden = false;
            }

            int in = getBatsmanOnStrike();
            batsmen[in].runs += runs;
            batsmen[in].ballsFaced++;
            if (runs == 4){
                batsmen[in].fours++;
            }
            if (runs == 6){
                batsmen[in].sixes++;
            }
            if (runs %2 == 1){
                updateStrike(in);
            }
        }
    }
    //Showing that one full over has been bowled
    if (((int)(10*prevBowlers[index].overs) % 10) == 6){
        prevBowlers[index].overs += 0.4;
    }
    //checking if maiden is true and a full over has been bowled
    if (maiden == true && (int)(10*prevBowlers[index].overs) % 10 == 0){
        prevBowlers[index].maidens++;
    }
    //Update strike at the end of the over
    if (sub == false){
        updateStrike(getBatsmanOnStrike());
    }
}

//Fixes the batsman vector's stats in case someone was not out hurt and they came back later
void fixBatsmanStats(){
    for (int i = 0; i < batsmen.size();i++){
        if (batsmen[i].noh == true){
            string name = batsmen[i].name;
            for (int j = i + 1; j < batsmen.size();){
                if (name.compare(batsmen[j].name) == 0){
                    batsmen[i].runs += batsmen[j].runs;
                    batsmen[i].ballsFaced += batsmen[j].ballsFaced;
                    if (batsmen[j].out == true){
                        batsmen[i].out = true;
                    }
                    batsmen[i].fours += batsmen[j].fours;
                    batsmen[i].sixes += batsmen[j].sixes;
                    batsmen[i].ducks += batsmen[j].ducks;

                    batsmen.erase(batsmen.begin() + j);
                }
                else{
                    j++;
                }
            }
        }
    }
}

void displayBowlerStats(){
    for (int i = 0; i < prevBowlers.size(); i++){
        cout << "Bowler Name: " << prevBowlers[i].name << endl;
        cout << "Stats: " << prevBowlers[i].overs << "-" << prevBowlers[i].maidens << "-" << prevBowlers[i].runs << "-" << prevBowlers[i].wickets << endl;
        double ballsBowled = 6*((int) prevBowlers[i].overs) + ((int)(10*prevBowlers[i].overs) % 10);
        double econ = 6*prevBowlers[i].runs/ballsBowled;
        cout << "Economy: " << econ << endl;
        cout << "The number of wides bowled is " << prevBowlers[i].wides << endl;
        cout << "The number of no balls bowled is " << prevBowlers[i].nb << endl;
        cout << endl;
    }
}

void displayBatsmanStats(){
    for (int i = 0; i < batsmen.size(); i++){
        cout << "Batsman Name: " << batsmen[i].name << endl;
        cout << "Stats: " << batsmen[i].runs;
        if (batsmen[i].out == false){
            cout << "*";
        }
        cout << "(" << batsmen[i].ballsFaced << ")" << endl;
        cout << "Strike Rate: " << 100*batsmen[i].runs/(double) batsmen[i].ballsFaced << endl;
        cout << "Fours hit: " << batsmen[i].fours << endl;
        cout << "Sixes hit: " << batsmen[i].sixes << endl;
        cout << "Ducks: " << batsmen[i].ducks << endl;
        cout << endl;
    }
}

void displayScore(){
    int ballsBowled = 0;
    int runs = 0;
    for (int i = 0; i < prevBowlers.size(); i++){
        runs += prevBowlers[i].runs;
        ballsBowled += 6*((int) prevBowlers[i].overs) + ((int)(10*prevBowlers[i].overs) % 10);
    }
    double overs = ((int) (ballsBowled/6.0)) + ((ballsBowled % 6)/10.0);
    cout << "Total score: " << runs << "/" << wickets << " in " << overs << " overs" << endl;
}

//Returns the index at which the person is in the spreadsheet
int findSpreadsheetIndex(string name){
    int index = -1;
    for (int i = 0; i < spreadsheet.size();i++){
        if (spreadsheet[i][0].compare(name) == 0){
            index = i;
            break;
        }
    }
    return index;
}

//Updates Batting Statistics for one player
void updateBatting(int sp_index, int b_index){
    spreadsheet[sp_index][2] = to_string(stoi(spreadsheet[sp_index][2]) + batsmen[b_index].runs);
    spreadsheet[sp_index][3] = to_string(stoi(spreadsheet[sp_index][3]) + batsmen[b_index].ballsFaced);

    //Updating High Score
    int hs;
    if (spreadsheet[sp_index][6].find("*") != string::npos){
        string highScore = spreadsheet[sp_index][6];
        highScore = highScore.substr(0, highScore.length() - 1);
        hs = stoi(highScore);
    }
    else{
        hs = stoi(spreadsheet[sp_index][6]);
    }
    if (batsmen[b_index].runs >= hs){
        spreadsheet[sp_index][6] = to_string(batsmen[b_index].runs);
        if (batsmen[b_index].out == false && batsmen[b_index].ballsFaced != 0){
            spreadsheet[sp_index][6] += "*";
        }
    }

    spreadsheet[sp_index][7] = to_string(stoi(spreadsheet[sp_index][7]) + batsmen[b_index].fours);
    spreadsheet[sp_index][8] = to_string(stoi(spreadsheet[sp_index][8]) + batsmen[b_index].sixes);
    spreadsheet[sp_index][9] = to_string(stoi(spreadsheet[sp_index][9]) + batsmen[b_index].ducks);
    //# of times got out
    if(batsmen[b_index].out == true){
        spreadsheet[sp_index][10] = to_string(stoi(spreadsheet[sp_index][10]) + 1);
    }
    //# of bat innings
    if (batsmen[b_index].ballsFaced != 0){
        spreadsheet[sp_index][11] = to_string(stoi(spreadsheet[sp_index][11]) + 1);
    }

    //update batting average
    if (stoi(spreadsheet[sp_index][10]) != 0){
        spreadsheet[sp_index][4] = to_string(stod(spreadsheet[sp_index][2])/stod(spreadsheet[sp_index][10]));
    }

    //update strike rate
    if (stoi(spreadsheet[sp_index][3]) != 0){
        spreadsheet[sp_index][5] = to_string(100*stod(spreadsheet[sp_index][2])/stod(spreadsheet[sp_index][3]));
    }
}

//Updates bowling statistics for one player
void updateBowling(int sp_index, int b_index){
    spreadsheet[sp_index][12] = to_string(stoi(spreadsheet[sp_index][12]) + prevBowlers[b_index].wickets);
    spreadsheet[sp_index][16] = to_string(stoi(spreadsheet[sp_index][16]) + prevBowlers[b_index].runs);

    int ballsBowled = 6*((int) prevBowlers[b_index].overs) + ((int)(10*prevBowlers[b_index].overs) % 10);
    spreadsheet[sp_index][17] = to_string(stoi(spreadsheet[sp_index][17]) + ballsBowled);

    //# of bowling innings
    if (prevBowlers[b_index].overs > 0){
        spreadsheet[sp_index][18] = to_string(stoi(spreadsheet[sp_index][18]) + 1);
    }
    spreadsheet[sp_index][19] = to_string(stoi(spreadsheet[sp_index][19]) + prevBowlers[b_index].maidens);
    spreadsheet[sp_index][20] = to_string(stoi(spreadsheet[sp_index][20]) + prevBowlers[b_index].wides);
    spreadsheet[sp_index][21] = to_string(stoi(spreadsheet[sp_index][21]) + prevBowlers[b_index].nb);

    //update bowling average
    if (stoi(spreadsheet[sp_index][12]) != 0){
        spreadsheet[sp_index][13] = to_string(stod(spreadsheet[sp_index][16])/stod(spreadsheet[sp_index][12]));
    }

    //update economy
    if (stoi(spreadsheet[sp_index][17]) != 0){
        double econ = 6*stod(spreadsheet[sp_index][16])/stod(spreadsheet[sp_index][17]);
        spreadsheet[sp_index][14] = to_string(econ);
    }

    //update best figures
    if (spreadsheet[sp_index][15].compare("") == 0){
        spreadsheet[sp_index][15] = to_string(prevBowlers[b_index].wickets) + "//" + to_string(prevBowlers[b_index].runs);
    }
    else{
        int first_slash = spreadsheet[sp_index][15].find("/");
        int w = stoi(spreadsheet[sp_index][15].substr(0, first_slash));
        int runs = stoi(spreadsheet[sp_index][15].substr((first_slash + 2), (spreadsheet[sp_index][15].length() - first_slash + 2)));

        if (prevBowlers[b_index].wickets > w){
            spreadsheet[sp_index][15] = to_string(prevBowlers[b_index].wickets) + "//" + to_string(prevBowlers[b_index].runs);
        }
        else if (prevBowlers[b_index].wickets == w){
            if(prevBowlers[b_index].runs < runs){
                spreadsheet[sp_index][15] = to_string(prevBowlers[b_index].wickets) + "//" + to_string(prevBowlers[b_index].runs);
            }
        }
    }
}

void updateSpreadsheet(){
    //Update Batting
    for (int i = 0; i < batsmen.size(); i++){
        int index = findSpreadsheetIndex(batsmen[i].name);
        if (index == -1){
            cout << "Error: Batsman is not listed in the spreadsheet\n";
            cout << "The Batsman's name is " << batsmen[i].name << endl;
        }
        else{
            updateBatting(index, i);
        }
    }

    //Update Bowling
    for (int i = 0; i < prevBowlers.size(); i++){
        int index = findSpreadsheetIndex(prevBowlers[i].name);
        if (index == -1){
            cout << "Error: Bowler is not listed in the spreadsheet\n";
            cout << "The Bowler's name is " << prevBowlers[i].name << endl;
        }
        else{
            updateBowling(index, i);
        }
    }
}

void printSpreadsheet(){
    for (int i = 0; i < spreadsheet.size(); i++){
        for (int j = 0; j < spreadsheet[i].size();j++){
            cout << spreadsheet[i][j] << ", ";
        }
        cout << endl;
    }
}

bool writeRecordToNewSpreadsheet(string filename){
    string headers[] = {"Name", "Grade","Runs Scored", "Balls Faced", "Batting Average","Strike Rate","High Score", "# of fours hit", "# of sixes hit", "# of ducks", "# of times got out","# of bat innings","Wickets taken", "Bowling Average", "Economy", "Best figures", "Runs given","Legal Balls bowled","# of bowl innings", "# of maidens", "# of wides", "# of no balls"};
    ofstream file;
    file.open(filename, ios_base::app);
    //putting in the headers
    for (int i = 0; i < spreadsheet[0].size(); i++){
        file << headers[i] << ",";
    }
    file << endl;

    //putting in spreadsheet data
    for (int i = 0; i < spreadsheet.size(); i++){
        for (int j = 0; j < spreadsheet[0].size();j++){
            file << spreadsheet[i][j] << ",";
        }
        file << endl;
    }
    file.close();
    return true;
}

//Initializing batting lineup in batsmen vector
void initializeBattingLineup(string input){
    ifstream batting("battinglineup.txt");
    string s;
    while(getline(batting,s)){
        batsman newBat = batsman(s);
        batsmen.push_back(newBat);
    }
    batting.close();
    batsmen[0].onStrike = true;
    batsmen[0].currentlyBatting = true;
    batsmen[1].currentlyBatting = true;
}

void analyzeGame(string input, string output){
    ifstream record;
    string line;
    record.open(input);
    int lineno = 0;
    while(getline(record,line)){
        if(lineno != 0){
            stringstream ss(line);
            string word;
            vector <string> rows;
            while(getline(ss, word, ',')){
                rows.push_back(word);
            }
            spreadsheet.push_back(rows);
        }
        lineno++;
    }
    record.close();
    printSpreadsheet();
    updateSpreadsheet();
    printSpreadsheet();
    writeRecordToNewSpreadsheet(output);
}

int main(){
    string batting = "battinglineup.txt";
    initializeBattingLineup(batting);

    //Parse input of game
    ifstream f("input.txt");
    string l;
    while (getline(f, l)){
        parseInput(l);
    }
    f.close();
    fixBatsmanStats();

    displayBowlerStats();
    displayBatsmanStats();
    displayScore();

    string input = "UpdatedCricketData.csv";
    string output = "UpdatedCricketDatav2.csv";
    analyzeGame(input, output);
}
