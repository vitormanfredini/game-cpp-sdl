#pragma once

#include <utility>

enum class Alignment {
  BottomUpCentered,
  Centered,
  TopLeft,
  BottomLeft,
  TopDownCentered,
  UI
};

namespace AlignmentUtils {
    std::pair<float, float> computeOffsets(Alignment alignment, float w, float h) {
        float offsetX = 0, offsetY = 0;
        switch (alignment) {
            case Alignment::BottomUpCentered:
                offsetX = -w / 2;
                offsetY = -h;
                break;
            case Alignment::Centered:
                offsetX = -w / 2;
                offsetY = -h / 2;
                break;
            case Alignment::TopLeft:
                // offsetX = 0;
                // offsetY = 0;
                break;
            case Alignment::BottomLeft:
                // offsetX = 0;
                offsetY = -h;
                break;
            case Alignment::TopDownCentered:
                offsetX = -w / 2;
                // offsetY = 0;
                break;
            case Alignment::UI:
                break;
            default:
                std::cerr << "AlignmentUtils::computeOffsets(): Unsupported alignment." << std::endl;
                break;
        }
        return std::make_pair(offsetX, offsetY);
    }
}
