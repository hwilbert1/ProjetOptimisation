// ProjetOptimisation.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <map>


float GetElevAv(int DebitTotal)
{

    return 0.007 * DebitTotal + 99.9;
}

float GetP1(float ChuteNette, int DebitEntrant)
{
    double outs = 20.316 - 0.12 * DebitEntrant - 0.588 * ChuteNette - 0.0004 * DebitEntrant * DebitEntrant + 0.014 * DebitEntrant * ChuteNette;
    return std::max(outs, 0.0);
}

float GetP2(float ChuteNette, int DebitEntrant)
{
    double outs = 16.1 - 0.223 * DebitEntrant - 0.465 * ChuteNette - 0.0001 * DebitEntrant * DebitEntrant + 0.016 * DebitEntrant * ChuteNette;
    return std::max(outs, 0.0);
}
float GetP3(float ChuteNette, int DebitEntrant)
{
    double outs = 10.67 - 0.0009 * DebitEntrant - 0.3090 * ChuteNette -0.0005  * DebitEntrant * DebitEntrant + 0.0109 * DebitEntrant * ChuteNette;
    return std::max(outs, 0.0);
}
float GetP4(float ChuteNette, int DebitEntrant)
{
    double outs = 19.6112 - 0.4494 * DebitEntrant - 0.5603 * ChuteNette + 0.0049 * DebitEntrant * DebitEntrant + 0.0112 * DebitEntrant * ChuteNette - 0.0000189 * DebitEntrant * DebitEntrant * DebitEntrant + 0.00001476 * DebitEntrant * DebitEntrant * ChuteNette;
    return std::max(outs, 0.0);
}
float GetP5(float ChuteNette, int DebitEntrant)
{
    double outs = 12.601 - 0.046 * DebitEntrant - 0.363 * ChuteNette - 0.0005 * DebitEntrant * DebitEntrant + 0.013 * DebitEntrant * ChuteNette;
    return std::max(outs, 0.0);
}

int main()
{
        //paramètre en entrée d’un instance : débit à repartir, élévation en amont, limitation eventuelle pour chaque turbine

        int DebitTotal = 0;
        float ElevAm = 0; //elevation amont
        int MaxQ1 = 0; //maximum temporaire de Q1
        int MaxQ2 = 0;
        int MaxQ3 = 0;
        int MaxQ4 = 0;
        int MaxQ5 = 0;


        DebitTotal = std::atoi("string");

        ElevAm = 137.89;
        DebitTotal = 561.26;
        
        MaxQ1 = 0;
        MaxQ2 = 160;
        MaxQ3 = 160;
        MaxQ4 = 160;
        MaxQ5 = 160;
        float ElevAvReel = 103.68;
        float Q1Reel = 0;
        float P1Reel = 0;
        float Q2Reel = 141;
        float P2Reel = 44.30;
        float Q3Reel = 140;
        float P3Reel = 42.2;
        float Q4Reel = 141.00;
        float P4Reel = 45.18;
        float Q5Reel = 140;
        float P5Reel = 44.53;
        









        float ElevAv = 0; //elevation aval
        int Step = 5; //increment de débit
        float ChuteNette = 0;

        ElevAv = GetElevAv(DebitTotal);
        ChuteNette = ElevAm - ElevAv;

        int Q1 = 0, P1 = 0;

        std::map<int, float> etape5; //debit entrant (etat), puissance produite; pas de variable de décision car sortie = 0;

        std::map<int, std::map<int, float>> etape4; //debit entrant (etat), debit sortant selectionné (variable de decision), puissance produite;
        std::map<int, std::pair<int, float>> maxetape4; //debit entrant, (debit sortant, puissance produite) tel que la puissante produite est maximale pour cet etat

        std::map<int, std::map<int, float>> etape3;
        std::map<int, std::pair<int, float>> maxetape3;

        std::map<int, std::map<int, float>> etape2;
        std::map<int, std::pair<int, float>> maxetape2;

        std::map<int, float> etape1;//debit sortant selectionné (variable de decision), puissance produite; pas de debit entrant car debit entrant = min(DebitTotal, SommeMaximum)
        std::pair<int, float> maxetape1;


        int SommeMaximum = MaxQ5;

        //std::cout << "etape 5\n";
        int maxetat = std::min(DebitTotal, SommeMaximum);
        for (int i = 0; i <= maxetat; i = i + 5)  //i : parcours des etat
        {
            //std::cout << i << " : " << GetP5(ChuteNette, i) << "\n";
            etape5.emplace(i, GetP5(ChuteNette, i));

        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 4\n";

        int PrevSommeMaximum = SommeMaximum;
        SommeMaximum += MaxQ4;
        maxetat = std::min(DebitTotal, SommeMaximum);
        for (int i = 0; i <= maxetat; i = i + 5)
        {


            std::map<int, float> possibilities;

            int mini = std::min(MaxQ4, i);
            int minj = std::max(i - PrevSommeMaximum, 0);

            int maxQ = minj;
            float maxP = GetP4(ChuteNette, minj) + etape5.find(i - minj)->second;

            for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
            {
                //   std::cout << "(" << j << " ";

                float P4temp = GetP4(ChuteNette, j) + etape5.find(i - j)->second; //calcule la puissance resultante de cette variable de decision dans cet etat

                //  std::cout << P4temp << ")\t";
                possibilities.emplace(j, P4temp);
                if (P4temp > maxP)//garde la meilleur puissance
                {
                    maxP = P4temp;
                    maxQ = j;
                }
            }


         //   std::cout << i << " " << maxQ << " , " << maxP << ")\n";

            maxetape4.emplace(i, std::pair<int, float>(maxQ, maxP));
            etape4.emplace(i, possibilities);
        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 3\n";

        PrevSommeMaximum = SommeMaximum;
        SommeMaximum += MaxQ3;
        maxetat = std::min(DebitTotal, SommeMaximum);
        for (int i = 0; i <= maxetat; i = i + 5)
        {
            
            //std::cout << i << " :\t";


            std::map<int, float> possibilities;

            int mini = std::min(MaxQ3, i);
            int minj = std::max(i - PrevSommeMaximum, 0);

            int maxQ = minj;
            float maxP = GetP3(ChuteNette, minj) + maxetape4.find(i - minj)->second.second;

            for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
            {
                //  std::cout << "(" << j << " ";

                float P3temp = GetP3(ChuteNette, j) + maxetape4.find(i - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

                // std::cout << P3temp << ")\t";
                possibilities.emplace(j, P3temp);
                if (P3temp > maxP)//garde la meilleur puissance
                {
                    maxP = P3temp;
                    maxQ = j;
                }
            }


            //std::cout << "\nmax (" << maxQ << " , " << maxP << ")\n";

            maxetape3.emplace(i, std::pair<int, float>(maxQ, maxP));
            etape3.emplace(i, possibilities);
        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 2\n";

        PrevSommeMaximum = SommeMaximum;
        SommeMaximum += MaxQ2;
        maxetat = std::min(DebitTotal, SommeMaximum);
        int tempint = 0;
        for (int i = 0; i <= maxetat; i = i + 5)
        {
            


            //std::cout << i << " |\t";

            std::map<int, float> possibilities;

            int mini = std::min(MaxQ2, i);
            int minj = std::max(i - PrevSommeMaximum, 0);
            
            int maxQ = minj;
            float maxP = GetP2(ChuteNette, minj) + maxetape3.find(i - minj)->second.second;

            for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
            {
                //  std::cout << "(" << j << " ";

                float P2temp = GetP2(ChuteNette, j) + maxetape3.find(i - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

                // std::cout << P2temp << ")\t";
                possibilities.emplace(j, P2temp);
                if (P2temp >= maxP)//garde la meilleur puissance
                {
                    maxP = P2temp;
                    maxQ = j;
                }
            }

            tempint = i;
             //std::cout << minj << " " << mini << " max (" << maxQ << " , " << maxP << ")\n";

            maxetape2.emplace(i, std::pair<int, float>(maxQ, maxP));
            etape2.emplace(i, possibilities);
        }
        // ___________________________________________________________________________________________________________________________________________________
        //std::cout << "\netape 1\n";

        PrevSommeMaximum = tempint;
        SommeMaximum += MaxQ1;
        Q1 = std::min(DebitTotal, SommeMaximum);


        int maxQ1 = 0;
        float maxP1 = GetP1(ChuteNette, 0) + maxetape2.find(std::min(DebitTotal, PrevSommeMaximum))->second.second;

        if (MaxQ1 != 0)
        {

            //std::cout << Q1 << " |\t";

            int mini1 = std::min(MaxQ1, Q1);
            int minj1 = std::max(Q1 - PrevSommeMaximum, 0);


            maxQ1 = minj1;
            maxP1 = GetP1(ChuteNette, minj1) + maxetape2.find(Q1 - minj1)->second.second;


            for (int j = minj1; j <= mini1; j = j + 5) // j parcours des variables de decision
            {
                // std::cout << "(" << j << " ";

                float P1temp = GetP1(ChuteNette, j) + maxetape2.find(Q1 - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

                // std::cout << P1temp << ")\t";
                etape1.emplace(j, P1temp);
                if (P1temp > maxP1)//garde la meilleur puissance
                {
                    maxP1 = P1temp;
                    maxQ1 = j;
                }
            }
            //std::cout << "\nmax (" << maxQ1 << " , " << maxP1 << ")\n";
            maxetape1 = std::pair<int, float>(maxQ1, maxP1);
        }


        std::cout << ElevAm << "\n";
        std::cout << DebitTotal << "\n";

        std::cout << ElevAv / ElevAvReel * 100 << "\n";

        std::cout << (int)(maxQ1 / Q1Reel * 100 ) << " " << (int)(GetP1(ChuteNette, maxQ1) / P1Reel * 100 )<<  "\n";

        int tempQ = DebitTotal - maxQ1;
        if (MaxQ1 == 0)
        {
            tempQ = std::min(DebitTotal, PrevSommeMaximum);
        }
        std::cout << (int)( maxetape2.find(tempQ)->second.first / Q2Reel * 100 ) <<" " << (int)(GetP2(ChuteNette, maxetape2.find(tempQ)->second.first) / P2Reel * 100) << "\n";


        tempQ = tempQ - maxetape2.find(tempQ)->second.first;
        
        std::cout << (int)(maxetape3.find(tempQ)->second.first / Q3Reel * 100) << " " << (int)(GetP3(ChuteNette, maxetape3.find(tempQ)->second.first) / P3Reel * 100) << "\n";

        tempQ = tempQ - maxetape3.find(tempQ)->second.first;
        std::cout << (int)(maxetape4.find(tempQ)->second.first / Q4Reel * 100) << " " << (int)(GetP4(ChuteNette, maxetape4.find(tempQ)->second.first) / P4Reel * 100) << "\n";

        tempQ = tempQ - maxetape4.find(tempQ)->second.first;
        std::cout << (int)(tempQ / Q5Reel * 100) << " " << (int)(GetP5(ChuteNette, tempQ) / P5Reel * 100) << "\n";

        /*
        std::cout << "\n";
        std::cout << "EvalAv " << GetElevAv(578) << " - 103,77 \n";
        std::cout << "158 - P1 " << GetP1(34, 158) << " - 45,76 \n"; //45,76
        //std::cout << "147 - P1 " << GetP1(34, 147) << " - 43,1416 \n"; //45,76
        std::cout << "141 - P2 " << GetP2(34, 141) << " - 44,18\n"; //44,18
        std::cout << "142 - P3 " << GetP3(34, 142) << " - 42,51\n"; //42,51
        std::cout << "140 - P4 " << GetP4(34, 140) << " - 44,84\n"; //44,84
        std::cout << "140 - P5 " << GetP5(34, 140) << " - 44,40\n"; //44,40
        */

}