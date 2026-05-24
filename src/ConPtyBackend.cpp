#ifdef _WIN32

#include "ConPtyBackend.hpp"

#include "threadsafequeue.hpp"
#include <process.h>
#include <stdexcept>

static void __cdecl PipeListener(LPVOID param) {
    ConPtyBackend* self = static_cast<ConPtyBackend*>(param);
    HANDLE hPipe   = self->m_hPipeIn;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    char  buf[512]{};
    DWORD bytesRead{}, bytesWritten{};

    while (ReadFile(hPipe, buf, sizeof(buf), &bytesRead, nullptr) && bytesRead > 0) {
        std::vector<char> vec_buf(bytesRead);
        std::memcpy(vec_buf.data(), buf, bytesRead);
        self->queue->push(std::move(vec_buf));
    }
}

ConPtyBackend::ConPtyBackend(ThreadSafeQueue<std::vector<char>> *q) : queue(q) {
    // Enable VT processing on the host console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode{};
    if (GetConsoleMode(hConsole, &consoleMode)) {
        SetConsoleMode(hConsole, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    HANDLE hPipePTYOut, hPipePTYIn;
    if (!CreatePipe(&m_hPipeIn, &hPipePTYOut, nullptr, 0) ||
        !CreatePipe(&hPipePTYIn, &m_hPipeOut, nullptr, 0)) {
        throw std::runtime_error("ConPTY: CreatePipe failed");
    }

    COORD consoleSize{ 80, 25 };
    HRESULT hr = CreatePseudoConsole(consoleSize, hPipePTYIn, hPipePTYOut, 0, &m_hPC);

    CloseHandle(hPipePTYIn);
    CloseHandle(hPipePTYOut);

    if (FAILED(hr)) throw std::runtime_error("ConPTY: CreatePseudoConsole failed");

    // Build a STARTUPINFOEX with the pseudo console attribute attached
    STARTUPINFOEX startupInfo{};
    startupInfo.StartupInfo.cb = sizeof(STARTUPINFOEX);

    size_t attrListSize{};
    InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
    startupInfo.lpAttributeList =
        reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(malloc(attrListSize));

    if (!startupInfo.lpAttributeList ||
        !InitializeProcThreadAttributeList(startupInfo.lpAttributeList, 1, 0, &attrListSize)) {
        throw std::runtime_error("ConPTY: InitializeProcThreadAttributeList failed");
    }

    if (FAILED(UpdateProcThreadAttribute(
            startupInfo.lpAttributeList, 0,
            PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
            m_hPC, sizeof(HPCON),
            nullptr, nullptr))) {
        throw std::runtime_error("ConPTY: UpdateProcThreadAttribute failed");
    }

    // Spawn the shell
    wchar_t shellCmd[] = L"pwsh.exe -NoExit -Command \"Set-PSReadLineOption -PredictionSource None\"";
    if (!CreateProcessW(
            nullptr, shellCmd,
            nullptr, nullptr,
            FALSE,
            EXTENDED_STARTUPINFO_PRESENT,
            nullptr, nullptr,
            reinterpret_cast<LPSTARTUPINFOW>(&startupInfo.StartupInfo),
            &m_piClient)) {
        throw std::runtime_error("ConPTY: CreateProcess failed");
    }

    DeleteProcThreadAttributeList(startupInfo.lpAttributeList);
    free(startupInfo.lpAttributeList);

    m_hListener = reinterpret_cast<HANDLE>(_beginthread(PipeListener, 0, this));
}

ConPtyBackend::~ConPtyBackend() {
    // Kill the shell process
    if (m_piClient.hProcess) {
        TerminateProcess(m_piClient.hProcess, 0);
        CloseHandle(m_piClient.hProcess);
        CloseHandle(m_piClient.hThread);
    }

    // Close the pseudo console — this signals EOF to the listener thread
    if (m_hPC != INVALID_HANDLE_VALUE) ClosePseudoConsole(m_hPC);

    // Close our pipe ends
    if (m_hPipeOut != INVALID_HANDLE_VALUE) CloseHandle(m_hPipeOut);
    // m_hPipeIn is owned by the listener thread; closing hPC will unblock it

    if (m_hListener != INVALID_HANDLE_VALUE) {
        WaitForSingleObject(m_hListener, 2000);
        CloseHandle(m_hListener);
        m_hListener = INVALID_HANDLE_VALUE;
    }
}

void ConPtyBackend::write(const std::string& input) {
    DWORD written{};
    WriteFile(m_hPipeOut, input.c_str(),
                     static_cast<DWORD>(input.size()), &written, nullptr);
}

void ConPtyBackend::resize(int cols, int rows) {
    COORD size{ static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
    SUCCEEDED(ResizePseudoConsole(m_hPC, size));
}

#endif // _WIN32
