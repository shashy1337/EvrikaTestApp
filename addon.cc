#include <napi.h>
#include <windows.h>
#include <lm.h>
#include <locale>
#include <codecvt>

#pragma comment(lib, "netapi32.lib")

Napi::Boolean checkUser(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Проверяем, что аргумент был передан
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Ожидается аргумент с именем пользователя").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    // Преобразуем аргумент в строку
    Napi::String username = info[0].As<Napi::String>();

    // Преобразуем имя пользователя в формат, который используется в NetUserGet
    std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
    std::wstring usernameWide = converter.from_bytes(username.Utf8Value());
    LPCWSTR usernamePtr = usernameWide.c_str();

    // Проверяем, существует ли пользователь
    USER_INFO_1* pUserInfo = NULL;
    DWORD dwError = NetUserGetInfo(NULL, usernamePtr, 1, (LPBYTE*)&pUserInfo);
    if (dwError == NERR_Success) {
        NetApiBufferFree(pUserInfo);
        return Napi::Boolean::New(env, true);
    }
    else if (dwError == NERR_UserNotFound) {
        return Napi::Boolean::New(env, false);
    }
    else {
        Napi::Error::New(env, "Ошибка при вызове функции NetUserGetInfo").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkUser"), Napi::Function::New(env, checkUser));
    return exports;
}

NODE_API_MODULE(addon, Init)
