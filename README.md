# CricketClubScoreAnalysis
This code takes in two .txt files and one .csv file. The .txt files are the scoresheet and batting lineup for one innings and the .csv file is the current cricket club statistics. The code analyzes the scoresheet and creates a new .csv file with the updated statistics.

How to format the batting lineup:
  Enter each batsman's name on separate lines, ensuring no spaces are entered after each name
  Ex:
  Rohit Sharma
  Jemimah Rodrigues
  Joe Root
  Hayley Matthews
  Kate Cross
  Tim Southee
  
How to format the scoresheet:
  Each over is one line in the scoresheet
  First, enter the bowler's name, then a colon, then a space, then the outcomes of the deliveries in the over
  In all cases where relevant, the striker's end is denoted by the number "1" and the non-striker's end is denoted by the      number "2"
  For regular deliveries, simply write the number of runs scored, ex: "0", "1", "4", etc.
  For wickets due to catches or being bowled, write "W" or "w"
  For wides write "WD", "wd", or "Wd"
  For no balls write the total number of runs plus NB, ex: "1NB", "3nb", or "7Nb"
  For run outs write runs gained, where the run out occurred, and W, ex: "11W" or "02w"
  For run outs on a no ball write runs gained, where the run out occurred, and NBW, ex: "12NBW"
  If a batsman retires not out hurt, write who retired (striker's or non-striker's end) and NOH, ex: "1NOH" or "2noh"
  If a batsman retires out, write who retired (striker's or non-striker's end) and RO, ex: "1RO" or "2ro"
  If a bowler is subbed out in the middle of an over, write "-s" at the end of the line and start a new line
    ex:
      Watson: W 0 W -s
      Ashwin: 6 4 1
  
  Ex:
  Keshav Maharaj: 0 1 4 W WD 5NB 2RO 2 11W
  Jasprit Bumrah: W 0 4 6 1NOH 12NBW 0 1
  Ellyse Perry: 2 3 1 0 6 02W

The .csv file contains the following columns:
"Name", "Grade","Runs Scored", "Balls Faced", "Batting Average","Strike Rate","High Score", "# of fours hit", "# of sixes hit", "# of ducks", "# of times got out","# of bat innings","Wickets taken", "Bowling Average", "Economy", "Best figures", "Runs given","Legal Balls bowled","# of bowl innings", "# of maidens", "# of wides", "# of no balls"
  
