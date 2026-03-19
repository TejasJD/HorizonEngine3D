#include "HorizonEditor.h"

#include "GameLayer.h"

#include "HorizonEngine/Layer.h"

#include <cassert>

#include <spdlog/spdlog.h>

namespace Hzn
{

auto HorizonEditor::Initialize() -> bool
{
    SPDLOG_INFO("HorizonEditor::{}", __func__);

    auto gameLayer = Layer::Create<GameLayer>();

    if (!AddLayer(gameLayer))
    {
        return false;
    }

    return true;
}

HorizonEditor::~HorizonEditor()
{
    SPDLOG_INFO("HorizonEditor::{}", __func__);
}

} // namespace Hzn