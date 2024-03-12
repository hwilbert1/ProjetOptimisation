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
    float outs = 20.316 - 0.12 * DebitEntrant - 0.588 * ChuteNette - 0.0004 * DebitEntrant * DebitEntrant + 0.014 * DebitEntrant * ChuteNette;
    return outs;
}

float GetP2(float ChuteNette, int DebitEntrant)
{
    float outs = 16.1 - 0.223 * DebitEntrant - 0.465 * ChuteNette - 0.0001 * DebitEntrant * DebitEntrant + 0.016 * DebitEntrant * ChuteNette;
    return outs;
}
float GetP3(float ChuteNette, int DebitEntrant)
{
    float outs = 10.67 - 0.0009 * DebitEntrant - 0.3090 * ChuteNette -0.0005  * DebitEntrant * DebitEntrant + 0.0109 * DebitEntrant * ChuteNette;
    return outs;
}
float GetP4(float ChuteNette, int DebitEntrant)
{
    float outs = 19.6112 - 0.4494 * DebitEntrant - 0.5603 * ChuteNette + 0.0049 * DebitEntrant * DebitEntrant + 0.0112 * DebitEntrant * ChuteNette - 0.0000189 * DebitEntrant * DebitEntrant * DebitEntrant + 0.00001476 * DebitEntrant * DebitEntrant * ChuteNette;
    return outs;
}
float GetP5(float ChuteNette, int DebitEntrant)
{
    float outs = 12.601 - 0.046 * DebitEntrant - 0.363 * ChuteNette - 0.0005 * DebitEntrant * DebitEntrant + 0.013 * DebitEntrant * ChuteNette;
    return outs;
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

    DebitTotal = 578.005676269531;
    ElevAm = 137.899993896484;

    MaxQ1 = 160;
    MaxQ2 = 160;
    MaxQ3 = 0;
    MaxQ4 = 160;
    MaxQ5 = 160;

    float ElevAv = 0; //elevation aval
    int Step = 5; //increment de débit
    int MaxDefault = 160; //maximum par défault du débit d'une turbine
    float ChuteNette = 0;

    ElevAv = GetElevAv(DebitTotal);
    ChuteNette = ElevAm - ElevAv;
       
    int Q5 = 0, P5 = 0;
    int Q4 = 0, P4 = 0;
    int Q3 = 0, P3 = 0;
    int Q2 = 0, P2 = 0;
    int Q1 = 0, P1 = 0;

    std::map<int, int> etape5; //debit entrant (etat), puissance produite; pas de variable de décision car sortie = 0;

    std::map<int, std::map<int, int>> etape4; //debit entrant (etat), debit sortant selectionné (variable de decision), puissance produite;
    std::map<int, std::pair<int, int>> maxetape4; //debit entrant, (debit sortant, puissance produite) tel que la puissante produite est maximale pour cet etat
    
    std::map<int, std::map<int, int>> etape3;
    std::map<int, std::pair<int, int>> maxetape3;
    
    std::map<int, std::map<int, int>> etape2;
    std::map<int, std::pair<int, int>> maxetape2;
    
    std::map<int, int> etape1;//debit sortant selectionné (variable de decision), puissance produite; pas de debit entrant car debit entrant = min(DebitTotal, SommeMaximum)
    std::pair<int, int> maxetape1;  

    
    int SommeMaximum = std::min(MaxDefault, MaxQ5);

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
    SommeMaximum += std::min(MaxDefault, MaxQ4);
    maxetat = std::min(DebitTotal, SommeMaximum);
    for (int i = 0; i <= maxetat; i = i + 5)
    {
        int maxQ = 0;
        int maxP = 0;

        //std::cout << i << " :\t";
        
        
        std::map<int, int> possibilities;
        
        int mini = std::min(MaxQ4, i);
        int minj = std::max(i - PrevSommeMaximum, 0);
        
            for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
            {
             //   std::cout << "(" << j << " ";

                int P4temp = GetP4(ChuteNette, j) + etape5.find(i - j)->second; //calcule la puissance resultante de cette variable de decision dans cet etat

              //  std::cout << P4temp << ")\t";
                possibilities.emplace(j, P4temp);
                if (P4temp > maxP)//garde la meilleur puissance
                {
                    maxP = P4temp;
                    maxQ = j;
                }
            }
        
        
        //std::cout << "\nmax (" << maxQ << " , " << maxP << ")\n";

        maxetape4.emplace(i, std::pair<int, int>(maxQ, maxP));
        etape4.emplace(i, possibilities);
    }
    // ___________________________________________________________________________________________________________________________________________________
    //std::cout << "\netape 3\n";

    PrevSommeMaximum = SommeMaximum;
    SommeMaximum += std::min(MaxDefault, MaxQ3);
    maxetat = std::min(DebitTotal, SommeMaximum);
    for (int i = 0; i <= maxetat; i = i + 5)
    {
        int maxQ = 0;
        int maxP = 0;

        //std::cout << i << " :\t";
        

        std::map<int, int> possibilities;

        int mini = std::min(MaxQ3, i);
        int minj = std::max(i - PrevSommeMaximum, 0);

        for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
        {
          //  std::cout << "(" << j << " ";

            int P3temp = GetP3(ChuteNette, j) + maxetape4.find(i - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

           // std::cout << P3temp << ")\t";
            possibilities.emplace(j, P3temp);
            if (P3temp > maxP)//garde la meilleur puissance
            {
                maxP = P3temp;
                maxQ = j;
            }
        }


        //std::cout << "\nmax (" << maxQ << " , " << maxP << ")\n";

        maxetape3.emplace(i, std::pair<int, int>(maxQ, maxP));
        etape3.emplace(i, possibilities);
    }
    // ___________________________________________________________________________________________________________________________________________________
    //std::cout << "\netape 2\n";

    PrevSommeMaximum = SommeMaximum;
    SommeMaximum += std::min(MaxDefault, MaxQ2);
    maxetat = std::min(DebitTotal, SommeMaximum);
    for (int i = 0; i <= maxetat; i = i + 5)
    {
        int maxQ = 0;
        int maxP = 0;

        
        //std::cout << i << " |\t";

        std::map<int, int> possibilities;

        int mini = std::min(MaxQ2, i);
        int minj = std::max(i - PrevSommeMaximum, 0);

        for (int j = minj; j <= mini; j = j + 5) // j parcours des variables de decision
        {
          //  std::cout << "(" << j << " ";

            int P2temp = GetP2(ChuteNette, j) + maxetape3.find(i - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

           // std::cout << P2temp << ")\t";
            possibilities.emplace(j, P2temp);
            if (P2temp > maxP)//garde la meilleur puissance
            {
                maxP = P2temp;
                maxQ = j;
            }
        }


      //  std::cout << "\nmax (" << maxQ << " , " << maxP << ")\n";

        maxetape2.emplace(i, std::pair<int, int>(maxQ, maxP));
        etape2.emplace(i, possibilities);
    }
    // ___________________________________________________________________________________________________________________________________________________
    //std::cout << "\netape 1\n";

    PrevSommeMaximum = SommeMaximum;
    SommeMaximum += std::min(MaxDefault, MaxQ1);
    Q1 = std::min(DebitTotal, SommeMaximum);
    
    
    int maxQ1 = 0;
    int maxP1 = 0;

    //std::cout << Q1 << " |\t";

    int mini1 = std::min(MaxQ1, Q1);
    int minj1 = std::max(Q1 - PrevSommeMaximum, 0);

    for (int j = minj1; j <= mini1; j = j + 5) // j parcours des variables de decision
    {
       // std::cout << "(" << j << " ";

        int P1temp = GetP1(ChuteNette, j) + maxetape2.find(Q1 - j)->second.second; //calcule la puissance resultante de cette variable de decision dans cet etat

       // std::cout << P1temp << ")\t";
        etape1.emplace(j, P1temp);
        if (P1temp > maxP1)//garde la meilleur puissance
        {
            maxP1 = P1temp;
            maxQ1 = j;
        }
    }
    //std::cout << "\nmax (" << maxQ1 << " , " << maxP1 << ")\n";
    maxetape1 = std::pair<int, int>(maxQ1, maxP1);
    
    std::cout << "total P " << maxP1 << "\n";
    std::cout << "179,17\n";
    std::cout << "maxP1 " << GetP1(ChuteNette, MaxQ1) << "\tmaxQ1 "<< maxQ1 << "\n";
    
    int tempQ = DebitTotal - maxQ1;
    //std::cout << "TempQ " << tempQ << "\n";
    std::cout << "maxP2 " << GetP2(ChuteNette, maxetape2.find(tempQ)->second.first) << "\tmaxQ2 " << maxetape2.find(tempQ)->second.first << "\n";
    
    tempQ = tempQ - maxetape2.find(tempQ)->second.first;
    //std::cout << "TempQ " << tempQ << "\n";
    std::cout << "maxP3 " << GetP3(ChuteNette, maxetape3.find(tempQ)->second.first) << "\tmaxQ3 " << maxetape3.find(tempQ)->second.first << "\n";
    
    tempQ = tempQ - maxetape3.find(tempQ)->second.first;
    //std::cout << "TempQ " << tempQ << "\n";
    std::cout << "maxP4 " << GetP4(ChuteNette, maxetape4.find(tempQ)->second.first) << "\tmaxQ4 " << maxetape4.find(tempQ)->second.first << "\n";

    tempQ = tempQ - maxetape4.find(tempQ)->second.first;
    //std::cout << "TempQ " << tempQ << "\n";
    std::cout << "maxP5 " << GetP5(ChuteNette, tempQ) << "\tmaxQ5 " << tempQ << "\n";

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
