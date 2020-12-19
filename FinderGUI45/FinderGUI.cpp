#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <tchar.h>
#include <shobjidl.h> 
#include <atlstr.h>
#include <vector>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

std::wstring filePath;
std::wstring dbpath;
std::string Word = "";
bool isIndex = false;

std::vector<std::string> parceMess(std::string str, std::string delim)
{
    std::vector<std::string> arr;
    size_t prev = 0;
    size_t next;
    size_t delta = delim.length();

    while ((next = str.find(delim, prev)) != std::string::npos)
    {
        std::string tmp = str.substr(prev, next - prev);
        arr.push_back(str.substr(prev, next - prev));
        prev = next + delta;
    }
    std::string tmp = str.substr(prev);
    arr.push_back(str.substr(prev));

    return arr;
}

int CALLBACK  wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
    MSG msg{};
    HWND hwnd{};
    WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"MyAppClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClassEx(&wc))
    {
        return EXIT_FAILURE;
    }


    if (hwnd = CreateWindow(wc.lpszClassName, L"Paint", WS_OVERLAPPEDWINDOW, 0, 0, 1000, 500, nullptr, nullptr, wc.hInstance, nullptr);
        hwnd == INVALID_HANDLE_VALUE)
    {
        return EXIT_FAILURE;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int> (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;


    static HWND hEdit;
    static HWND hListBox;
    static HMODULE hLib;


    switch (messg)
    {
    case WM_CREATE:
    {
        HWND hButtonBrowse = CreateWindow(
            L"BUTTON",
            L"DLL file",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, 150, 50, hWnd, reinterpret_cast<HMENU>(0), nullptr, nullptr
        );
        HWND hButtonFind = CreateWindow(
            L"BUTTON",
            L"Find",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 50, 150, 50, hWnd, reinterpret_cast<HMENU>(1), nullptr, nullptr
        );
        HWND hComboBox = CreateWindow(
            L"COMBOBOX",
            L"Category",
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            0, 125, 150, 50, hWnd, reinterpret_cast<HMENU>(2), nullptr, nullptr
        );
        hListBox = CreateWindow(
            L"LISTBOX",
            NULL,
            WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_NOTIFY,
            200, 0, 750, 450, hWnd, reinterpret_cast<HMENU>(3), nullptr, nullptr
        );
        hEdit = CreateWindow(
            L"EDIT",
            L"Search",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
            0, 100, 150, 25, hWnd, reinterpret_cast<HMENU>(4), nullptr, nullptr
        );
        HWND hFileBrowse = CreateWindow(
            L"BUTTON",
            L"DB file",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 175, 150, 50, hWnd, reinterpret_cast<HMENU>(5), nullptr, nullptr
        );
        HWND hButtonIndex = CreateWindow(
            L"BUTTON",
            L"Index",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 225, 150, 50, hWnd, reinterpret_cast<HMENU>(6), nullptr, nullptr
        );
        HWND hButtonFindBig = CreateWindow(
            L"BUTTON",
            L"Find Big",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 275, 150, 50, hWnd, reinterpret_cast<HMENU>(7), nullptr, nullptr
        );
        HWND hButtonNext = CreateWindow(
            L"BUTTON",
            L"Next",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 325, 150, 50, hWnd, reinterpret_cast<HMENU>(8), nullptr, nullptr
        );
        HWND hButtonPrev = CreateWindow(
            L"BUTTON",
            L"Prev",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 375, 150, 50, hWnd, reinterpret_cast<HMENU>(9), nullptr, nullptr
        );


        SendMessage(GetDlgItem(hWnd, 2), CB_ADDSTRING, 0, (LPARAM)L"Phone");

        SendMessage(GetDlgItem(hWnd, 2), CB_ADDSTRING, 0, (LPARAM)L"Second name");

        SendMessage(GetDlgItem(hWnd, 2), CB_ADDSTRING, 0, (LPARAM)L"Streat");
    }
    return 0;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 0:
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                COINIT_DISABLE_OLE1DDE);
            IFileOpenDialog* pFileOpen;
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
            hr = pFileOpen->Show(NULL);
            IShellItem* pItem;
            LPWSTR pszFilePath;
            hr = pFileOpen->GetResult(&pItem);
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
            const wchar_t* file = pszFilePath;
            std::wstring widestr(file);
            filePath = widestr;
            CoTaskMemFree(pszFilePath);
            pItem->Release();
            pFileOpen->Release();
            CoUninitialize();

            
            hLib = LoadLibrary(filePath.c_str());
        }
        break;
        case 1:
        {
            if (filePath.size() == 0 || dbpath.size() == 0 || !isIndex)
                return 0;
            int textLen = GetWindowTextLengthA(hEdit) + 1;
            TCHAR buff[1024];
            GetWindowText(hEdit, buff, textLen);
            std::wstring buffstr = buff;
            std::string findWord(buffstr.begin(), buffstr.end());
            Word = findWord;
            TCHAR field[1024];
            GetDlgItemText(hWnd, 2, field, 1024);
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
            std::wstring fieldstr = field;
            std::string findField(fieldstr.begin(), fieldstr.end());
            if (findField == "Phone")
            {
                findField = "phone";
            }
            else if (findField == "Second name")
            {
                findField = "secondName";
            }
            else if (findField == "Streat")
            {
                findField = "streat";
            }
            else
            {
                findField = "";
            }

            std::string findedFields;
            std::string(*pFunction)(std::string, std::string);
            (FARPROC&)pFunction = GetProcAddress(hLib, "Find");
            std::string fields = pFunction(findWord, findField);

            if (fields.size() > 0)
            {
                std::vector<std::string> parseFields = parceMess(fields, ";");
                for (int i = 0; i < parseFields.size(); i++)
                {
                    findedFields = "";
                    std::vector<std::string> field = parceMess(parseFields[i], ",");
                    for (int j = 0; j < field.size(); j++)
                    {
                        findedFields += " " + field[j];
                    }
                    std::wstring widestr = std::wstring(findedFields.begin(), findedFields.end());
                    const wchar_t* szName = widestr.c_str();
                    SendMessage(GetDlgItem(hWnd, 3), LB_ADDSTRING, 0, (LPARAM)szName);
                }
            }
        }
        break;
        case 5:
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                COINIT_DISABLE_OLE1DDE);
            IFileOpenDialog* pFileOpen;
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
            hr = pFileOpen->Show(NULL);
            IShellItem* pItem;
            LPWSTR pszFilePath;
            hr = pFileOpen->GetResult(&pItem);
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
            const wchar_t* filePath = pszFilePath;
            std::wstring widestr(filePath);
            dbpath = widestr;
            CoTaskMemFree(pszFilePath);
            pItem->Release();
            pFileOpen->Release();
            CoUninitialize();
        }
        break;
        case 6:
        {
            void(*pFunctionIndex)(const wchar_t*);
            (FARPROC&)pFunctionIndex = GetProcAddress(hLib, "Index");
            pFunctionIndex(dbpath.c_str());
            isIndex = true;
        }
        break;
        case 7:
        {
            if (filePath.size() == 0 || dbpath.size() == 0)
                return 0;
            int textLen = GetWindowTextLengthA(hEdit) + 1;
            TCHAR buff[1024];
            GetWindowText(hEdit, buff, textLen);
            std::wstring buffstr = buff;
            std::string findWord(buffstr.begin(), buffstr.end());
            Word = findWord;
            std::string findedFields;
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

            std::string(*pFunction2)(std::wstring, std::string);
            (FARPROC&)pFunction2 = GetProcAddress(hLib, "ReadFileMem");
            std::string fields = pFunction2(dbpath, findWord);

            if (fields.size() > 0)
            {
                std::vector<std::string> parseFields = parceMess(fields, ";");
                for (int i = 0; i < parseFields.size(); i++)
                {
                    findedFields = "";
                    std::vector<std::string> field = parceMess(parseFields[i], ",");
                    for (int j = 0; j < field.size(); j++)
                    {
                        findedFields += " " + field[j];
                    }
                    std::wstring widestr = std::wstring(findedFields.begin(), findedFields.end());
                    const wchar_t* szName = widestr.c_str();
                    SendMessage(GetDlgItem(hWnd, 3), LB_ADDSTRING, 0, (LPARAM)szName);
                }
            }
        }
        break;
        case 8:
        {
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
            std::string findedFields;
            std::string(*pFunctionIndex)(std::wstring, std::string);
            (FARPROC&)pFunctionIndex = GetProcAddress(hLib, "Next");
            std::string fields  = pFunctionIndex(dbpath, Word);
            if (fields.size() > 0)
            {
                std::vector<std::string> parseFields = parceMess(fields, ";");
                for (int i = 0; i < parseFields.size(); i++)
                {
                    findedFields = "";
                    std::vector<std::string> field = parceMess(parseFields[i], ",");
                    for (int j = 0; j < field.size(); j++)
                    {
                        findedFields += " " + field[j];
                    }
                    std::wstring widestr = std::wstring(findedFields.begin(), findedFields.end());
                    const wchar_t* szName = widestr.c_str();
                    SendMessage(GetDlgItem(hWnd, 3), LB_ADDSTRING, 0, (LPARAM)szName);
                }
            }
        }
        break;
        case 9:
        {
            SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
            std::string findedFields;
            std::string(*pFunctionIndex)(std::wstring, std::string);
            (FARPROC&)pFunctionIndex = GetProcAddress(hLib, "Prev");
            std::string fields = pFunctionIndex(dbpath, Word);
            if (fields.size() > 0)
            {
                std::vector<std::string> parseFields = parceMess(fields, ";");
                for (int i = 0; i < parseFields.size(); i++)
                {
                    findedFields = "";
                    std::vector<std::string> field = parceMess(parseFields[i], ",");
                    for (int j = 0; j < field.size(); j++)
                    {
                        findedFields += " " + field[j];
                    }
                    std::wstring widestr = std::wstring(findedFields.begin(), findedFields.end());
                    const wchar_t* szName = widestr.c_str();
                    SendMessage(GetDlgItem(hWnd, 3), LB_ADDSTRING, 0, (LPARAM)szName);
                }
            }
        }
        break;
        default:
            break;
        }
    }
    return 0;
    case WM_DESTROY:
    {
        PostQuitMessage(EXIT_SUCCESS);
    }
    return 0;


    default:
    {
        return(DefWindowProc(hWnd, messg, wParam, lParam));
    }
    }
    return 0;
}