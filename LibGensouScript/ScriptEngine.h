#pragma once
#include "framework.h"
#include <stdexcept>
#include "ConsoleUtility.h"
#include "Script.h"

namespace GensouScript
{
	class ScriptEngineException : public std::runtime_error
	{
	public:
		ScriptEngineException(const char* message) : runtime_error(message)
		{}
	};

	class ScriptFile
	{
	public:
		ScriptFile(std::wstring filePath, Ken_Cir_GT_ConsoleUtility::ConsoleUtility* logger);
		~ScriptFile() {};

	private:
		/** スクリプト構文本体、ここではBufferと定義する */
		std::wstring buffer;

		Ken_Cir_GT_ConsoleUtility::ConsoleUtility* logger;
	};

	class ScriptEngine
	{
	public:
		ScriptEngine(Ken_Cir_GT_ConsoleUtility::ConsoleUtility logger);

	private:
		Ken_Cir_GT_ConsoleUtility::ConsoleUtility logger;
		Value values;
	};
}