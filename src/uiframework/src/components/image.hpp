#pragma once

#ifndef __IMPL_IMAGE_HPP__
#define __IMPL_IMAGE_HPP__

#include "components/image.hpp"
#include "browser.hpp"

namespace component {
    class Image : public IImage {
    public:
        Image(const std::string& name, Browser* browser, const std::string& target, const VECTOR2I& size)
        : m_size(size) {
            m_name = name;
            m_browser = browser;
            m_target = target;
        }

        void Update(void) override {}

        void Render(void) override {
            m_browser->CallJSFunction("createUIComponent", { "image", m_name, GetId(), m_target, m_size.x, m_size.y });
        }

    private:
        Browser* m_browser;
        VECTOR2I m_size;
    };
}; // namespace component

#endif // __IMPL_IMAGE_HPP__