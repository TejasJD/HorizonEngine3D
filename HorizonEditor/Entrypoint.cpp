#include "HorizonEditor.h"

#include "HorizonEngine/App.h"

#include <cstdlib>

#include <spdlog/spdlog.h>

int main()
{
    auto appSpec = Hzn::AppSpecification{{800, 600, "HorizonEditor"}};
    auto app = Hzn::App::Create<HznEditor::EditorApp>(appSpec);
    if (!app)
    {
        return EXIT_FAILURE;
    }

    app->Run();

    return EXIT_SUCCESS;
}