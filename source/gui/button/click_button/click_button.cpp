// Copyright (C) 2024 - zsliu98
// This file is part of ZLSplitter
//
// ZLSplitter is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// ZLSplitter is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLSplitter. If not, see <https://www.gnu.org/licenses/>.

#include "click_button.hpp"
#include "click_button_look_and_feel.hpp"

namespace zlInterface {
    ClickButton::ClickButton(juce::Drawable *image, UIBase &base)
        : button("", juce::DrawableButton::ButtonStyle::ImageFitted), lookAndFeel(image, base) {
        button.setLookAndFeel(&lookAndFeel);
        addAndMakeVisible(button);
    }

    ClickButton::~ClickButton() {
        button.setLookAndFeel(nullptr);
    }

    void ClickButton::resized() {
        button.setBounds(getLocalBounds());
    }


} // zlInterface
