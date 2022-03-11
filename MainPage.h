﻿#pragma once

#include "MainPage.g.h"

namespace winrt
{
    using namespace winrt::Microsoft::Graphics::Canvas;
    using namespace winrt::Windows::Graphics::Display;
}

static const UINT DefaultWidth = 300, DefaultHeight = 300;

static const UINT PanelCount = 8;

static float ColorList[PanelCount][3] =
{
    {1.f,0.f,0.f},
    {0.f,1.f,0.f},
    {0.f,0.f,1.f},
    {1.f,1.f,1.f},
    {100.f,0.f,0.f},
    {0.f,100.f,0.f},
    {0.f,0.f,100.f},
    {5.f,5.f,5.f}
};

struct ColorPanel
{
    std::shared_ptr<Patch> patch;
    winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasSwapChainPanel panel;
    float red, green, blue;
};
namespace winrt::ColorPatch::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        void DrawSwapChain();
        void CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args);
        void SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e);
        void CanvasControl_CreateResources(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args);
        void Refresh_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        winrt::DisplayInformation m_dispInfo;
        winrt::CanvasDevice m_device;
        std::vector<::ColorPanel> m_patches;
    };
}

namespace winrt::ColorPatch::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
