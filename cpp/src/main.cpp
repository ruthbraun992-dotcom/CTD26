#include "img.hpp"
#include "MouseHandler.hpp"
#include "ClickTranslator.hpp"
#include "BoardRenderer.hpp"
#include "BoardSetup.hpp"

#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>

namespace
{
std::filesystem::path resolveAssetPath(const std::filesystem::path& relativePath)
{
    std::filesystem::path base = std::filesystem::path(__FILE__).parent_path();
    if (!base.is_absolute())
    {
        base = std::filesystem::absolute(std::filesystem::current_path() / base);
    }

    std::vector<std::filesystem::path> candidates;
    std::filesystem::path current = base;

    while (true)
    {
        candidates.push_back(current / relativePath);
        candidates.push_back(current / "src" / relativePath);
        candidates.push_back(current / "cpp" / relativePath);

        const std::filesystem::path parent = current.parent_path();
        if (parent == current)
        {
            break;
        }

        current = parent;
    }

    candidates.push_back(std::filesystem::current_path() / relativePath);

    for (const auto& candidate : candidates)
    {
        const std::filesystem::path normalized = candidate.lexically_normal();
        if (std::filesystem::exists(normalized))
        {
            return normalized;
        }
    }

    return candidates.front().lexically_normal();
}
}

int main()
{
        

    try
    {
        const std::filesystem::path boardImagePath = resolveAssetPath("C:\\Users\\This User\\Downloads\\board_classic.png");
        Img boardImage;
        
        boardImage.read(boardImagePath.string(), {800, 800}, false);

        const std::filesystem::path spriteDir = resolveAssetPath("../../pieces2");
        SpriteManager spriteManager(spriteDir.string());

        const cv::Mat& img = boardImage.get_mat();
        constexpr int kBoardSize = 800;
        const int cellSize = kBoardSize / 8;
        const int offsetX = 0;
        const int offsetY = 0;
        Board board(8, 8);
        
        setupStartingPosition(board);
        
        BoardRenderer renderer(8, 8, cellSize, spriteDir.string(), offsetX, offsetY);
        std::optional<Position> selectedCell;
        cv::Mat canvas;

        ClickTranslator translator(8, 8, cellSize, offsetX, offsetY);
        MouseHandler mouse(translator);

        mouse.setOnClick([&](const Position& pos)
        {
            selectedCell = pos;
            renderer.setSelectedCell(selectedCell);

            renderer.draw(img, board, canvas);

            cv::imshow("Image", canvas);
        });

        cv::startWindowThread();
        // cv::namedWindow("Image", cv::WINDOW_NORMAL);
        cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
       // cv::resizeWindow("Image", kBoardSize, kBoardSize);

        renderer.setSelectedCell(selectedCell);
        renderer.draw(img, board, canvas);
        cv::imshow("Image", canvas);

        mouse.attachTo("Image");

        while (true)
        {
            int key = cv::waitKey(50) & 0xFF;
            if (key == 27)
                break;
        }

        cv::destroyAllWindows();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}