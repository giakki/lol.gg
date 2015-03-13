#pragma once

namespace strings {

const char* const banner[7] = { R"( ___       ________  ___           ________  ________     )",
                                R"(|\  \     |\   __  \|\  \         |\   ____\|\   ____\    )",
                                R"(\ \  \    \ \  \|\  \ \  \        \ \  \___|\ \  \___|    )",
                                R"( \ \  \    \ \  \\\  \ \  \        \ \  \  __\ \  \  ___  )",
                                R"(  \ \  \____\ \  \\\  \ \  \____  __\ \  \|\  \ \  \|\  \ )",
                                R"(   \ \_______\ \_______\ \_______\\__\ \_______\ \_______\)",
                                R"(    \|_______|\|_______|\|_______\|__|\|_______|\|_______|)" };

const char* const expectedBuildsWarning = "Warning: only found %d of %d valid builds.";

const char* const finished =
    "Finished!\nYou can now copy the generated 'Config' directory over to your League of Legends "
    "installation folder.\n";

}  // namespace strings
