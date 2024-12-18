#include "study_in_pink1.h"

bool readFile(
        const string &filename,
        int &HP1,
        int &HP2,
        int &EXP1,
        int &EXP2,
        int &M1,
        int &M2,
        int &E1,
        int &E2,
        int &E3
) {
    // This function is used to read the input file,
    // DO NOT MODIFY THIS FUNTION
    ifstream ifs(filename);
    if (ifs.is_open()) {
        ifs >> HP1 >> HP2
            >> EXP1 >> EXP2
            >> M1 >> M2
            >> E1 >> E2 >> E3;
        return true;
    } else {
        cerr << "The file is not found" << endl;
        return false;
    }
}

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

//Function use to get the correct value of HP
void checkingHP(int &hp)
{
    if (hp > 666)
    {
        hp = 666;
    }
    else if (hp < 0)
    {
        hp = 0;
    }
}

//Function use to get the correct value of EXP
void checkingEXP(int &exp)
{
    if (exp > 600)
    {
        exp = 600;
    }
    else if (exp < 0)
    {
        exp = 0;
    }
}

//Function use to get the correct value of M
void checkingMONEY(int &money)
{
    if (money > 3000)
    {
        money = 3000;
    }
    else if (money < 0)
    {
        money = 0;
    }
}

//Function use to check if the E value is in the right range
bool checkingEVENT(int &eventCode)
{
    return (eventCode >= 0 && eventCode <= 99);
}

//Function use to calculate and round up the result
void roundUpAndCalculate(int &value, double factor, char operation)
{
    //If the operation is addition
    if (operation == 'a')
    {
        //Perform the operation then round up the result
        value = (double)value + factor + 0.9999;
    }
    //If the operation is subtraction
    else if (operation == 's')
    {
        //Perform the operation then round up the result
        value = (double)value - factor + 0.9999;
    }
    //If the operation is multiplication
    else if (operation == 'm')
    {
        //Perform the operation then round up the result
        value = (double)value * factor + 0.9999;
    }
}

/*=============================================================== TASK 1 =====================================================================*/

int firstMeet(int &exp1, int &exp2, int e1)
{
    // TODO: Complete this function

    //If E1 is not in range, immidiately return -99
    if (checkingEVENT(e1) == false)
    {
        return -99;
    }

    //Correcting the input 
    checkingEXP(exp1);
    checkingEXP(exp2);

    //If E1 is in range [0,3]
    if (e1 >= 0 && e1 <= 3)
    {
        switch (e1)
        {
        case 0:
            //Watson's EXP increase by 29
            exp2 += 29;
            checkingEXP(exp2);
            break;

        case 1:
            //Watson's EXP increase by 45
            exp2 += 45;
            checkingEXP(exp2);
            break;

        case 2:
            //Watson's EXP increase by 75
            exp2 += 75;
            checkingEXP(exp2);
            break;

        case 3:
            //Watson's EXP increase by 149
            exp2 += 149;
            checkingEXP(exp2);
            break;
        }

        //Define Sherlock's decision variable D
        int sherlockDecision = e1 * 3 + exp1 * 7;

        //If Sherlock's decision is not an even number, decrease Sherlock's EXP by D/100
        if (sherlockDecision % 2 != 0)
        {
            roundUpAndCalculate(exp1, (double)sherlockDecision / 100, 's');
            checkingEXP(exp1);
        }
        else //If Sherlock's decision is an even number, increase Sherlock's EXP by D/200
        {
            roundUpAndCalculate(exp1, (double)sherlockDecision / 200, 'a');
            checkingEXP(exp1);
        }
    }

    //If E1 is in range [4,99]
    if (e1 >= 4 && e1 <= 99)
    {
        //If E1 is in range [4,19] 
        if (e1 >= 4 && e1 <= 19)
        {
            //Watson's EXP increase by E1/4 + 19
            roundUpAndCalculate(exp2, (double)e1 / 4 + 19, 'a');
            checkingEXP(exp2);

            //Sherlock's EXP decrease by E1 
            exp1 -= e1;
            checkingEXP(exp1);
        }
        //If E1 is in range [20,49] 
        else if (e1 >= 20 && e1 <= 49)
        {
            //Watson's EXP increase by E1/9 + 21
            roundUpAndCalculate(exp2, (double)e1 / 9 + 21, 'a');
            checkingEXP(exp2);

            //Sherlock's EXP decrease by E1
            exp1 -= e1;
            checkingEXP(exp1);
        }
        //If E1 is in range [50,65] 
        else if (e1 >= 50 && e1 <= 65)
        {
            //Watson's EXP increase by E1/16 + 17
            roundUpAndCalculate(exp2, (double)e1 / 16 + 17, 'a');
            checkingEXP(exp2);

            //Sherlock's EXP decrease by E1
            exp1 -= e1;
            checkingEXP(exp1);
        }
        //If E1 is in range [66,79] 
        else if (e1 >= 66 && e1 <= 79)
        {
            //Watson's EXP increase by E1/4 + 19
            roundUpAndCalculate(exp2, (double)e1 / 4 + 19, 'a');
            checkingEXP(exp2);

            //If Watson's EXP greater than 200
            if (exp2 > 200)
            {
                //Watson's EXP increase by E1/9 + 21
                roundUpAndCalculate(exp2, (double)e1 / 9 + 21, 'a');
                checkingEXP(exp2);
            }

            //Sherlock's EXP decrease by E1
            exp1 -= e1;
            checkingEXP(exp1);
        }
        //If E1 is in range [80,99] 
        else if (e1 >= 80 && e1 <= 99)
        {
            //Watson's EXP increase by E1/4 + 19
            roundUpAndCalculate(exp2, (double)e1 / 4 + 19, 'a');
            checkingEXP(exp2);

            //Watson's EXP increase by E1/9 + 21
            roundUpAndCalculate(exp2, (double)e1 / 9 + 21, 'a');
            checkingEXP(exp2);

            //If Watson's EXP greater than 400
            if (exp2 > 400)
            {
                //Watson's EXP increase by E1/16 + 17
                roundUpAndCalculate(exp2, (double)e1 / 16 + 17, 'a');
                checkingEXP(exp2);

                //All 3 information is gived, Watson's EXP increase by 15% of itself
                roundUpAndCalculate(exp2, 1.15, 'm');
                checkingEXP(exp2);
            }

            //Sherlock's EXP decrease by E1
            exp1 -= e1;
            checkingEXP(exp1);
        }
    }
    //Return the sum of Sherlock & watson EXP
    return exp1 + exp2;
}

/*=============================================================== TASK 2 =====================================================================*/

//Function use to get the probability on each roads
double determineProbability(int exp1)
{
    //Get the nearest perfect square of exp1 
    double s = round(sqrt(exp1)) * round(sqrt(exp1));

    //Define the probability variable
    double p;

    //If exp1 greater or equal to s, p = 100%
    if (exp1 >= s)
    {
        p = 1;
    }
    else //Else, p = (exp1 / s + 80) /123
    {
        p = (exp1 / s + 80) / 123;
    }
    //Return the probability value
    return p;
}

//Calculation during the first event on Road 2
void event01(int &HP1, int &M1, int &paidMoney)
{
    //If Sherlock's HP smaller than 200
    if (HP1 < 200)
    {
        //Sherlock's HP increase by 30% of itself
        roundUpAndCalculate(HP1, 1.3, 'm');
        checkingHP(HP1);

        //Sherlock has to pay 150 and his budget reduces by 150
        paidMoney += 150;
        M1 -= 150;
        checkingMONEY(M1);
    }
    else //If Sherlock's HP is not smaller than 200
    {
        //Sherlock's HP increase by 10% of itself
        roundUpAndCalculate(HP1, 1.1, 'm');
        checkingHP(HP1);

        //Sherlock has to pay 70 and his budget reduces by 70
        paidMoney += 70;
        M1 -= 70;
        checkingMONEY(M1);
    }
}

//Calculation during the second event on Road 2
void event02(int &EXP1, int &M1, int &paidMoney)
{
    //If Sherlock's EXP smaller than 400
    if (EXP1 < 400)
    {
        //Sherlock has to pay 200 and his budget reduces by 200
        paidMoney += 200;
        M1 -= 200;
        checkingMONEY(M1);
    }
    else
    {
        //Sherlock has to pay 120 and his budget reduces by 120
        paidMoney += 120;
        M1 -= 120;
        checkingMONEY(M1);
    }
    //After all, Sherlock's EXP increase by 13% of itself
    roundUpAndCalculate(EXP1, 1.13, 'm');
    checkingEXP(EXP1);
}

//Calculation during the third event on Road 2
void event03(int &EXP1, int &M1, int &paidMoney)
{
    //If Sherlock's EXP smaller than 300
    if (EXP1 < 300)
    {
        //Sherlock has to pay 100 and his budget reduces by 100
        paidMoney += 100;
        M1 -= 100;
        checkingMONEY(M1);
    }
    else
    {
        //Sherlock has to pay 120 and his budget reduces by 120
        paidMoney += 120;
        M1 -= 120;
        checkingMONEY(M1);
    }
    //After all, Sherlock's EXP decrease by 10% of itself
    roundUpAndCalculate(EXP1, 0.9, 'm');
    checkingEXP(EXP1);
}

int traceLuggage(int &HP1, int &EXP1, int &M1, int E2)
{
    // TODO: Complete this function

    //If E1 is not in range, immidiately return -99
    if (checkingEVENT(E2) == false)
    {
        return -99;
    }

    //Correcting the input
    checkingEXP(EXP1);
    checkingHP(HP1);
    checkingMONEY(M1);

    /*************************************************************** ROAD 01 **********************************************************************/
    
    //Define the probability variable on road 1 
    double p1;

    //Calculate the value of p1
    p1 = determineProbability(EXP1);

    /*************************************************************** ROAD 02 **********************************************************************/
    
    //Define the probability variable on road 2
    double p2;

    //Calculate the limit (half of Sherlock's budget)
    double limit = 0.5 * M1;

    //Define the money that Sherlock had paid 
    int paidMoney = 0;

    //If E2 is an odd number and Sherlock's budget is not empty
    if (E2 % 2 != 0 && M1 != 0)
    {
        //Repeating doing the event 1, event 2, event 3 until the paid money is greater than the limit
        while (paidMoney <= limit)
        {
            //Doing event 1
            event01(HP1, M1, paidMoney);

            //If the paid money has greater than the limit
            if (paidMoney > limit)
            {
                break;
            }

            //Doing event 2
            event02(EXP1, M1, paidMoney);

            //If the paid money has greater than the limit
            if (paidMoney > limit)
            {
                break;
            }

            //Doing event 3
            event03(EXP1, M1, paidMoney);

            //If the paid money has greater than the limit
            if (paidMoney > limit)
            {
                break;
            }
        }
    }
    else if (E2 % 2 == 0 && M1 != 0) //If E2 is an even number and Sherlock's budget is not empty
    {
        //Set a while true loop
        while (M1 > 0)
        {
            //Doing event 1
            event01(HP1, M1, paidMoney);

            //If Sherlock's budget is empty, break out of the loop
            if (M1 == 0)    
            {
                break;
            }

            //Doing event 2
            event02(EXP1, M1, paidMoney);

            //If Sherlock's budget is empty, break out of the loop
            if (M1 == 0)
            {
                break;
            }

            //Doing event 3
            event03(EXP1, M1, paidMoney);

            //After doing the third event, which mean one round of event has finised, break out of the loop
            break;
        }
    }
    //After all, Sherlock's HP decrease by 17% and Sherlock's EXP increase by 17%
    roundUpAndCalculate(HP1, 0.83, 'm');
    checkingHP(HP1);

    roundUpAndCalculate(EXP1, 1.17, 'm');
    checkingEXP(EXP1);

    //Calculate the value of p1
    p2 = determineProbability(EXP1);

    /*************************************************************** ROAD 03 **********************************************************************/
    //Define the probability variable on road 3
    double p3;

    //Define the probability array
    double P[10] = {0.32, 0.47, 0.28, 0.79, 1, 0.5, 0.22, 0.83, 0.64, 0.11};

    //Define the result index 
    int i;

    //If E2 is a one-digit number
    if (E2 < 10)
    {
        //The result index is E2
        i = E2;

        //Get the value of P[E2]
        p3 = P[i];
    }
    else //If E2 is a two-digit number
    {
        //Get the sum of two digit in E2
        i = E2 % 10 + E2 / 10;

        //Get the unit row of the sum and get the value of P[unit row of sum]
        p3 = P[i % 10];
    }

    // If p1 = p2 = p3 = 100%
    if (p1 == 1 && p2 == 1 && p3 == 1)
    {
        //Sherlock's EXP decrease by 25% of itself
        roundUpAndCalculate(EXP1, 0.75, 'm');
        checkingEXP(EXP1);
    }
    else
    {
        //Define the average value of probabilities
        double pAverage = (p1 + p2 + p3) / 3;

        //If the average is smaller than 50%
        if (pAverage < 0.5)
        {
            //Sherlock's HP decrease by 15% of itself and Sherlock's EXP increase by 15% of itself
            roundUpAndCalculate(HP1, 0.85, 'm');
            checkingHP(HP1);

            roundUpAndCalculate(EXP1, 1.15, 'm');
            checkingEXP(EXP1);
        }
        else //If the average is greater or equal to 50%
        {
             //Sherlock's HP decrease by 10% of itself and Sherlock's EXP increase by 20% of itself
            roundUpAndCalculate(HP1, 0.9, 'm');
            checkingHP(HP1);

            roundUpAndCalculate(EXP1, 1.2, 'm');
            checkingEXP(EXP1);
        }
    }
    //Return the sum of Sherlock HP, EXP and M
    return HP1 + EXP1 + M1;
}

/*=============================================================== TASK 3 =====================================================================*/
//Return the one-digit number is the sum of two digit inside a two-digit number
int sumOfTwoDigits(int meetingPosition)
{
    //If meetingPosition is a one-digit number
    if (meetingPosition < 10)
    {
        //Return it
        return meetingPosition;
    }
    else
    {
        //Calculate the sum of two digit inside meetingPosition
        int sum = 0;
        sum += meetingPosition % 10 + meetingPosition / 10;

        //Using recursive to get the correct result of this sum if the sum is a two-digit number
        return sumOfTwoDigits(sum);
    }
}

//Calculate the grade at each position on the Sherlock & Watson map
int sherlockWatsonGradeGenerated(int rowIndex, int columnIndex, int taxiMap[10][10])
{
    int sherlockWatsonPositionGrade = taxiMap[rowIndex][columnIndex];
    int sumOfPosition = rowIndex + columnIndex;
    int subtractOfPosition = rowIndex - columnIndex;

    //Get the maximum on the right diagonal
    for (int i = 0; i <= 9; i++)
    {
        if (sumOfPosition - i <= 9 && sumOfPosition - i >= 0 && sherlockWatsonPositionGrade < taxiMap[i][sumOfPosition - i])
        {
            sherlockWatsonPositionGrade = abs(taxiMap[i][sumOfPosition - i]);
        }
    }

    //Get the maximum on the left diagonal
    for (int i = 0; i <= 9; i++)
    {
        if (i + subtractOfPosition <= 9 && i + subtractOfPosition >= 0 && sherlockWatsonPositionGrade < taxiMap[i + subtractOfPosition][i])
        {
            sherlockWatsonPositionGrade = abs(taxiMap[i + subtractOfPosition][i]);
        }
    }
    //Return the final maximum value of the left & right diagonal
    return sherlockWatsonPositionGrade;
}

int chaseTaxi(int &HP1, int &EXP1, int &HP2, int &EXP2, int E3)
{
    // TODO: Complete this function

    //If E3 is not in range, immidiately return -99
    if (checkingEVENT(E3) == false)
    {
        return -99;
    }

    //Correcting the input
    checkingHP(HP1);
    checkingHP(HP2);
    checkingEXP(EXP1);
    checkingEXP(EXP2);

    //Define the index of meeting column, meeting row, taxi and Sherlock & Watson grad on that meeting point
    int meetingColumn = 0, meetingRow = 0;
    int taxiGrade = 0, sherlockWatsonGrade = 0;

    //Define the matrix for taxi and Sherlock & Watson
    int taxiMap[10][10];
    int sherlockMap[10][10];

    //Initialize all 0 on taxi map
    for (int row = 0; row < 10; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            taxiMap[row][column] = 0;
        }
    }

    //Calculate the grade on each position on taxi map (row by row)
    for (int row = 0; row < 10; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            taxiMap[row][column] = ((E3 * column) + (row * 2)) * (row - column);
        }
    }

    //Calculate the grade on each position on Sherlock & Watson map (column by column)
    for (int column = 0; column < 10; column++)
    {
        for (int row = 0; row < 10; row++)
        {
            sherlockMap[row][column] = sherlockWatsonGradeGenerated(row, column, taxiMap);
        }
    }

    //Find the index of meeting point
    for (int row = 0; row < 10; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            //If the grade on taxi map point is greater than 2 * E3 
            if (taxiMap[row][column] > 2 * E3)
            {
                //Meeting row index increase by 1
                meetingRow++;
            }
            //If the grade on taxi map point is smaller than -E3 
            else if (taxiMap[row][column] < -E3)
            {
                //Meeting column index increase by 1
                meetingColumn++;
            }
        }
    }

    //Get the final index of meeting column and meeting row
    meetingColumn = sumOfTwoDigits(meetingColumn);
    meetingRow = sumOfTwoDigits(meetingRow);

    //Get taxi grade and Sherlock & Watson grade on the meeting position
    taxiGrade = taxiMap[meetingRow][meetingColumn];
    sherlockWatsonGrade = sherlockMap[meetingRow][meetingColumn];

    //If the absolute value of taxi grade on the meeting position is greater than Sherlock & Watson grade on the meeting position
    if (abs(taxiGrade) > sherlockWatsonGrade)
    {
        //Sherlock's EXP and Watson's EXP decrease by 12%
        roundUpAndCalculate(EXP1, 0.88, 'm');
        checkingEXP(EXP1);

        roundUpAndCalculate(EXP2, 0.88, 'm');
        checkingEXP(EXP2);

        //Sherlock's HP and Watson's HP decrease by 10%
        roundUpAndCalculate(HP1, 0.9, 'm');
        checkingHP(HP1);

        roundUpAndCalculate(HP2, 0.9, 'm');
        checkingHP(HP2);

        //Return the taxi grade
        return taxiGrade;
    }
    else //If the absolute value of taxi grade on the meeting position is not greater than Sherlock & Watson grade on the meeting position
    {
        //Sherlock's EXP and Watson's EXP increase by 12%
        roundUpAndCalculate(EXP1, 1.12, 'm');
        checkingEXP(EXP1);

        roundUpAndCalculate(EXP2, 1.12, 'm');
        checkingEXP(EXP2);

        //Sherlock's HP and Watson's HP increase by 10%
        roundUpAndCalculate(HP1, 1.1, 'm');
        checkingHP(HP1);

        roundUpAndCalculate(HP2, 1.1, 'm');
        checkingHP(HP2);

        //Return the Sherlock & Watson grade
        return sherlockWatsonGrade;
    }
}

/*=============================================================== TASK 4 =====================================================================*/
//Get the size of the const char * datatype
int sizeOfConstCharX(const char *s)
{
    int sSize = 0;
    while (s[sSize] != '\0')
    {
        sSize++;
    }
    return sSize;
}

//Checking if se is a substring of s
int checkingSe(const char *s, string se, int sSize)
{
    //If se is a empty string, then se appears in the first position of s
    if (se == "\0")
    {
        return 0;
    }

    int seSize = se.size();
    int checkSe = -1;

    //Find the first index that se appears in s
    for (int i = 0; i <= sSize - seSize; i++)
    {
        if (s[i] == se[0] && s[i + seSize - 1] == se[seSize - 1])
        {
            for (int j = 0; j <= seSize - 1; j++)
            {
                if (s[i + j] != se[j])
                {
                    checkSe = -1;
                    break;
                }
                else
                {
                    checkSe = i;
                }
            }
        }
        if (checkSe != -1)
        {
            break;
        }
    }

    //Return the index of s in se
    return checkSe;
}

//Checking if s has more than 2 consecutive characters
int checkingConsecutive(const char *s, int sSize)
{
    int checkConsecutive = -1;
    
    for (int i = 0; i <= sSize - 3; i++)
    {
        if (s[i] == s[i + 1] && s[i] == s[i + 2])
        {
            checkConsecutive = i;
            break;
        }
    }
    //Return the index of s that have more than 2 consecutive characters
    return checkConsecutive;
}

//Checking if s has special characters or not
bool checkingSpecial(const char *s, int sSize)
{
    char specialCharacters[5] = {'@', '#', '%', '$', '!'};

    //Find special character in s 
    for (int i = 0; i < sSize; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (specialCharacters[j] == s[i])
            {
                return true;
            }
        }
    }
    return false;
}

//Checking if there is a strange character in s
int checkingStrange(const char *s, int sSize)
{
    int strangeIndex = 0;
    bool validCheck = false;

    for (int i = 0; i < sSize; i++)
    {
        validCheck = false;
        //If s[i] is a special character
        if (s[i] == '@' || s[i] == '#' || s[i] == '%' || s[i] == '$' || s[i] == '!')
        {
            strangeIndex++;
            validCheck = true;
            continue;
        }
        //If s[i] is a upperacase character
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            strangeIndex++;
            validCheck = true;
            continue;
        }
        //If s[i] is a number character
        if (s[i] >= '0' && s[i] <= '9')
        {
            strangeIndex++;
            validCheck = true;
            continue;
        }
        //If s[i] is a lowercase character
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            strangeIndex++;
            validCheck = true;
            continue;
        }
        if (validCheck == false)
        {
            break;
        }
    }
    if (validCheck == true)
    {
        return -1;
    }
    return strangeIndex;
}

int checkPassword(const char *s, const char *email)
{
    // TODO: Complete this function
    
    int sSize = sizeOfConstCharX(s);

    //If size of s is smaller than 8, return -1 
    if (sSize < 8)
    {
        return -1;
    }

    //If size of s is smaller than 20, return -2 
    if (sSize > 20)
    {
        return -2;
    }

    //Define string se and get the string before "@" in email and pass it to se
    string se = "\0";

    for (int i = 0; i < sizeOfConstCharX(email); i++)
    {
        if (email[i] == '@')
        {
            break;
        }
        else
        {
            se += email[i];
        }
    }

    //Find the index of se in s, index of more than two consecutive characters in s and index of strange character in s 
    int statusSe = checkingSe(s, se, sSize);
    int statusConsecutive = checkingConsecutive(s, sSize);
    int statusStrange = checkingStrange(s, sSize);

    //If se in s
    if (statusSe != -1)
    {
        //Return -(300 + index of first se in s)
        return -(300 + statusSe);
    }

    //If there are more than 2 consecutive characters in s
    if (statusConsecutive != -1)
    {
        //Return -(400 + index of first consecutive characters)
        return -(400 + statusConsecutive);
    }

    //If there is no special character in s 
    if (!checkingSpecial(s, sSize))
    {
        //Return -5
        return -5;
    }

    //If there is a strange character in s 
    if (statusStrange != -1)
    {
        //Return the index of the first strange character
        return statusStrange;
    }

    //If s is a valid password, return -10
    return -10;
}

/*=============================================================== TASK 5 =====================================================================*/

int findCorrectPassword(const char *arr_pwds[], int num_pwds)
{
    // TODO: Complete this function

    //Define a string array of arr_pwds
    string arr_pwds_string[num_pwds];
    for (int i = 0; i < num_pwds; i++)
    {
        arr_pwds_string[i] = arr_pwds[i];
    }
    //Define the counting variable
    int count = 0;

    //Define the maximum value of appearance
    int max = 0;

    //Define the index of the correct password
    int correctPassIndex;

    //Find the password that appears most in arr_pwds
    for (int i = 0; i < num_pwds; i++)
    {
        count = 0;
        for (int j = i; j < num_pwds; j++)
        {
            if (arr_pwds_string[i] == arr_pwds_string[j])
            {
                count++;
            }
        }
        if (count > max)
        {
            max = count;

            correctPassIndex = i;
        }
        else if (count == max)
        {

            if (sizeOfConstCharX(arr_pwds[i]) > sizeOfConstCharX(arr_pwds[correctPassIndex]))
            {
                correctPassIndex = i;
            }
        }
    }
    //Return the first index of correct password
    return correctPassIndex;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
