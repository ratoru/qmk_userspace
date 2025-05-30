#pragma once

#if defined(BINDINGS_OS_DYNAMIC) || defined(OS_DETECTION_ENABLE)
/**
 * @brief Callback for whether the host uses Mac vs. Windows/Linux hotkeys.
 *
 * Optionally, this callback may be defined to indicate dynamically whether the
 * keyboard is being used with a Mac or non-Mac system.
 *
 * For instance suppose layer 0 is your base layer for Windows and layer 1 is
 * your base layer for Mac. Indicate this by adding in keymap.c:
 *
 *     bool bindings_host_is_mac(void) {
 *       return IS_LAYER_ON(1);  // Supposing layer 1 = base layer for Mac.
 *     }
 */
bool bindings_host_is_mac(void);
#endif // defined(BINDINGS_OS_DYNAMIC) || defined(OS_DETECTION_ENABLE)

// Macro `IS_MAC` determines whether to use Mac vs. Windows/Linux hotkeys:
//
// * OS Detection is used if it is enabled.
//
// * With BINDINGS_OS_DYNAMIC, the user may define callback
//   bindings_host_is_mac().
//
// * Otherwise, the assumed OS is set at compile time, to Mac by
//   default, or to Windows/Linux with BINDINGS_OS_WIN.
#if defined(BINDINGS_OS_DYNAMIC) || defined(OS_DETECTION_ENABLE)
__attribute__((weak)) bool bindings_host_is_mac(void) {
#    ifdef OS_DETECTION_ENABLE // Use OS Detection if enabled.
    switch (detected_host_os()) {
        case OS_LINUX:
        case OS_WINDOWS:
            return false;
        case OS_MACOS:
        case OS_IOS:
            return true;
        default:
            break;
    }
#    endif // OS_DETECTION_ENABLE
#    ifdef BINDINGS_OS_WIN
    return false;
#    else
    return true;
#    endif // BINDINGS_OS_WIN
}
#    define IS_MAC bindings_host_is_mac()
#else
#    ifdef BINDINGS_OS_WIN
#        define IS_MAC false
#    else
#        define IS_MAC true
#    endif // BINDINGS_OS_WIN
#endif     // defined(BINDINGS_OS_DYNAMIC) || defined(OS_DETECTION_ENABLE)
