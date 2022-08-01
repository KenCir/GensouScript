#pragma once
#include "framework.h"

namespace GensouScript
{
	enum class ValueType
	{
		TYPE_REAL,
		TYPE_CHAR,
		TYPE_BOOLEAN
	};

	class Value
	{
	public:
		const int REF_CIRCULAR_CHECK_FLAG = 1;
		const int REF_CIRCULAR_VALUE_FLAG = 2;
		const int REF_CIRCULAR_COLLECT_FLAG = 3;

		Value();
		Value(std::wstring name, long double value);
		Value(std::wstring name, wchar_t* value);
		Value(std::wstring name, bool value);
		~Value();

		std::wstring getName();
		ValueType getType();
		void setNext(Value* next);
		void setPrev(Value* prev);
		Value* get();
		Value* next();
		Value* prev();
		void release();
		// 循環参照を確認するためのスキャン 1
		void refScan_1(Value* value);
		// 循環参照を確認するためのスキャン 2
		void refScan_2(Value* value);
		// 循環参照を確認するためのスキャン 2
		void refScan_3(Value* value);
		void refCollect(Value* valu);

	private:
		struct Data
		{
			ValueType type;

			union
			{
				long double realValue;
				wchar_t* charValue;
				bool booleanValue;
			};
		};
		
		int refCount;
		// 循環参照対処用のフラグ
		int refCircularFlag;
		std::wstring name;
		Data* data;
		Value* node;
		Value* _next;
		Value* _prev;
	};

	class NodeMemory
	{
	public:
		NodeMemory();
		~NodeMemory();
		unsigned long getAddress(std::wstring name);
		Value& getMemory(std::wstring name);
		Value* getMemoryPointer(std::wstring name);
		unsigned long getsize();
		Value* createReal(std::wstring name, long double value);
		Value* createChar(std::wstring name, std::wstring value);
		Value* createBoolean(std::wstring name, bool value);
		Value* at(unsigned long index);
		void dump();

	private:
		Value* mem;
		Value* last;
		unsigned long size;
	};
}