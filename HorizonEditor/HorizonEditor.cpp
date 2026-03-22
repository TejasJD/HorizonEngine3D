#include "HorizonEditor.h"

#include "GameLayer.h"

#include "HorizonEngine/Layer.h"

#include <cassert>

#include <spdlog/spdlog.h>

namespace HznEditor
{

auto EditorApp::Initialize() -> bool
{
    SPDLOG_INFO("HorizonEditor::{}", __func__);

    auto gameLayer = Hzn::Layer::Create<HznEditor::GameLayer>();

    if (!AddLayer(gameLayer))
    {
        return false;
    }

    return true;
}

EditorApp::~EditorApp()
{
    SPDLOG_INFO("HorizonEditor::{}", __func__);
}

} // namespace HznEditor