// (C) Copyright Michael Glassford 2004.
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/sync/detail/config.hpp>

#if defined(BOOST_SYNC_DETAIL_PLATFORM_WINAPI) && defined(BOOST_SYNC_DYN_LINK)

#include <windows.h>
#include "tss_windows_hooks.hpp"

#if defined(BOOST_BORLANDC)
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID /*lpReserved*/)
#elif defined(BOOST_EMBTC)
extern "C" int _libmain(DWORD dwReason)
#elif defined(_WIN32_WCE)
extern "C" BOOL WINAPI DllMain(HANDLE /*hInstance*/, DWORD dwReason, LPVOID /*lpReserved*/)
#else
extern "C" BOOL WINAPI DllMain(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID /*lpReserved*/)
#endif
{
    switch(dwReason)
    {
    case DLL_PROCESS_ATTACH:
        {
            boost::sync::detail::windows::on_process_enter();
            boost::sync::detail::windows::on_thread_enter();
            break;
        }

    case DLL_THREAD_ATTACH:
        {
            boost::sync::detail::windows::on_thread_enter();
            break;
        }

    case DLL_THREAD_DETACH:
        {
            boost::sync::detail::windows::on_thread_exit();
            break;
        }

    case DLL_PROCESS_DETACH:
        {
            boost::sync::detail::windows::on_thread_exit();
            boost::sync::detail::windows::on_process_exit();
            break;
        }
    }

    return TRUE;
}

namespace boost {

namespace sync {

namespace detail {

namespace windows {

void tss_cleanup_implemented()
{
    /*
    This function's sole purpose is to cause a link error in cases where
    automatic tss cleanup is not implemented by Boost.Threads as a
    reminder that user code is responsible for calling the necessary
    functions at the appropriate times (and for implementing an a
    tss_cleanup_implemented() function to eliminate the linker's
    missing symbol error).

    If Boost.Threads later implements automatic tss cleanup in cases
    where it currently doesn't (which is the plan), the duplicate
    symbol error will warn the user that their custom solution is no
    longer needed and can be removed.
    */
}

} // namespace windows

} // namespace detail

} // namespace sync

} // namespace boost

#endif // defined(BOOST_SYNC_DETAIL_PLATFORM_WINAPI) && defined(BOOST_SYNC_DYN_LINK)
