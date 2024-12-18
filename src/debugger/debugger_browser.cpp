#include <core/debugger/debugger_browser.h>
#include <emscripten.h>

namespace hud::browser
{
    bool debugger::is_present() noexcept
    {
        return EM_ASM_INT({
            // Vérifie si le débogueur est attaché via `debugger` statement ou console inspection
            try
            {
                debugger; // Provoque un arrêt si un débogueur est attaché
                return 1; // Si `debugger` passe, un débogueur est présent
            }
            catch (e)
            {
                return 0; // Sinon, aucun débogueur
            }
        });
    }

    void debugger::break_here() noexcept
    {
        EM_ASM({
            debugger;
        });
    }
} // namespace hud::browser