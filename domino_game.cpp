#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class Domino {
public:
    Domino(int side1, int side2) : side1(side1), side2(side2) {}

    friend std::ostream& operator<<(std::ostream& os, const Domino& domino) {
        os << "[" << domino.side1 << "|" << domino.side2 << "]";
        return os;
    }

    int getTotalPips() const {
        return side1 + side2;
    }

    int getSide1() const {
        return side1;
    }

    int getSide2() const {
        return side2;
    }

private:
    int side1;
    int side2;
};

class CRandom {
public:
    static void Shuffle(std::vector<Domino>& dominoes) {
        std::random_shuffle(dominoes.begin(), dominoes.end());
    }
};

class CTable {
public:
    static void Display(const std::vector<Domino>& dominoes) {
        for (const Domino& domino : dominoes) {
            std::cout << domino << " ";
        }
        std::cout << std::endl;
    }
};

class CDominoes {
public:
    static std::vector<Domino> Generate() {
        std::vector<Domino> dominoes;
        for (int side1 = 0; side1 <= 6; ++side1) {
            for (int side2 = side1; side2 <= 6; ++side2) {
                dominoes.emplace_back(side1, side2);
            }
        }
        return dominoes;
    }
};

class CPlayer {
public:
    CPlayer(std::vector<Domino>& allDominoes) : dominoes(allDominoes) {}

    void Draw(int numDraw) {
        for (int i = 0; i < numDraw; ++i) {
            hand.push_back(dominoes.back());
            dominoes.pop_back();
        }
    }

    void DisplayHand() {
        for (const Domino& domino : hand) {
            std::cout << domino << " ";
        }
        std::cout << std::endl;
    }

    int GetTotalPips() const {
        int totalPips = 0;
        for (const Domino& domino : hand) {
            totalPips += domino.getTotalPips();
        }
        return totalPips;
    }

    bool MakeMove(int& tableTail, int& tableHead, std::vector<Domino>& playedPieces) {
        for (size_t i = 0; i < hand.size(); ++i) {
            if (hand[i].getSide1() == tableTail) {
                tableTail = hand[i].getSide2();
                playedPieces.push_back(hand[i]);
                hand.erase(hand.begin() + i);
                return true;
            } else if (hand[i].getSide2() == tableTail) {
                tableTail = hand[i].getSide1();
                playedPieces.push_back(hand[i]);
                hand.erase(hand.begin() + i);
                return true;
            } else if (hand[i].getSide1() == tableHead) {
                tableHead = hand[i].getSide2();
                playedPieces.push_back(hand[i]);
                hand.erase(hand.begin() + i);
                return true;
            } else if (hand[i].getSide2() == tableHead) {
                tableHead = hand[i].getSide1();
                playedPieces.push_back(hand[i]);
                hand.erase(hand.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool CanPlay(int tableTail, int tableHead) const {
        for (const Domino& domino : hand) {
            if (domino.getSide1() == tableTail || domino.getSide2() == tableTail ||
                domino.getSide1() == tableHead || domino.getSide2() == tableHead) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Domino> hand;
    std::vector<Domino>& dominoes;
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<Domino> domino_set = CDominoes::Generate();
    CRandom::Shuffle(domino_set);

    int num_draw = 10;  // Each player draws 10 dominoes

    CPlayer player1(domino_set);
    CPlayer player2(domino_set);

    player1.Draw(num_draw);
    player2.Draw(num_draw);

    std::cout << "Available pieces: ";
    CTable::Display(domino_set);

    std::cout << "Player 1 hand: ";
    player1.DisplayHand();

    std::cout << "Player 2 hand: ";
    player2.DisplayHand();

    int tableTail = 0;  // Start with a piece that has a '0'
    int tableHead = 0;

    std::vector<Domino> playedPieces; // Store played pieces

    CPlayer* currentPlayer = &player1;
    CPlayer* otherPlayer = &player2;

    while (currentPlayer->CanPlay(tableTail, tableHead)) {
        std::cout << "Table: " << tableTail << "..." << tableHead << std::endl;
        std::cout << "Player " << (currentPlayer == &player1 ? "1" : "2") << "'s turn:" << std::endl;

        if (currentPlayer->MakeMove(tableTail, tableHead, playedPieces)) {
            std::cout << "Played a piece from their hand." << std::endl;
        } else {
            if (!domino_set.empty()) {
                currentPlayer->Draw(num_draw);
                std::cout << "Drew " << num_draw << " pieces from available pieces." << std::endl;
                num_draw = 1; // Draw only one piece per turn
            } else {
                std::cout << "No available pieces to draw. Passing the turn." << std::endl;
            }
        }

        std::cout << "Player " << (currentPlayer == &player1 ? "1" : "2") << " hand: ";
        currentPlayer->DisplayHand();

        std::cout << "Played pieces: ";
        CTable::Display(playedPieces);

        std::cout << "Available pieces left: ";
        CTable::Display(domino_set);

        // Swap players
        std::swap(currentPlayer, otherPlayer);
    }

    std::cout << "No valid moves left. Game over!" << std::endl;

    // Determine the winner
    if (player1.GetTotalPips() > player2.GetTotalPips()) {
        std::cout << "Player 1 wins!" << std::endl;
    } else if (player2.GetTotalPips() > player1.GetTotalPips()) {
        std::cout << "Player 2 wins!" << std::endl;
    } else {
        std::cout << "It's a tie!" << std::endl;
    }

    return 0;
}