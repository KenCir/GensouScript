#include "Script.h"

GensouScript::Value::Value() : data(nullptr), node(nullptr), _next(nullptr), _prev(nullptr), refCount(1), refCircularFlag(0)
{
}

GensouScript::Value::Value(std::wstring name, long double value) : name(name), node(this), _next(nullptr), _prev(nullptr), refCount(1), refCircularFlag(0)
{
	this->data = new Data();
	this->data->type = ValueType::TYPE_REAL;
	this->data->realValue = value;
}

GensouScript::Value::Value(std::wstring name, wchar_t* value) : name(name), node(this), _next(nullptr), _prev(nullptr), refCount(1), refCircularFlag(0)
{
	this->data = new Data();
	this->data->type = ValueType::TYPE_CHAR;
	this->data->charValue = value;
}

GensouScript::Value::Value(std::wstring name, bool value) : name(name), node(this), _next(nullptr), _prev(nullptr), refCount(1), refCircularFlag(0)
{
	this->data = new Data();
	this->data->type = ValueType::TYPE_BOOLEAN;
	this->data->booleanValue = value;
}

GensouScript::Value::~Value()
{
	release();
}

void GensouScript::Value::setNext(Value* next)
{
	this->_next = next;
}

void GensouScript::Value::setPrev(Value* prev)
{
	this->_prev = prev;
}

GensouScript::Value* GensouScript::Value::get()
{
	return this->node;
}

GensouScript::Value* GensouScript::Value::next()
{
	return this->_next;
}

GensouScript::Value* GensouScript::Value::prev()
{
	return this->_prev;
}

void GensouScript::Value::release()
{
	this->refCount--;

	if (this->refCount == 0)
	{
		delete this->data;
		this->_next = nullptr;
	}
	// zŠÂŽQÆ‚ð‹^‚¦
	else
	{
		refScan_1(this);
		refScan_2(this);
	}
}

void GensouScript::Value::refScan_1(Value* value)
{
	this->refCircularFlag |= Value::REF_CIRCULAR_CHECK_FLAG;

	if (value->_next)
	{
		value->_next->refCount--;
		refScan_1(value->_next);
	}
}

void GensouScript::Value::refScan_2(Value* value)
{
	if (!(value->refCircularFlag & Value::REF_CIRCULAR_CHECK_FLAG)) return;

	if (value->refCount > 0)
	{
		refScan_3(value);
	}
	else
	{
		value->refCircularFlag = Value::REF_CIRCULAR_COLLECT_FLAG;
		refScan_2(value->_next);
	}
	
}

void GensouScript::Value::refScan_3(Value* value)
{
	if (!(value->refCircularFlag & Value::REF_CIRCULAR_VALUE_FLAG)) return;

	value->refCircularFlag |= Value::REF_CIRCULAR_VALUE_FLAG;

	if (value->_next)
	{
		value->_next->refCount++;
		refScan_3(value->_next);
	}

	value->release();
}

void GensouScript::Value::refCollect(Value* value)
{
	if (!(value->refCircularFlag & Value::REF_CIRCULAR_VALUE_FLAG)) return;

	value->refCircularFlag |= Value::REF_CIRCULAR_VALUE_FLAG;

	if (value->_next)
	{
		refCollect(value->_next);
	}

	release();
}

GensouScript::NodeMemory::NodeMemory()
{
	mem = NULL;
	size = 0;
	last = NULL;
}

unsigned long GensouScript::NodeMemory::getAddress(std::wstring name)
{
	Value* pt = mem;
	if (pt == NULL) return -1;

	for (unsigned long n = 0; n < size; n++)
	{
		Value* tmp = pt->get();

		if (tmp->getName() == name)  return n;
		pt = pt->next();
	}

	return -1;
}

GensouScript::Value& GensouScript::NodeMemory::getMemory(std::wstring name)
{
	Value* pt = mem;
	for (unsigned long n = 0; n < size; n++)
	{
		Value* tmp = pt->get();
		if (tmp->getName() == name)  return (*tmp);
		pt = pt->next();
	}

	Value* tmp = NULL;
	return *tmp;
}

GensouScript::Value* GensouScript::NodeMemory::getMemoryPointer(std::wstring name)
{
	Value* pt = mem;
	for (unsigned long n = 0; n < size; n++)
	{
		Value* tmp = pt->get();
		if (tmp->getName() == name)  return tmp;
		pt = pt->next();
	}
	Value* tmp = NULL;
	return tmp;
}

unsigned long GensouScript::NodeMemory::getsize()
{
	return size;
}

GensouScript::Value* GensouScript::NodeMemory::createReal(std::wstring name, long double value)
{
	if (this->mem == NULL)
	{
		mem = new Value(name, value);
		last = mem;
		size = 1;
		return mem;
	}

	Value* tmp = new Value(name, value);
	last->setNext(tmp);
	tmp->setPrev(last);
	last = tmp;
	size++;

	return tmp;
}

GensouScript::Value* GensouScript::NodeMemory::createChar(std::wstring name, std::wstring value)
{
	if (this->mem == NULL)
	{
		mem = new Value(name, value.c_str());
		last = mem;
		size = 1;
		return mem;
	}

	Value* tmp = new Value(name, value.c_str());
	last->setNext(tmp);
	tmp->setPrev(last);
	last = tmp;
	size++;

	return tmp;
}

GensouScript::Value* GensouScript::NodeMemory::createBoolean(std::wstring name, bool value)
{
	if (this->mem == NULL)
	{
		mem = new Value(name, value);
		last = mem;
		size = 1;
		return mem;
	}

	Value* tmp = new Value(name, value);
	last->setNext(tmp);
	tmp->setPrev(last);
	last = tmp;
	size++;

	return tmp;
}

GensouScript::Value* GensouScript::NodeMemory::at(unsigned long index)
{
	Value* ret = NULL;
	if (index > size) return ret;

	Value* pt = mem;
	for (unsigned long n = 0; n < index; n++)
	{
		if (pt == NULL) break;
		Value* tmp = pt->next();
		pt = tmp;
	}
	if (pt != NULL) ret = pt->get();

	return ret;
}

void GensouScript::NodeMemory::dump()
{
}
