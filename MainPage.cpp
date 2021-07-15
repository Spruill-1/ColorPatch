#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace winrt::Microsoft::Graphics::Canvas;
using namespace winrt::Windows::Graphics::DirectX;

namespace winrt::ColorPatch::implementation
{
    MainPage::MainPage() : m_device(nullptr)
    {
        InitializeComponent();

        // Add color patches
        // TODO: yes I realize this is terribly factored and all of this should be in the Patch class
        for (UINT i = 0; i < PanelCount; i++)
        {
            auto newPatch = UI::Xaml::CanvasSwapChainPanel();
            newPatch.Width(DefaultWidth); newPatch.Height(DefaultHeight);
            grid().Children().Append(newPatch);
            grid().SetColumn(newPatch, i % (grid().ColumnDefinitions().Size()-1) + 1);
            grid().SetRow(newPatch, i / (grid().ColumnDefinitions().Size()-1));
            m_patches.push_back({ nullptr, newPatch, ColorList[i][0], ColorList[i][1], ColorList[i][2] });
        }
    }
}

void winrt::ColorPatch::implementation::MainPage::DrawSwapChain()
{
    if (!m_device) m_device = CanvasDevice();

    int i = 0;
    for (auto&& patch : m_patches)
    {
        if (!patch.patch)
        {
            patch.patch = std::make_shared<Patch>(m_device);
            patch.patch->SetColors(patch.red, patch.green, patch.blue);
        }

        patch.panel.SwapChain(patch.patch->GetSwapChain());
        patch.patch->Draw(patch.panel.DesiredSize());
    }
}

void winrt::ColorPatch::implementation::MainPage::CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
{
    DrawSwapChain();
}

void winrt::ColorPatch::implementation::MainPage::SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e)
{
    DrawSwapChain();
}

void winrt::ColorPatch::implementation::MainPage::CanvasControl_CreateResources(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args)
{
    DrawSwapChain();
}


void winrt::ColorPatch::implementation::MainPage::Refresh_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
