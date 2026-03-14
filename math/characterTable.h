#ifndef CHARACTERTABLE_H
#define CHARACTERTABLE_H

#include "symmetry.h"
#include <QString>

//http://www.gernot-katzers-spice-pages.com/character%5ftables/index.html
class characterTable
{
public:
    characterTable();

    /*
    typedef struct _msym_representation {
        enum {IRREDUCIBLE = 1, REDUCIBLE = 2} type;
        int d;
        struct {
            int p, v, h, i, l;
        } eig;
        char name[8];
    } msym_representation_t;
    */


    void generateCharacterTable();

    void buildRepresentationsCi();
    void buildRepresentationsCs();
    void buildRepresentationsCn();
    void buildRepresentationsCnh();
    void buildRepresentationsCnv();
    void buildRepresentationsSn();
    void buildRepresentationsDn();
    void buildRepresentationsDnh();
    void buildRepresentationsDnd();

    void buildCharacterTableT();
    void buildCharacterTableTd();
    void buildCharacterTableTh();
    void buildCharacterTableO();
    void buildCharacterTableOh();
    void buildCharacterTableI();
    void buildCharacterTableIh();

    void buildCharacterTableUnknown();

private:
    int d;
    int n,  rl;
    int sopsl;
    QString pointGroupName;





};

#endif // CHARACTERTABLE_H
