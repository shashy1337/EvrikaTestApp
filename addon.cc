#include <napi.h>
#include <windows.h>
#include <lm.h>
#include <locale>
#include <codecvt>

#pragma comment(lib, "netapi32.lib")

Napi::String checkUser(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // проверка на переданный аргумент
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Ожидается аргумент с именем пользователя").ThrowAsJavaScriptException();
    return Napi::String::New(env, "invalid");
  }

  // тут в строчку пихаем
  Napi::String username = info[0].As<Napi::String>();

  // для userNetGetInfo
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring usernameWide = converter.from_bytes(username.Utf8Value());
  LPCWSTR usernamePtr = usernameWide.c_str();

  // проверка на юзера
  USER_INFO_1* pUserInfo = NULL;
  DWORD dwError = NetUserGetInfo(NULL, usernamePtr, 1, (LPBYTE*)&pUserInfo);
  if (dwError == NERR_Success) {
    NetApiBufferFree(pUserInfo);
    return Napi::String::New(env, "exists");
  } else if (dwError == NERR_UserNotFound) {
    return Napi::String::New(env, "notfound");
  } else {
    Napi::Error::New(env, "Ошибка при вызове функции NetUserGetInfo").ThrowAsJavaScriptException();
    return Napi::String::New(env, "err");
  }
}

//собираем в метод
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "checkUser"), Napi::Function::New(env, checkUser));
  return exports;
}
// реализуем сборку
NODE_API_MODULE(addon, Init)