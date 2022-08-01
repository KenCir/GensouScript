#include "ScriptEngine.h"
#include "StringUtility.h"

using namespace Ken_Cir_GT_StringUtility;
using namespace Ken_Cir_GT_ConsoleUtility;

GensouScript::ScriptFile::ScriptFile(std::wstring filePath, ConsoleUtility* logger) : buffer(L""), logger(logger)
{
	HANDLE hFile = ::CreateFile(filePath.c_str(), 
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		throw new ScriptEngineException(::StringUtility::Format("File %s Not Found", filePath).c_str());
	}

	// ファイルサイズ
	DWORD dwFileSize = ::GetFileSize(hFile, NULL);
	DWORD dwReadSize;
	if (::ReadFile(hFile, &this->buffer, dwFileSize, &dwReadSize, NULL) == NULL)
	{
		throw new ScriptEngineException(::StringUtility::Format("File %s Read Failed", filePath).c_str());
	}

	this->logger->writeConsole(::StringUtility::Format(L"Read File %s %lo Size", filePath, dwFileSize).c_str());

	::CloseHandle(hFile);
}

GensouScript::ScriptEngine::ScriptEngine()
{
}
