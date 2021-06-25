/*#include "Computer.h"
#include "Board.h"

#include "CallFunc.h"
#include "DelayTime.h"
#include "Sequence.h"

#define DIFICULTY_STEP 20

std::shared_ptr<Computer> Computer::create(std::shared_ptr<Board> board, int difficulty)
{
    auto ret = std::make_shared<Computer>();
    if (ret) {
        ret->board = board;
        ret->moveCounter = 0;
        ret->difficulty = difficulty;
        return ret;
    }
    return nullptr;
}

void Computer::updateEX(float dt)
{
    Node::updateEX(dt);

    moveCounter++;

    if (moveCounter >= DIFICULTY_STEP - difficulty) {
        moveCounter = 0;

        if (!board->getTouching() && !board->getInCombo() && !board->getPaused()) {

            auto piecePair = board->getPossibleRandomPiecePair();

            if (piecePair.first == nullptr || piecePair.second == nullptr) {
                return;
            }

            board->swapPieceGridPositions(piecePair);
            auto possibilityA = board->getBreakPossibility(piecePair.first);
            auto possibilityB = board->getBreakPossibility(piecePair.second);
            board->swapPieceGridPositions(piecePair);

            if (possibilityA || possibilityB) {
                board->tryBreak(piecePair);
            }

        }

    }
}
*/