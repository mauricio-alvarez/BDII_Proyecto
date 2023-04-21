#include <iostream>
#include <string>
#include "SequentialFile.h"

using namespace std;

int main(){
     Rtitles r1{"tm2193", "(500) Days of Summer", "MOVIE", 2009, 95, 7.7, 524841};
     Rtitles r2{"tm100656", "...And God Created Woman", "MOVIE", 1956, 15, 7.7, 8762};
     Rtitles r3{"tm945893", "1, 2, 3, All Eyes On Me", "MOVIE", 2020, 95, 7.7, 0};
     Rtitles r4{"tm700", "10,000 BC", "MOVIE", 2008, 95, 7.7, 132777};
     Rtitles r5{"ts282909", "100 Foot Wave", "SHOW", 2021, 95, 8.1, 2004};
     Rtitles r6{"ts331129", "101 Places to Party Before You Die", "SHOW", 2022, 95, 8.1, 312};

     SequentialFile sf1("data.dat", "auxiliar.dat");
     sf1.add(r1);
     sf1.add(r2);
     sf1.add(r3);
     sf1.add(r4);
     sf1.add(r5);
     sf1.add(r6);







     return EXIT_SUCCESS;
}