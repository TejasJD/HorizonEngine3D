#include "HorizonEditor.h"

#include "HorizonEngine/App.h"

#include <cstdlib>

#include <spdlog/spdlog.h>

using namespace Hzn;

int main()
{
    auto appSpec = AppSpecification{{800, 600, "HorizonEditor"}};
    auto app = App::Create<HorizonEditor>(appSpec);
    if (!app)
    {
        return EXIT_FAILURE;
    }

    app->Run();

    return EXIT_SUCCESS;
}