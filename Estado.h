class Estado {
public:
    vector<Jarro> jarros;
    vector<vector<int>> movimentos;
    Estado* anterior;  // Ponteiro para o estado anterior
    bool visitado;
    Passo passoAteAqui;
    int profundidade;


    // Construtor para a raiz
    Estado(const vector<Jarro>& jarros);

    // Construtor para os demais nós
    Estado(const vector<Jarro>& jarros, Estado* anterior, const Passo passo);

    void defineMovimentos(const vector<vector<int>>& movimentos);

    void marcarComoVisitado();
};

// Construtor para a raiz
Estado::Estado(const vector<Jarro>& jarros)
    : jarros(jarros), anterior(nullptr), visitado(false),passoAteAqui(-1, -1), profundidade(0) {}

// Construtor para os demais nós
Estado::Estado(const vector<Jarro>& jarros, Estado* anterior,const Passo passo)
    : jarros(jarros), anterior(anterior), visitado(false), passoAteAqui(passo) {
        this->profundidade+=1;
    }

// Define os movimentos possíveis para este estado
void Estado::defineMovimentos(const vector<vector<int>>& movimentos) {
    this->movimentos = movimentos;
}

// Marca o estado como visitado
void Estado::marcarComoVisitado() {
    this->visitado = true;
}
