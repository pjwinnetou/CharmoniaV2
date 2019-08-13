/*
        const int npt = 7;
        const int npt1 = 3;
        const int npt2 = 4;

        const double ptBin[8] = {3, 4, 5, 6.5, 8, 10, 15, 50};
        const double ctauBin[7] = {0.067, 0.055, 0.049, 0.043, 0.037, 0.031, 0.025};
        const double x[npt] = {3.5, 4.5, 5.75, 7.25, 9, 12.5, 32.5};

        const double yLow1 = 0;
        const double yLow2 = 1.8;
        const double yHigh = 2.4;

        const int cLow = 40; //20-120 , 40 -80
        const int cHigh = 80;
*/

        ////////////////////////////////////////////////////////ver2 ///////////////////////////////////


        const int npt = 5;
        const int npt1 = 2;
        const int npt2 = 3;

        const double ptBin[npt+1] = {3, 5, 6.5, 9, 15, 50};

       const double ctauBin[npt] = {0.061, 0.049, 0.043, 0.031, 0.025}; // separating prmpt and non pr JPsi
//        const double ctauBin[npt] = {9.999, 9.999, 9.999, 9.999, 9.999}; // inclusive 
        const double x[npt] = {4, 5.75, 7.75, 12, 32.5};

        const double yLow1 = 0;
        const double yLow2 = 1.6;   // ver 3 : yLow2 = 1.6, centrality 20-120
        const double yHigh = 2.4;

        const int cLow = 40;
        const int cHigh = 80;



