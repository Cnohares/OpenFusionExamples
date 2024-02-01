#include "Encryption.h"

Encryption::~Encryption() {
    Release(this->InputData);
    Release(this->OutputData);
    Release(this->Key);
    Release(this->IV);
    Release(this->InputStr);
    Release(this->OutputStr);

    Release(this->HashStr);
}

bool Encryption::OpenFile(const wchar_t* FileName) {
    return OpenFileGeneral(FileName, [this] (FILE* fp, long sz) {
        this->InputData = new BYTE[this->InputLength];
        fread(this->InputData, this->InputLength, 1, fp);
        });
}

bool Encryption::SaveFile(const wchar_t* FileName, bool SaveSrc) const {
    PBYTE Output = SaveSrc ? this->InputData : this->OutputData;
    DWORD Length = SaveSrc ? this->InputLength : this->OutputLength;

    if (Output == nullptr) {
        return false;
    }

    FILE* fp = nullptr;

    _wfopen_s(&fp, FileName, L"wb");
    if (fp == nullptr) {
        return false;
    }

    fwrite(Output, Length, 1, fp);
    fclose(fp);

    return true;
}

template<typename T>
void Encryption::SetEncryptData(const T* pBuf, DWORD sz) {
    Release(this->InputData);

    this->InputLength = sz * sizeof(T);
    this->InputData = new BYTE[this->InputLength];
    memcpy(this->InputData, pBuf, this->InputLength);

    return;
}

void Encryption::SetEncryptStr(const std::string& Str) {
    SetEncryptData(Str.c_str(), (DWORD)Str.length());
}

void Encryption::SetEncryptStr(const char* Str, DWORD StrLength) {
    SetEncryptData(Str, StrLength);
}

void Encryption::SetEncryptStr(const std::wstring& Str) {
    SetEncryptData(Str.c_str(), (DWORD)Str.length());
}

void Encryption::SetEncryptStr(const wchar_t* Str, DWORD StrLength) {
    SetEncryptData(Str, StrLength);
}

char* Encryption::GetInputStr() {
    return GetStr(this->InputStr, this->InputData, this->InputLength);
}

DWORD Encryption::GetInputStrLength() const {
    return this->InputLength + 1;
}

void Encryption::ReleaseInputStr() {
    Release(this->InputStr);
    this->InputLength = 0;
}

void Encryption::ReleaseInputData() {
    Release(this->InputData);
    this->InputLength = 0;
}

char* Encryption::GetOutputStr() {
    return GetStr(this->OutputStr, this->OutputData, this->OutputLength);
}

DWORD Encryption::GetOutputStrLength() const {
    return this->OutputLength + 1;
}

void Encryption::ReleaseOutputStr() {
    Release(this->OutputStr);
    this->OutputLength = 0;
}

void Encryption::GenerateKey(const wchar_t* KeyStr) {
	//release old
	Release(this->Key);
	Release(this->IV);

	this->Key = new BYTE[16];
	this->IV = new BYTE[16];

	if (KeyStr == nullptr) {
		memcpy(this->Key, DefaultKey, 16);
		memcpy(this->IV, DefaultIV, 16);

		return;
	}

	const auto keyStrLength = wcslen(KeyStr);

	const DWORD strLength = sizeof(wchar_t) * keyStrLength;
	const DWORD wantedLength = this->KeyLength + this->IVLength;

	// length enough
	if (strLength == wantedLength) {
		memcpy(this->Key, KeyStr, 8 * sizeof(wchar_t));
		memcpy(this->IV, KeyStr + 8, 8 * sizeof(wchar_t));

		return;
	}

	// too long, do hash move
	if (strLength > wantedLength) {
		size_t loop = keyStrLength - 16;
		auto pChar = KeyStr + loop;

		memcpy(this->Key, KeyStr, 8 * sizeof(wchar_t));
		memcpy(this->IV, KeyStr + 8, 8 * sizeof(wchar_t));

		for (; loop != 0; loop--) {
			const auto curChar = pChar[0];

			for (size_t pos = 0; pos < 8; pos++) {
				((wchar_t*)this->Key + pos)[0] ^= curChar;
				((wchar_t*)this->IV + pos)[0] ^= curChar;
			}
		}

		return;
	}

	// too short, loop to fill
	size_t seed = keyStrLength;

	for (size_t loop = keyStrLength; loop != 0; loop--) {
		seed ^= KeyStr[loop - 1] + HASHER_MOVE(seed);
	}

	auto generatedKeyStr = new wchar_t[16];
	memset(generatedKeyStr, 0, 16 * sizeof(wchar_t));
	memcpy(generatedKeyStr, KeyStr, keyStrLength * sizeof(wchar_t));

	for (size_t loop = keyStrLength; loop < 16; loop++) {
		auto pChar = generatedKeyStr + loop - 1;
		wchar_t high = HIBYTE(seed);
		wchar_t low = LOBYTE(seed);
		pChar[0] = high ^ low;

		seed ^= pChar[0] + HASHER_MOVE(seed);
	}
}