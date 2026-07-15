#include "ClickTranslator.hpp"


std::optional<Position> ClickTranslator::pixelToCell(int x, int y) const
{
    // הורדת ההיסט של הלוח בתוך החלון
    int localX = x - offsetX_;
    int localY = y - offsetY_;

    // לחיצה מחוץ לאזור הלוח
    if (localX < 0 || localY < 0)
        return std::nullopt;


    int col = localX / cellSize_;
    int row = localY / cellSize_;


    // בדיקה שהשורה והעמודה קיימות בלוח
    if (row >= rows_ || col >= cols_)
        return std::nullopt;


    return Position{row, col};
}