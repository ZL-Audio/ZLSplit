// Copyright (C) 2024 - zsliu98
// This file is part of ZLSplitter
//
// ZLSplitter is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// ZLSplitter is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLSplitter. If not, see <https://www.gnu.org/licenses/>.

#include "lr_panel.hpp"

namespace zlPanel {
    LRPanel::LRPanel(juce::AudioProcessorValueTreeState &parameter, zlInterface::UIBase &base)
        : uiBase(base),
          mixS("Mix", uiBase) {
        attach({&mixS.getSlider1()}, {zlDSP::mix::ID}, parameter, sliderAttachments);
        mixS.setFontScale(1.25f, 1.25f);
        addAndMakeVisible(mixS);
    }

    void LRPanel::resized() {
        auto bound = getLocalBounds().toFloat();
        bound = bound.withSizeKeepingCentre(bound.getWidth(), bound.getHeight() * .5f);
        mixS.setBounds(bound.toNearestInt());
    }
} // zlPanel
