#include "img.hpp"
#include "MouseHandler.hpp"
#include "ClickTranslator.hpp"

#include <iostream>


int main()
{
    try
    {
        Img board;

        board.read("board.png");


        // לדוגמה: לוח 8x8, תא בגודל 100 פיקסלים
        ClickTranslator translator(
            8,      // rows
            8,      // cols
            100     // cell size
        );


        MouseHandler mouse(translator);


        mouse.setOnClick(
            [](const Position& pos)
            {
                std::cout
                    << "Clicked cell: row="
                    << pos.row
                    << " col="
                    << pos.col
                    << std::endl;
            }
        );


        board.show();


        mouse.attachTo("Image");


        while(true)
        {
            int key = cv::waitKey(50) & 0xFF;


            if(key == 27) // ESC
                break;
        }


        cv::destroyAllWindows();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: "
                  << e.what()
                  << std::endl;

        return 1;
    }


    return 0;
}