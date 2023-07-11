#ifndef PASSO_H
#define PASSO_H


class Passo {
    private: 
        int jarro;
        int movimento;
    public:
        Passo();
        Passo(int j, int m);
        void setPasso(int jarro_, int movimento_);
        int getPasso();
        void setJarro(int jarro_);
        int getJarro();
        void setMovimento(int movimento_);
        int getMovimento();
};


#endif